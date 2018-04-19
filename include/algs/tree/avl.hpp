#include <stdexcept>

namespace algs::tree::bst {

// TODO: iterators instead in_order/in_order_reverse methods
// TODO: find and operator[] return a reference to a key-value pair

/**
 * Recursive implementation of an AVL tree.
 * NOTE: To simplify code, nearly everything is passed by value
 **/
template<typename Key, typename Value>
class avl {
public:
    typedef Key key_type;
    typedef Value mapped_type;
    typedef std::pair<Key, Value> value_type;

private:
    class Node {
    public:
        std::pair<Key, Value> mData;
        Node *mpParent, *mpLeft, *mpRight;
        char mBalance; // -1 for left-heavy, +1 for right-heavy

        Node(Key key, Value value, Node *pParent)
            : mData(std::make_pair(key, value)), mpParent(pParent),
              mpLeft(nullptr), mpRight(nullptr), mBalance(0)
        {}

        Node *next() {
            if (mpRight == nullptr) { // find a parent with an unvisited right subtree
                if (!mpParent)
                    return nullptr;
                Node *pParent = mpParent;
                for (Node *pCurrent = this;
                     pParent && pParent->mpRight == pCurrent;
                     pCurrent = pParent, pParent = pCurrent->mpParent);
                return pParent;
            } else { // find a min key in a right subtree
                Node *pNext = mpRight;
                while (pNext->mpLeft)
                    pNext = pNext->mpLeft;
                return pNext;
            }
        }

        Node *prev() {
            if (mpLeft == nullptr) { // find a parent with an unvisited left subtree
                if (!mpParent)
                    return nullptr;
                Node *pParent = mpParent;
                for (Node *pCurrent = this;
                     pParent && pParent->mpLeft == pCurrent;
                     pCurrent = pParent, pParent = pCurrent->mpParent);
                return pParent;
            } else { // find a max key in a left subtree
                Node *pPrev = mpLeft;
                while (pPrev->mpRight)
                    pPrev = pPrev->mpRight;
                return pPrev;
            }
        }

        size_t size() {
            return 1 + (mpLeft ? mpLeft->size() : 0) +
                (mpRight ? mpRight->size() : 0);
        }

        size_t height() {
            return 1 + std::max(mpLeft ? mpLeft->height() : 0,
                mpRight ? mpRight->height() : 0);
        }
    };

    Node *mpRoot;

public:
    avl()
        : mpRoot(nullptr)
    {}

    ~avl() {
        clear();
    }

    /**
     * Remove all key-value pairs from a container
     **/
    void clear() {
        clear(mpRoot);
        mpRoot = nullptr;
    }

    /**
     * Find a mapped value for a given key. If the value is not found,
     * find will return a default value for the Value type.
     * TODO: Should insert a (key, default value) pair if not found?
     **/
    value_type find(const Key& key) {
        Node *pNode = find(mpRoot, key);
        if (pNode == nullptr)
            return std::make_pair(key, Value());
        return pNode->mData;
    }

    /**
     * Returns a reference to a key-value pair.
     * TODO: implement this
     **/
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
    value_type& operator[](const Key& key) {
        throw std::logic_error("not implemented");
    }
#pragma clang diagnostic pop

    /**
     * Check if a container contains a value for a given key
     **/
    bool contains(const Key& key) {
        return find(mpRoot, key) != nullptr;
    }

    /**
     * Find k-th minimal element in a collection
     **/
    value_type select(size_t k) {
        Node *pNode = select(mpRoot, k);
        if (pNode == nullptr)
            throw std::out_of_range("avl::select");
        return pNode->mData;
    }

    /**
     * Insert a key-value pair to a container
     **/
    void insert(Key key, Value value) {
        Node *pNew = nullptr;
        mpRoot = insert(mpRoot, nullptr, key, value, &pNew);
        balance(pNew);
    }

    /**
     * Remove a key-value pair from a container
     **/
    void remove(Key key) {
        mpRoot = remove(mpRoot, key);
    }

    /**
     * Returns count of elements in a container
     **/
    size_t size() {
        return size(mpRoot);
    }

    /**
     * Returns tree height
     **/
    size_t height() {
        return height(mpRoot);
    }

protected:
    void clear(Node *pNode) {
        if (pNode == nullptr)
            return;
        clear(pNode->mpLeft);
        clear(pNode->mpRight);
        delete pNode;
    }

    Node *find(Node *pNode, const Key& key) {
        if (pNode == nullptr)
            return pNode;
        else if (key < pNode->mData.first)
            return find(pNode->mpLeft, key);
        else if (pNode->mData.first < key)
            return find(pNode->mpRight, key);
        return pNode;
    }

    Node *find_min(Node *pNode) {
        if (pNode == nullptr)
            return pNode;
        else if (pNode->mpLeft == nullptr)
            return pNode;
        else
            return find_min(pNode->mpLeft);
    }

    Node *find_max(Node *pNode) {
        if (pNode == nullptr)
            return pNode;
        else if (pNode->mpRight == nullptr)
            return pNode;
        else
            return find_max(pNode->mpRight);
    }

    Node *select(Node *pNode, size_t k) {
        if (pNode == nullptr)
            return nullptr;
        size_t sz = pNode->mpLeft ? pNode->mpLeft->size() : 0;
        if (k < sz)
            return select(pNode->mpLeft, k);
        else if (sz < k)
            return select(pNode->mpRight, k-sz-1);
        else
            return pNode;
    }

    Node *insert(Node *pNode, Node *pParent, Key key, Value value, Node **ppNew) {
        if (pNode == nullptr)
            return *ppNew = new Node(key, value, pParent);
        else if (key < pNode->mData.first)
            pNode->mpLeft = insert(pNode->mpLeft, pNode, key, value, ppNew);
        else if (pNode->mData.first < key)
            pNode->mpRight = insert(pNode->mpRight, pNode, key, value, ppNew);
        else
            pNode->mData.second = value;
        return pNode;
    }

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
    Node *remove(Node *pNode, Key key) {
        // TODO: fix this
        throw std::logic_error("not implemented");
    }
#pragma clang diagnostic pop

    Node *rotate_left(Node *x) {
        Node *z = x->mpRight;
        Node *t23 = z->mpLeft;

        z->mpParent = x->mpParent;
        x->mpRight = t23;
        if (t23)
            t23->mpParent = x;
        z->mpLeft = x;
        x->mpParent = z;

        if (z->mBalance == 0) {
            x->mBalance = +1;
            z->mBalance = -1;
        } else {
            x->mBalance = 0;
            z->mBalance = 0;
        }

        return z;
    }

    Node *rotate_right(Node *x) {
        Node *z = x->mpLeft;
        Node *t23 = z->mpRight;

        z->mpParent = x->mpParent;
        x->mpLeft = t23;
        if (t23)
            t23->mpParent = x;
        z->mpRight = x;
        x->mpParent = z;

        if (z->mBalance == 0) {
            x->mBalance = -1;
            z->mBalance = +1;
        } else {
            x->mBalance = 0;
            z->mBalance = 0;
        }

        return z;
    }

    Node *rotate_left_right(Node *x) {
        Node *z = x->mpLeft;
        Node *y = z->mpRight;
        Node *t2 = y->mpLeft;
        Node *t3 = y->mpRight;

        y->mpParent = x->mpParent;

        y->mpRight = x;
        x->mpParent = y;
        x->mpLeft = t3;
        if (t3)
            t3->mpParent = x;

        y->mpLeft = z;
        z->mpParent = y;
        z->mpRight = t2;
        if (t2)
            t2->mpParent = z;

        if (y->mBalance < 0) {
            x->mBalance = +1;
            z->mBalance = 0;
        } else if (y->mBalance == 0) {
            x->mBalance = 0;
            z->mBalance = 0;
        } else {
            x->mBalance = 0;
            z->mBalance = -1;
        }
        y->mBalance = 0;

        return y;
    }

    Node *rotate_right_left(Node *x) {
        Node *z = x->mpRight;
        Node *y = z->mpLeft;
        Node *t2 = y->mpLeft;
        Node *t3 = y->mpRight;

        y->mpParent = x->mpParent;

        y->mpLeft = x;
        x->mpParent = y;
        x->mpRight = t2;
        if (t2)
            t2->mpParent = x;

        y->mpRight = z;
        z->mpParent = y;
        z->mpLeft = t3;
        if (t3)
            t3->mpParent = z;

        if (y->mBalance > 0) {
            x->mBalance = -1;
            z->mBalance = 0;
        } else if (y->mBalance == 0) {
            x->mBalance = 0;
            z->mBalance = 0;
        } else {
            x->mBalance = 0;
            z->mBalance = +1;
        }
        y->mBalance = 0;

        return y;
    }

    void balance(Node *z) {
        if (z == nullptr)
            return;
        for (Node *x = z->mpParent; x != nullptr; x = z->mpParent) {
            Node *n, *g;
            if (z == x->mpRight){
                if (x->mBalance > 0) {
                    g = x->mpParent;
                    if (z->mBalance < 0)
                        n = rotate_right_left(x);
                    else
                        n = rotate_left(x);
                } else {
                    if (x->mBalance < 0) {
                        x->mBalance = 0;
                        break;
                    }
                    x->mBalance = +1;
                    z = x;
                    continue;
                }
            } else {
                if (x->mBalance < 0) {
                    g = x->mpParent;
                    if (z->mBalance > 0)
                        n = rotate_left_right(x);
                    else
                        n = rotate_right(x);
                } else {
                    if (x->mBalance > 0) {
                        x->mBalance = 0;
                        break;
                    }
                    x->mBalance = -1;
                    z = x;
                    continue;
                }
            }

            n->mpParent = g;
            if (g != nullptr) {
                if (g->mpLeft == x)
                    g->mpLeft = n;
                else
                    g->mpRight = n;
                break;
            } else {
                mpRoot = n;
                break;
            }
        }
    }

    size_t size(Node *pNode) {
        if (pNode == nullptr)
            return 0;
        else
            return 1 + size(pNode->mpLeft) + size(pNode->mpRight);
    }

    size_t height(Node *pNode) {
        if (pNode == nullptr)
            return 0;
        else
            return 1 + std::max(height(pNode->mpLeft), height(pNode->mpRight));
    }

    // ADDITIONAL METHODS
public:
    /**
     * In order traversal (min to max)
     **/
    void in_order(std::vector<value_type>& out) {
        for (Node *pCurr = find_min(mpRoot); pCurr != nullptr; pCurr = pCurr->next())
            out.push_back(pCurr->mData);
    }

    /**
     * In order reverse traversal (max to min)
     **/
    void in_order_reverse(std::vector<value_type>& out) {
        for (Node *pCurr = find_max(mpRoot); pCurr != nullptr; pCurr = pCurr->prev())
            out.push_back(pCurr->mData);
    }

    /**
     * Print tree contents
     **/
    void print(std::ostream& out) {
        print(out, mpRoot, 0);
    }

    bool check_links_valid() {
        if (mpRoot == nullptr)
            return true;
        if (mpRoot->mpParent)
            return false;
        return check_links_valid(mpRoot);
    }

    bool check_is_bst() {
        if (mpRoot == nullptr)
            return true;
        Node *pMin = find_min(mpRoot->mpLeft);
        Node *pMax = find_max(mpRoot->mpRight);
        pMin = pMin ? pMin : mpRoot;
        pMax = pMax ? pMax : mpRoot;
        return check_is_bst(mpRoot, pMin->mData.first, pMax->mData.first);
    }

    bool check_is_avl() {
        return check_is_avl(mpRoot);
    }

protected:
    void print(std::ostream& out, Node *pNode, int depth) {
        if (pNode)
            print(out, pNode->mpRight, depth+1);
        print_node(out, pNode, depth);
        if (pNode)
            print(out, pNode->mpLeft, depth+1);
    }

    void print_node(std::ostream& out, Node *pNode, int depth) {
        for (int i = 0; i < depth; ++i)
            out << "    ";
        if (pNode) {
            size_t hl = pNode->mpLeft ? pNode->mpLeft->height() : 0;
            size_t hr = pNode->mpRight ? pNode->mpRight->height() : 0;
            out << pNode->mData.first << " (" 
                    << int(pNode->mBalance) << ", "
                    << int(hr-hl)
                << ")" << std::endl;
        }
        else
            out << '*' << std::endl;
    }

    bool check_links_valid(Node *pNode) {
        if (pNode == nullptr)
            return true;
        if (pNode->mpLeft && pNode->mpLeft->mpParent != pNode) {
            return false;
        }
        if (pNode->mpRight && pNode->mpRight->mpParent != pNode) {
            return false;
        }
        return check_links_valid(pNode->mpLeft) && check_links_valid(pNode->mpRight);
    }

    bool check_is_bst(Node *pNode, const Key& min, const Key& max) {
        if (pNode == nullptr)
            return true;
        if (pNode->mData.first < min || max < pNode->mData.first)
            return false;
        return check_is_bst(pNode->mpLeft, min, pNode->mData.first) ||
            check_is_bst(pNode->mpRight, pNode->mData.first, max);
    }

    bool check_is_avl(Node *pNode) {
        if (pNode == nullptr)
            return true;
        size_t hl = pNode->mpLeft ? pNode->mpLeft->height() : 0;
        size_t hr = pNode->mpRight ? pNode->mpRight->height() : 0;
        bool flag = pNode->mBalance == char(hr - hl);
        return flag && check_is_avl(pNode->mpLeft) && check_is_avl(pNode->mpRight);
    }
};

} // namespace algs::tree::bst

