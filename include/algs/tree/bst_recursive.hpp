#include <stdexcept>

namespace algs::tree::bst {

// TODO: iterators instead in_order/in_order_reverse methods
// TODO: find and operator[] return a reference to a key-value pair

template<typename Key, typename Value>
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
    typedef Node<Key, Value> node_type;

public:
    bst_recursive()
        : mpRoot(nullptr)
    {}

    virtual ~bst_recursive() {
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
        if (k >= size())
            throw std::out_of_range("bst_recursive::select");
        node_type *pNode = select(root(), k);
        return pNode->mData;
    }

    /**
     * Partition a collection around a k-th minimal element
     **/
    void partition(size_t k) {
        if (k >= size())
            throw std::out_of_range("bst_recursive::partition");
        mpRoot = partition(root(), k);
    }

    /**
     * Insert a key-value pair to a container
     **/
    value_type insert(Key key, Value value) {
        node_type *pNew = nullptr;
        mpRoot = insert(root(), nullptr, key, value, &pNew);
        return pNew->mData;
    }

    /**
     * Insert a key-value pair to a container into a root node
     **/
    value_type insert_root(Key key, Value value) {
        mpRoot = insert_root(root(), nullptr, key, value);
        return root()->mData;
    }

    /**
     * Insert a key-value pair to a container a splay-way
     **/
    value_type insert_splay(Key key, Value value) {
        node_type *pNew = nullptr;
        mpRoot = insert(root(), nullptr, key, value, &pNew);
        splay(pNew);
        return root()->mData;
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
        return root() ? height(root()) : 0;
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

    node_type *partition(node_type *pNode, size_t k) {
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

    void splay(node_type *x) {
        while (x) {
            node_type *y = x->mpParent;
            if (y == nullptr) // x is a root node, do nothing
                break;

            node_type *z = y->mpParent;
            if (z == nullptr) { // y is a root node, rotate x to root
                if (y->mpLeft == x)
                    mpRoot = rotate_right(mpRoot);
                else
                    mpRoot = rotate_left(mpRoot);
                break;
            }

            // y is not root, two rotations required
            node_type *n; // new x-y-z hierachy's root
            node_type *g = z->mpParent; // g is z's parent

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

    node_type *insert_root(node_type *pNode, node_type *pParent, Key key, Value value) {
        if (pNode == nullptr) {
            pNode = new node_type(key, value, pParent);
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
    node_type *remove(node_type *pNode, Key key) {
        if (pNode == nullptr)
            return pNode;
        else if (key < pNode->mData.first) {
            pNode->mpLeft = remove(pNode->mpLeft, key);
            return pNode;
        } else if (pNode->mData.first < key) {
            pNode->mpRight = remove(pNode->mpRight, key);
            return pNode;
        } else {
            node_type *pNewNode = nullptr;
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

    node_type *rotate_left(node_type *pNode) {
        node_type *pNewNode = pNode->mpRight;
        pNewNode->mpParent = pNode->mpParent;
        pNode->mpRight = pNewNode->mpLeft;
        if (pNode->mpRight)
            pNode->mpRight->mpParent = pNode;
        pNewNode->mpLeft = pNode;
        if (pNewNode->mpLeft)
            pNewNode->mpLeft->mpParent = pNewNode;
        return pNewNode;
    }

    node_type *rotate_right(node_type *pNode) {
        node_type *pNewNode = pNode->mpLeft;
        pNewNode->mpParent = pNode->mpParent;
        pNode->mpLeft = pNewNode->mpRight;
        if (pNode->mpLeft)
            pNode->mpLeft->mpParent = pNode;
        pNewNode->mpRight = pNode;
        if (pNewNode->mpRight)
            pNewNode->mpRight->mpParent = pNewNode;
        return pNewNode;
    }

protected:
    node_type *root() {
        return mpRoot;
    }

private:
    node_type *mpRoot;
};

template<typename Key, typename Value>
class bst_recursive_debug : public bst_recursive<Key, Value> {
public:
    using value_type = typename bst_recursive<Key, Value>::value_type;
    using node_type = typename bst_recursive<Key, Value>::node_type;

    /**
     * In order traversal (min to max)
     **/
    void in_order(std::vector<value_type>& out) {
        for (auto *pCurr = this->find_min(this->root()); pCurr != nullptr; pCurr = pCurr->next())
            out.push_back(pCurr->mData);
    }

    /**
     * In order reverse traversal (max to min)
     **/
    void in_order_reverse(std::vector<value_type>& out) {
        for (auto *pCurr = this->find_max(this->root()); pCurr != nullptr; pCurr = pCurr->prev())
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
        if (pNode)
            out << pNode->mData.first << std::endl;
        else
            out << '*' << std::endl;
    }
}; // bst_recursive_debug

template<typename Key, typename Value>
class bst_recursive_test : public bst_recursive_debug<Key, Value> {
public:
    using value_type = typename bst_recursive_debug<Key, Value>::value_type;
    using node_type = typename bst_recursive_debug<Key, Value>::node_type;

    bool check_links_valid() {
        auto *pRoot = this->root();
        if (pRoot == nullptr)
            return true;
        if (pRoot->mpParent)
            return false;
        return check_links_valid(pRoot);
    }

    bool check_is_bst() {
        auto *pRoot = this->root();
        if (pRoot == nullptr)
            return true;
        auto *pMin = this->find_min(pRoot->mpLeft);
        auto *pMax = this->find_max(pRoot->mpRight);
        pMin = pMin ? pMin : pRoot;
        pMax = pMax ? pMax : pRoot;
        return check_is_bst(pRoot, pMin->mData.first, pMax->mData.first);
    }

    bool check_is_partitioned(size_t k) {
        auto *pRoot = this->root();
        return this->select(pRoot, k) == pRoot;
    }

private:
    bool check_links_valid(node_type *pNode) {
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

    bool check_is_bst(node_type *pNode, const Key& min, const Key& max) {
        if (pNode == nullptr)
            return true;
        if (pNode->mData.first < min || max < pNode->mData.first)
            return false;
        return check_is_bst(pNode->mpLeft, min, pNode->mData.first) ||
            check_is_bst(pNode->mpRight, pNode->mData.first, max);
    }
}; // bst_recursive_test

} // namespace algs::tree::bst

