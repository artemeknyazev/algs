namespace algs::tree::bst {

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
        Node *mParent, *mLeft, *mRight;

        Node(Key key, Value value, Node *parent)
            : mData(std::make_pair(key, value)), mParent(parent),
              mLeft(nullptr), mRight(nullptr)
        {}
    };

    Node *mRoot;

public:
    bst_recursive()
        : mRoot(nullptr)
    {}

    ~bst_recursive() {
        clear();
    }

    /**
     * Remove all key-value pairs from a container
     **/
    void clear() {
        clear(mRoot);
        mRoot = nullptr;
    }

    /**
     * Find a mapped value for a given key. If a value is not found,
     * find will return default value for Value type.
     * NOTE: Maybe should insert (key, default value) pair if not found?
     * TODO: Should return reference to allow value modification
     **/
    value_type find(const Key& key) {
        Node *pNode = find(mRoot, key);
        if (pNode == nullptr)
            return std::make_pair(key, Value());
        return pNode->mData;
    }

    /**
     * Finds minimal element in a container. If a container is empty,
     * returns empty key-value pair
     * TODO: what to do if a container is empty? Throw, assert, return
     *       fake value (referncing problem?)
     * TODO: return a reference
     **/
    value_type find_min() {
        Node *pNode = find_min(mRoot);
        if (pNode == nullptr)
            return std::make_pair(Key(), Value());
        return pNode->mData;
    }

    /**
     * Finds maximal element in a container. If a container is empty,
     * returns empty key-value pair
     * TODO: same as find_min
     **/
    value_type find_max() {
        Node *pNode = find_max(mRoot);
        if (pNode == nullptr)
            return std::make_pair(Key(), Value());
        return pNode->mData;
    }

    /**
     * Check if a container contains a value for a given key
     **/
    bool contains(const Key& key) {
        return find(mRoot, key) != nullptr;
    }

    /**
     * Insert a key-value pair to a container
     **/
    void insert(Key key, Value value) {
        mRoot = insert(mRoot, nullptr, key, value);
    }

    /**
     * Returns count of elements in a container
     **/
    size_t size() {
        return size(mRoot);
    }

protected:
    void clear(Node *pNode) {
        if (pNode == nullptr)
            return;
        clear(pNode->mLeft);
        clear(pNode->mRight);
        delete pNode;
    }

    Node *find(Node *pNode, const Key& key) {
        if (pNode == nullptr)
            return pNode;
        else if (key < pNode->mData.first)
            return find(pNode->mLeft, key);
        else if (pNode->mData.first < key)
            return find(pNode->mRight, key);
        return pNode;
    }

    Node *find_min(Node *pNode) {
        if (pNode == nullptr)
            return pNode;
        else if (pNode->mLeft == nullptr)
            return pNode;
        else
            return find_min(pNode->mLeft);
    }

    Node *find_max(Node *pNode) {
        if (pNode == nullptr)
            return pNode;
        else if (pNode->mRight == nullptr)
            return pNode;
        else
            return find_max(pNode->mRight);
    }

    Node *insert(Node *pNode, Node *pParent, Key key, Value value) {
        if (pNode == nullptr)
            return new Node(key, value, pParent);
        else if (key < pNode->mData.first)
            pNode->mLeft = insert(pNode->mLeft, pNode, key, value);
        else if (pNode->mData.first < key)
            pNode->mRight = insert(pNode->mRight, pNode, key, value);
        else
            pNode->mData.second = value;
        return pNode;
    }

    size_t size(Node *pNode) {
        if (pNode == nullptr)
            return 0;
        else
            return 1 + size(pNode->mLeft) + size(pNode->mRight);
    }
};

} // namespace algs::tree::bst

