#include <stdexcept>

namespace algs::tree::bst {

// TODO: iterators instead in_order/in_order_reverse methods
// TODO: select k-th method
// TODO: find and operator[] return a reference to a key-value pair

/**
 * Recursive implementation of a binary search tree.
 * NOTE: To simplify code, nearly everything is passed by value
 **/
template<typename Key, typename Value>
class bst_recursive {
public:
    typedef Key key_type;
    typedef Value mapped_type;
    typedef std::pair<Key, Value> value_type;

private:
    class Node {
    public:
        std::pair<Key, Value> mData;
        Node *mpParent, *mpLeft, *mpRight;

        Node(Key key, Value value, Node *pParent)
            : mData(std::make_pair(key, value)), mpParent(pParent),
              mpLeft(nullptr), mpRight(nullptr)
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
    bst_recursive()
        : mpRoot(nullptr)
    {}

    ~bst_recursive() {
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
        if (k >= size())
            throw std::out_of_range("bst_recursive::select");
        Node *pNode = select(mpRoot, k);
        return pNode->mData;
    }

    /**
     * Partition a collection around a k-th minimal element
     **/
    void partition(size_t k) {
        if (k >= size())
            throw std::out_of_range("bst_recursive::partition");
        mpRoot = partition(mpRoot, k);
    }

    /**
     * Insert a key-value pair to a container
     **/
    value_type insert(Key key, Value value) {
        Node *pNew = nullptr;
        mpRoot = insert(mpRoot, nullptr, key, value, &pNew);
        return pNew->mData;
    }

    /**
     * Insert a key-value pair to a container into a root node
     **/
    value_type insert_root(Key key, Value value) {
        mpRoot = insert_root(mpRoot, nullptr, key, value);
        return mpRoot->mData;
    }

    /**
     * Insert a key-value pair to a container a splay-way
     **/
    value_type insert_splay(Key key, Value value) {
        Node *pNew = nullptr;
        mpRoot = insert(mpRoot, nullptr, key, value, &pNew);
        splay(pNew);
        return mpRoot->mData;
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

    Node *partition(Node *pNode, size_t k) {
        if (pNode == nullptr)
            return nullptr;
        size_t sz = pNode->mpLeft ? pNode->mpLeft->size() : 0;
        if (k < sz) {
            pNode->mpLeft = partition(pNode->mpLeft, k);
            pNode = rotate_right(pNode);
        } else if (sz < k) {
            pNode->mpRight = partition(pNode->mpRight, k-sz-1);
            pNode = rotate_left(pNode);
        }
        return pNode;
    }

    void splay(Node *x) {
        while (x) {
            Node *y = x->mpParent;
            if (y == nullptr) // x is a root node, do nothing
                break;

            Node *z = y->mpParent;
            if (z == nullptr) { // y is a root node, rotate x to root
                if (y->mpLeft == x)
                    mpRoot = rotate_right(mpRoot);
                else
                    mpRoot = rotate_left(mpRoot);
                break;
            }

            // y is not root, two rotations required
            Node *n; // new x-y-z hierachy's root
            Node *g = z->mpParent; // g is z's parent

            if (z->mpLeft == y) {
                if (y->mpLeft == x) {
                    n = rotate_right(z); // rotate first y-z
                    n = rotate_right(n); // then x-y
                } else {
                    z->mpLeft = rotate_left(z->mpLeft); // rotate x-y
                    n = rotate_right(z); // then new x-z
                }
            } else {
                if (y->mpLeft == x) {
                    z->mpRight = rotate_right(z->mpRight); // rotate x-y
                    n = rotate_left(z); // then new x-z
                } else {
                    n = rotate_left(z); // rotate y-z
                    n = rotate_left(n); // then x-y
                }
            }

            n->mpParent = g; // adapt parent link
            if (g) { // relink parent to new node
                if (g->mpLeft == z)
                    g->mpLeft = n;
                else
                    g->mpRight = n;
                x = n;
                continue;
            } else { // g is empty, so z was a root node
                mpRoot = n;
                break;
            }
        }
    }

    Node *insert(Node *pNode, Node *pParent, Key key, Value value, Node **ppNew) {
        if (pNode == nullptr)
            return *ppNew = new Node(key, value, pParent);
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

    Node *insert_root(Node *pNode, Node *pParent, Key key, Value value) {
        if (pNode == nullptr) {
            pNode = new Node(key, value, pParent);
        } else if (key < pNode->mData.first) {
            pNode->mpLeft = insert_root(pNode->mpLeft, pNode, key, value);
            pNode = rotate_right(pNode);
        } else if (pNode->mData.first < key) {
            pNode->mpRight = insert_root(pNode->mpRight, pNode, key, value);
            pNode = rotate_left(pNode);
        } else {
            pNode->mData.second = value;
        }
        return pNode;
    }

    /**
     * Hibbard deletion: find minimal node in a right subtree
     * and replace with a removed one
     **/
    Node *remove(Node *pNode, Key key) {
        if (pNode == nullptr)
            return pNode;
        else if (key < pNode->mData.first) {
            pNode->mpLeft = remove(pNode->mpLeft, key);
            return pNode;
        } else if (pNode->mData.first < key) {
            pNode->mpRight = remove(pNode->mpRight, key);
            return pNode;
        } else {
            Node *pNewNode = nullptr;
            if (pNode->mpRight == nullptr) {
                pNewNode = pNode->mpLeft;
            } else if (pNode->mpLeft == nullptr) {
                pNewNode = pNode->mpRight;
            } else {
                pNewNode = find_min(pNode->mpRight);
                if (pNewNode != pNode->mpRight) {
                    pNewNode->mpParent->mpLeft = pNewNode->mpRight;
                    if (pNewNode->mpRight)
                        pNewNode->mpRight->mpParent = pNewNode->mpParent;
                    pNewNode->mpRight = pNode->mpRight;
                    pNode->mpRight->mpParent = pNewNode;
                }
                pNewNode->mpLeft = pNode->mpLeft;
                pNode->mpLeft->mpParent = pNewNode;
            }
            if (pNewNode)
                pNewNode->mpParent = pNode->mpParent;
            delete pNode;
            return pNewNode;
        }
    }

    Node *rotate_left(Node *pNode) {
        Node *pNewNode = pNode->mpRight;
        pNewNode->mpParent = pNode->mpParent;
        pNode->mpRight = pNewNode->mpLeft;
        if (pNode->mpRight)
            pNode->mpRight->mpParent = pNode;
        pNewNode->mpLeft = pNode;
        if (pNewNode->mpLeft)
            pNewNode->mpLeft->mpParent = pNewNode;
        return pNewNode;
    }

    Node *rotate_right(Node *pNode) {
        Node *pNewNode = pNode->mpLeft;
        pNewNode->mpParent = pNode->mpParent;
        pNode->mpLeft = pNewNode->mpRight;
        if (pNode->mpLeft)
            pNode->mpLeft->mpParent = pNode;
        pNewNode->mpRight = pNode;
        if (pNewNode->mpRight)
            pNewNode->mpRight->mpParent = pNewNode;
        return pNewNode;
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

    bool check_is_partitioned(size_t k) {
        return select(mpRoot, k) == mpRoot;
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
        if (pNode)
            out << pNode->mData.first << std::endl;
        else
            out << '*' << std::endl;
    }

    bool check_links_valid(Node *pNode) {
        if (pNode == nullptr)
            return true;
        if (pNode->mpLeft)
            if (pNode->mpLeft->mpParent != pNode || !check_links_valid(pNode->mpLeft))
                return false;
        if (pNode->mpRight)
            if (pNode->mpRight->mpParent != pNode || !check_links_valid(pNode->mpRight))
                return false;
        return true;
    }

    bool check_is_bst(Node *pNode, const Key& min, const Key& max) {
        if (pNode == nullptr)
            return true;
        if (pNode->mData.first < min || max < pNode->mData.first)
            return false;
        return check_is_bst(pNode->mpLeft, min, pNode->mData.first) ||
            check_is_bst(pNode->mpRight, pNode->mData.first, max);
    }
};

} // namespace algs::tree::bst

