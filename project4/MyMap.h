// MyMap.h

// Skeleton for the MyMap class template.  You must implement the first six
// member functions.



template<typename KeyType, typename ValueType>
class MyMap
{
public:
    MyMap() {
        m_numNodes = 0;
        head = nullptr;
    }
    
    ~MyMap() {
        clear();
    }
    void clear() {
        FreeTree(head);
        head = nullptr;
    }
    
    int size() const {
        return m_numNodes;
    }
    void associate(const KeyType& key, const ValueType& value) {
        if (head == nullptr) {
            head = createNode(key, value);
            m_numNodes++;
            return;
        }
        node *cur = head;
        for (;;) {
            if (key == cur->key) {
                cur->value = value;
                return;
            }
            if (key < cur->key) {
                if (cur->left != nullptr)
                    cur = cur->left;
                else {
                    cur->left = createNode(key, value);
                    m_numNodes++;
                    return;
                }
            } else if (key > cur->key) {
                if (cur->right != nullptr)
                    cur = cur->right;
                else {
                    cur->right = createNode(key, value);
                    m_numNodes++;
                    return;
                }
            }
        }
        
    }

	  // for a map that can't be modified, return a pointer to const ValueType
    const ValueType* find(const KeyType& key) const {
        return searchFor(key, head);
    }
        
     
	  // for a modifiable map, return a pointer to modifiable ValueType
	ValueType* find(const KeyType& key)
	{
		return const_cast<ValueType*>(const_cast<const MyMap*>(this)->find(key));
	}

	  // C++11 syntax for preventing copying and assignment
	MyMap(const MyMap&) = delete;
	MyMap& operator=(const MyMap&) = delete;

private:
    
    struct node {
        KeyType key;
        ValueType value;
        node *left;
        node *right;
    };
    
    node *head;
    int m_numNodes;
    
    node* createNode(const KeyType& key, const ValueType& value) {
        node *newNode = new node;
        newNode->key = key;
        newNode->value = value;
        newNode->left = nullptr;
        newNode->right = nullptr;
        return newNode;
    }
                   
    ValueType* searchFor(const KeyType& key, node *ptr) const {
        
        if (ptr == nullptr)
            return nullptr;
        
        else if (key == ptr->key) {
            return &ptr->value;
        }
        else if (key < ptr->key)
            return searchFor(key, ptr->left);
        else {
            return searchFor(key, ptr->right);
        }
            
    }
    
    void FreeTree(node *cur) {
        if (cur == nullptr)
            return;
        FreeTree(cur->left);
        FreeTree(cur->right);
        delete cur;
    }
    
};
