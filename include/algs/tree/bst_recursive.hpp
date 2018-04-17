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
     * Insert a key-value pair to a container
     **/
    void insert(Key key, Value value) {
        mpRoot = insert(mpRoot, nullptr, key, value);
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

    Node *insert(Node *pNode, Node *pParent, Key key, Value value) {
        if (pNode == nullptr)
            return new Node(key, value, pParent);
        else if (key < pNode->mData.first)
            pNode->mpLeft = insert(pNode->mpLeft, pNode, key, value);
        else if (pNode->mData.first < key)
            pNode->mpRight = insert(pNode->mpRight, pNode, key, value);
        else
            pNode->mData.second = value;
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

    size_t size(Node *pNode) {
        if (pNode == nullptr)
            return 0;
        else
            return 1 + size(pNode->mpLeft) + size(pNode->mpRight);
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
};

} // namespace algs::tree::bst

