#include <stdexcept>

namespace algs::tree::avl {

// TODO: removal&rebalancing after removal
// TODO: iterators instead in_order/in_order_reverse methods
// TODO: find and operator[] return a reference to a key-value pair

template<typename Key, typename Value>
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
    typedef Node<Key, Value> node_type;

public:
    avl()
        : mpRoot(nullptr)
    {}

    virtual ~avl() {
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
        node_type *pNode = find(root(), key);
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
        return find(root(), key) != nullptr;
    }

    /**
     * Find k-th minimal element in a collection
     **/
    value_type select(size_t k) {
        node_type *pNode = select(root(), k);
        if (pNode == nullptr)
            throw std::out_of_range("avl::select");
        return pNode->mData;
    }

    /**
     * Insert a key-value pair to a container
     **/
    void insert(Key key, Value value) {
        node_type *pNew = nullptr;
        mpRoot = insert(root(), nullptr, key, value, &pNew);
        balance(pNew);
    }

    /**
     * Remove a key-value pair from a container
     **/
    void remove(Key key) {
        mpRoot = remove(root(), key);
    }

    /**
     * Returns count of elements in a container
     **/
    size_t size() {
        return root() ? root()->size() : 0;
    }

    /**
     * Returns tree height
     **/
    size_t height() {
        return root() ? root()->height() : 0;
    }

protected:
    void clear(node_type *pNode) {
        if (pNode == nullptr)
            return;
        clear(pNode->mpLeft);
        clear(pNode->mpRight);
        delete pNode;
    }

    node_type *find(node_type *pNode, const Key& key) {
        if (pNode == nullptr)
            return pNode;
        else if (key < pNode->mData.first)
            return find(pNode->mpLeft, key);
        else if (pNode->mData.first < key)
            return find(pNode->mpRight, key);
        return pNode;
    }

    node_type *find_min(node_type *pNode) {
        if (pNode == nullptr)
            return pNode;
        else if (pNode->mpLeft == nullptr)
            return pNode;
        else
            return find_min(pNode->mpLeft);
    }

    node_type *find_max(node_type *pNode) {
        if (pNode == nullptr)
            return pNode;
        else if (pNode->mpRight == nullptr)
            return pNode;
        else
            return find_max(pNode->mpRight);
    }

    node_type *select(node_type *pNode, size_t k) {
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

    node_type *insert(node_type *pNode, node_type *pParent, Key key, Value value, node_type **ppNew) {
        if (pNode == nullptr)
            return *ppNew = new node_type(key, value, pParent);
        else if (key < pNode->mData.first)
            pNode->mpLeft = insert(pNode->mpLeft, pNode, key, value, ppNew);
        else if (pNode->mData.first < key)
            pNode->mpRight = insert(pNode->mpRight, pNode, key, value, ppNew);
        else {
            pNode->mData.second = value;
            *ppNew = pNode;
        }
        return pNode;
    }

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
    node_type *remove(node_type *pNode, Key key) {
        // TODO: fix this
        throw std::logic_error("not implemented");
    }
#pragma clang diagnostic pop

    node_type *rotate_left(node_type *x) {
        node_type *z = x->mpRight;
        node_type *t23 = z->mpLeft;

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

    node_type *rotate_right(node_type *x) {
        node_type *z = x->mpLeft;
        node_type *t23 = z->mpRight;

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

    node_type *rotate_left_right(node_type *x) {
        node_type *z = x->mpLeft;
        node_type *y = z->mpRight;
        node_type *t2 = y->mpLeft;
        node_type *t3 = y->mpRight;

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

    node_type *rotate_right_left(node_type *x) {
        node_type *z = x->mpRight;
        node_type *y = z->mpLeft;
        node_type *t2 = y->mpLeft;
        node_type *t3 = y->mpRight;

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

    void balance(node_type *z) {
        if (z == nullptr)
            return;
        for (node_type *x = z->mpParent; x != nullptr; x = z->mpParent) {
            node_type *n, *g;
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

protected:
    node_type *root() {
        return mpRoot;
    }

private:
    node_type *mpRoot;
};

template<typename Key, typename Value>
class avl_debug : public avl<Key, Value> {
public:
    using node_type = typename avl<Key, Value>::node_type;
    using value_type = typename avl<Key, Value>::value_type;

    /**
     * In order traversal (min to max)
     **/
    void in_order(std::vector<value_type>& out) {
        for (node_type *pCurr = this->find_min(this->root()); pCurr != nullptr; pCurr = pCurr->next())
            out.push_back(pCurr->mData);
    }

    /**
     * In order reverse traversal (max to min)
     **/
    void in_order_reverse(std::vector<value_type>& out) {
        for (node_type *pCurr = this->find_max(this->root()); pCurr != nullptr; pCurr = pCurr->prev())
            out.push_back(pCurr->mData);
    }

    /**
     * Print tree contents
     **/
    void print(std::ostream& out) {
        print(out, this->root(), 0);
    }

private:
    void print(std::ostream& out, node_type *pNode, int depth) {
        if (pNode)
            print(out, pNode->mpRight, depth+1);
        print_node(out, pNode, depth);
        if (pNode)
            print(out, pNode->mpLeft, depth+1);
    }

    void print_node(std::ostream& out, node_type *pNode, int depth) {
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
};

template<typename Key, typename Value>
class avl_test : public avl_debug<Key, Value> {
public:
    using node_type = typename avl_debug<Key, Value>::node_type;
    using value_type = typename avl_debug<Key, Value>::value_type;

    bool check_links_valid() {
        auto pRoot = this->root();
        if (pRoot == nullptr)
            return true;
        if (pRoot->mpParent)
            return false;
        return check_links_valid(pRoot);
    }

    bool check_is_bst() {
        auto pRoot = this->root();
        if (pRoot == nullptr)
            return true;
        node_type *pMin = this->find_min(pRoot->mpLeft);
        node_type *pMax = this->find_max(pRoot->mpRight);
        pMin = pMin ? pMin : pRoot;
        pMax = pMax ? pMax : pRoot;
        return check_is_bst(pRoot, pMin->mData.first, pMax->mData.first);
    }

    bool check_is_avl() {
        return check_is_avl(this->root());
    }

private:
    bool check_links_valid(node_type *pNode) {
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

    bool check_is_bst(node_type *pNode, const Key& min, const Key& max) {
        if (pNode == nullptr)
            return true;
        if (pNode->mData.first < min || max < pNode->mData.first)
            return false;
        return check_is_bst(pNode->mpLeft, min, pNode->mData.first) ||
            check_is_bst(pNode->mpRight, pNode->mData.first, max);
    }

    bool check_is_avl(node_type *pNode) {
        if (pNode == nullptr)
            return true;
        size_t hl = pNode->mpLeft ? pNode->mpLeft->height() : 0;
        size_t hr = pNode->mpRight ? pNode->mpRight->height() : 0;
        bool flag = pNode->mBalance == char(hr - hl);
        return flag && check_is_avl(pNode->mpLeft) && check_is_avl(pNode->mpRight);
    }
};

} // namespace algs::tree::bst

