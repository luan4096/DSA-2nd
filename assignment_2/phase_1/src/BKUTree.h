#include <iostream>
#include <vector>
#include <list>

using namespace std;
#define COUNT 10

template <class K, class V>
class BKUTree {
public:
    class AVLTree;
    class SplayTree;

    class Entry {
    public:
        K key;
        V value;

        Entry(K key, V value) : key(key), value(value) {}
    };

//private:
public:
    AVLTree* avl;
    SplayTree* splay;
    //queue<K> keys;
    list<K> keys;
    unsigned int maxNumOfKeys;

public:
    BKUTree(int maxNumOfKeys = 5)
    {
        this->maxNumOfKeys = maxNumOfKeys;
        this->avl = new AVLTree();
        this->splay = new SplayTree();
    }
    ~BKUTree() {
        this->clear();
        delete this->avl;
        delete this->splay;
    }

    void printTree()
    {
        cout << "------------------------- BKU Tree -------------------------" << endl;
        cout << "Splay Tree : " << endl;
        this->splay->printTree();
        cout << endl;
        cout << "AVL Tree : " << endl;
        this->avl->printTree();
    }

    void add(K key, V value)
    {
        typename SplayTree::Node* splayCorr = this->splay->add(key, value);
        typename AVLTree::Node* avlCorr = this->avl->add(key, value, splayCorr);
        this->splay->setRootCorr(avlCorr);

        if (this->maxNumOfKeys > 0)
        {
            if (this->keys.size() == this->maxNumOfKeys)
            {
                this->keys.pop_front();
            }
            this->keys.push_back(key);
        }
    }
    void remove(K key)
    {
        this->avl->remove(key);
        this->splay->remove(key);

        if (this->maxNumOfKeys > 0)
        {
            size_t oldSize = this->keys.size();
            this->keys.remove(key);
            size_t newSize = this->keys.size();
            if (oldSize != newSize && this->splay->root != NULL)
            {
                this->keys.push_back(this->splay->getRootKey());
            }
        }
    }
    V search(K key, vector<K>& traversedList)
    {
        if (this->splay->root == NULL)
            throw "Not found";
        V result;
        if (key == this->splay->getRootKey())
        {

            result = this->splay->getRootValue();
        }
        else
        {
            bool check = false;
            for (K& k : keys)
            {
                if (k == key)
                {
                    check = true;
                    break;
                }
            }
            if (check == true)
                result = this->splay->search(key, traversedList);
            else
            {
                typename AVLTree::Node* corrAVLNode = this->splay->corrNodeOfRoot();
                typename SplayTree::Node* SplayCorr;
                result = this->avl->searchRec(corrAVLNode, NULL, key, traversedList, SplayCorr, NULL);
                this->splay->splayOne(SplayCorr);
            }
        }

        if (this->keys.size() == this->maxNumOfKeys)
            this->keys.pop_front();
        this->keys.push_back(key);

        return result;
    }

    void traverseNLROnAVL(void (*func)(K key, V value))
    {
        this->avl->traverseNLR(func);
    }
    void traverseNLROnSplay(void (*func)(K key, V value))
    {
        this->splay->traverseNLR(func);
    }

    void clear()
    {
        this->avl->clear();
        this->splay->clear();
        this->keys.clear();
    }

    class SplayTree {
    public:
        class Node {
            Entry* entry;
            Node* left;
            Node* right;
            Node* parent;
            typename AVLTree::Node* corr;
            friend class SplayTree;

            Node(Entry* entry = NULL, Node* left = NULL, Node* right = NULL, Node* parent = NULL) {
                this->entry = entry;
                this->left = left;
                this->right = right;
                this->parent = parent;
                this->corr = NULL;
            }

            K getKey()
            {
                return this->entry->key;
            }
            V& getValue()
            {
                return this->entry->value;
            }
            void clearEntry()
            {
                delete this->entry;
            }
        };

    public:
        Node* root;

        SplayTree() : root(NULL) {};
        ~SplayTree() { this->clear(); };

        void printTree()
        {
            this->printTreeRec(this->root, 0);
        }

        void printTreeRec(Node* root, int space)
        {
            if (root == NULL) 
                return; 
    
            // Increase distance between levels 
            space += COUNT; 
        
            // Process right child first 
            printTreeRec(root->right, space); 
        
            // Print current node after space 
            // count 
            cout<<endl; 
            for (int i = COUNT; i < space; i++) 
                cout << " "; 
            cout << root->getKey() <<"\n"; 
        
            // Process left child 
            printTreeRec(root->left, space); 
        }

        void setRootCorr(typename AVLTree::Node* corr)
        {
            this->root->corr = corr;
        }
        K getRootKey()
        {
            return this->root->getKey();
        }
        V getRootValue()
        {
            return this->root->getValue();
        }

        void rotateLeft(Node*& root) // Zag
        {
            Node* temp = root->right;
            root->right = temp->left;
            if (temp->left != NULL)
                root->right->parent = root;
            temp->left = root;
            temp->parent  = root->parent;
            root = temp;
            root->left->parent = root;
        }
        void rotateRight(Node*& root) //Zig
        {
            Node* temp = root->left;
            root->left = temp->right;
            if (temp->right != NULL)
                root->left->parent = root;
            temp->right = root;
            temp->parent = root->parent;
            root = temp;
            root->right->parent = root;
        }
        void splay(Node* root, Node*& ancestorPtr, Node* ancestor = NULL, bool splayOne = false)
        {
            if (root == ancestor || root->parent == ancestor)
                return;
            Node *pPtr = root->parent, *ppPtr = pPtr->parent;
            if (pPtr->left == root)
            {
                if (ppPtr == ancestor) // Zig
                    this->rotateRight(ancestorPtr);
                else if (ppPtr->left == pPtr) // Zig Zig
                {
                    Node* pppPtr = ppPtr->parent;
                    if (pppPtr == ancestor)
                    {
                        this->rotateRight(ancestorPtr);
                        this->rotateRight(ancestorPtr);
                    }
                    else if (pppPtr->left == ppPtr)
                    {
                        this->rotateRight(pppPtr->left);
                        this->rotateRight(pppPtr->left);
                    }
                    else
                    {
                        this->rotateRight(pppPtr->right);
                        this->rotateRight(pppPtr->right);
                    }
                }
                else // Zag Zig
                {
                    this->rotateRight(ppPtr->right);
                    Node* pppPtr = ppPtr->parent;
                    if (pppPtr == ancestor)
                        this->rotateLeft(ancestorPtr);
                    else if (pppPtr->left == ppPtr)
                        this->rotateLeft(pppPtr->left);
                    else this->rotateLeft(pppPtr->right);
                }
            }
            else
            {
                if (ppPtr == ancestor) // Zag
                    this->rotateLeft(ancestorPtr);
                else if (ppPtr->right == pPtr) // Zag Zag
                {
                    Node* pppPtr = ppPtr->parent;
                    if (pppPtr == ancestor)
                    {
                        this->rotateLeft(ancestorPtr);
                        this->rotateLeft(ancestorPtr);
                    }
                    else if (pppPtr->right == ppPtr)
                    {
                        this->rotateLeft(pppPtr->right);
                        this->rotateLeft(pppPtr->right);
                    }
                    else
                    {
                        this->rotateLeft(pppPtr->left);
                        this->rotateLeft(pppPtr->left);
                    }
                }
                else // Zig Zag
                {
                    this->rotateLeft(ppPtr->left);
                    Node* pppPtr = ppPtr->parent;
                    if (pppPtr == ancestor)
                        this->rotateRight(ancestorPtr);
                    else if (pppPtr->right == ppPtr)
                        this->rotateRight(pppPtr->right);
                    else this->rotateRight(pppPtr->left);
                }
            }

            if (splayOne == false)
                this->splay(root, ancestorPtr, ancestor);
        }
        void splayOne(Node* root)
        {
            this->splay(root, this->root, NULL, true);
        }

        void insert(Node*& root, Node* parentRoot, Node* ptr)
        {
            if (root == NULL)
            {
                root = ptr;
                root->parent = parentRoot;
                this->splay(root, this->root);
                return;
            }

            if (root->getKey() == ptr->getKey())
                throw "Duplicate key";
            else if (root->getKey() > ptr->getKey())
            {
                this->insert(root->left, root, ptr);
            }
            else
            {
                this->insert(root->right, root, ptr);
            }
        }
        Node* add(K key, V value)
        {
            Entry* e = new Entry(key, value);
            Node* ptr = new Node(e);
            this->insert(this->root, NULL, ptr);
            return ptr;
        }
        Node* add(Entry* entry)
        {
            if (entry == NULL)
                return NULL;
            Node* ptr = new Node(entry);
            this->insert(this->root, ptr);
            return ptr;
        }

        void del(K key, Node*& root)
        {
            if (root == NULL)
                throw "Not found";
            else if (root->getKey() < key)
                this->del(key, root->right);
            else if (root->getKey() > key)
                this->del(key, root->left);
            else
            {
                this->splay(root, this->root);
                Node* deletePtr = this->root;
                if (this->root->left == NULL)
                {
                    this->root = this->root->right;
                    if (this->root != NULL)
                        this->root->parent = NULL;
                    deletePtr->clearEntry();
                    delete deletePtr;
                    return;
                }
                if (this->root->right == NULL)
                {
                    this->root = this->root->left;
                    if (this->root != NULL)
                        this->root->parent = NULL;
                    deletePtr->clearEntry();
                    delete deletePtr;
                    return;
                }
                Node* largestLeft = this->root->left;
                while (largestLeft->right != NULL)
                    largestLeft = largestLeft->right;
                this->splay(largestLeft, this->root->left, this->root);
                this->root->left->right = this->root->right;
                this->root = this->root->left;
                this->root->parent = NULL;
                if (this->root->right != NULL)
                    this->root->right->parent = this->root;
                deletePtr->clearEntry();
                delete deletePtr;
            }
        }
        void remove(K key)
        {
            this->del(key, this->root);
        }

        V searchRec(Node* root, K key)
        {
            if (root == NULL)
                throw "Not found";
            if (root->getKey() == key)
            {
                this->splay(root, this->root);
                return root->getValue();
            }
            else if (root->getKey() > key)
                return this->searchRec(root->left, key);
            else return this->searchRec(root->right, key);
        }
        V search(K key)
        {
            return this->searchRec(this->root, key);
        }

        //for bku tree
        typename AVLTree::Node* corrNodeOfRoot()
        {
            return this->root->corr;
        }
        V searchRec(Node* root, K key, vector<K>& traversedList)
        {
            if (root == NULL)
                throw "Not found";
            if (root->getKey() == key)
            {
                this->splayOne(root);
                return root->getValue();
            }
            else if (root->getKey() > key)
            {
                traversedList.push_back(root->getKey());
                return this->searchRec(root->left, key, traversedList);
            }
            else
            {
                traversedList.push_back(root->getKey());
                return this->searchRec(root->right, key, traversedList);
            }
        }
        V search(K key, vector<K>& traversedList)
        {
            return this->searchRec(this->root, key, traversedList);
        }

        void traverseNLRRec(void (*func)(K key, V value), Node* root)
        {
            if (root == NULL)
                return;
            (*func)(root->getKey(), root->getValue());
            this->traverseNLRRec(func, root->left);
            this->traverseNLRRec(func, root->right);
        }
        void traverseNLR(void (*func)(K key, V value))
        {
            this->traverseNLRRec(func, this->root);
        }

        void clearRec(Node*& root)
        {
            if (root == NULL)
                return;
            this->clearRec(root->left);
            this->clearRec(root->right);
            root->clearEntry();
            delete root;
            root = NULL;
        }
        void clear()
        {
            this->clearRec(this->root);
        }
    };

    class AVLTree {
    public:
        class Node {
            Entry* entry;
            Node* left;
            Node* right;
            int balance;
            typename SplayTree::Node* corr;
            friend class AVLTree;

            Node(Entry* entry = NULL, Node* left = NULL, Node* right = NULL) {
                this->entry = entry;
                this->left = left;
                this->right = right;
                this->balance = 0;
                this->corr = NULL;
            }

            K getKey()
            {
                return this->entry->key;
            }
            V& getValue()
            {
                return this->entry->value;
            }
            void clearEntry()
            {
                delete this->entry;
            }
        };

    public:
        Node* root;

        AVLTree() : root(NULL) {};
        ~AVLTree() { this->clear(); };

        void printTree()
        {
            this->printTreeRec(this->root, 0);
        }

        void printTreeRec(Node* root, int space)
        {
            if (root == NULL) 
                return; 
    
            // Increase distance between levels 
            space += COUNT; 
        
            // Process right child first 
            printTreeRec(root->right, space); 
        
            // Print current node after space 
            // count 
            cout<<endl; 
            for (int i = COUNT; i < space; i++) 
                cout << " "; 
            cout << root->getKey() <<"\n"; 
        
            // Process left child 
            printTreeRec(root->left, space); 
        }

        void rotateLeft(Node*& root)
        {
            Node* temp = root->right;
            root->right = temp->left;
            temp->left = root;
            root = temp;
        }
        void rotateRight(Node*& root)
        {
            Node* temp = root->left;
            root->left = temp->right;
            temp->right = root;
            root = temp;
        }

        void leftBalance(Node*& root, bool& taller)
        {
            Node* leftTree = root->left;
            if (leftTree->balance == -1)
            {
                this->rotateRight(root);
                root->right->balance = 0;
                leftTree->balance = 0;
                taller = false;
            }
            else
            {
                Node* rightTree = leftTree->right;
                if (rightTree->balance == -1)
                {
                    root->balance = 1;
                    leftTree->balance = 0;
                }
                else if (rightTree->balance == 0)
                {
                    root->balance = 0; ///////////////////
                    leftTree->balance = 0;
                }
                else
                {
                    root->balance = 0;
                    leftTree->balance = -1;
                }
                rightTree->balance = 0;
                this->rotateLeft(root->left);
                this->rotateRight(root);
                taller = false;
            }
        }
        void rightBalance(Node*& root, bool& taller)
        {
            Node* rightTree = root->right;
            if (rightTree->balance == 1)
            {
                this->rotateLeft(root);
                root->left->balance = 0;
                rightTree->balance = 0;
                taller = false;
            }
            else
            {
                Node* leftTree = rightTree->left;
                if (leftTree->balance == 1)
                {
                    root->balance = -1;
                    rightTree->balance = 0;
                }
                else if (leftTree->balance == 0)
                {
                    root->balance = 0; ///////////////////
                    rightTree->balance = 0;
                }
                else
                {
                    root->balance = 0;
                    rightTree->balance = 1;
                }
                leftTree->balance = 0;
                this->rotateRight(root->right);
                this->rotateLeft(root);
                taller = false;
            }
        }
        void insert(Node*& root, Node* ptr, bool& taller)
        {
            if (root == NULL)
            {
                root = ptr;
                taller = true;
                return;
            }

            if (ptr->getKey() == root->getKey())
                throw "Duplicate key";
            else if (ptr->getKey() < root->getKey())
            {
                this->insert(root->left, ptr, taller);
                if (taller == true)
                {
                    if (root->balance == -1)
                        this->leftBalance(root, taller);
                    else if (root->balance == 0)
                        root->balance = -1;
                    else
                    {
                        root->balance = 0;
                        taller = false;
                    }
                }
            }
            else
            {
                this->insert(root->right, ptr, taller);
                if (taller == true)
                {
                    if (root->balance == 1)
                        this->rightBalance(root, taller);
                    else if (root->balance == 0)
                        root->balance = 1;
                    else
                    {
                        root->balance = 0;
                        taller = false;
                    }
                }
            }
        }
        Node* add(K key, V value, typename SplayTree::Node* splayCorr = NULL)
        {
            Entry* e = new Entry(key, value);
            Node* ptr = new Node(e);
            ptr->corr = splayCorr;
            bool taller;
            this->insert(this->root, ptr, taller);
            return ptr;
        }
        Node* add(Entry* entry)
        {
            if (entry == NULL)
                return NULL;
            Node* ptr = new Node(entry);
            bool taller;
            this->insert(this->root, ptr, taller);
            return ptr;
        }

        void deleteLeftBalance(Node*& root, bool& shorter)
        {
            if (root->balance == 1)
                root->balance = 0;
            else if (root->balance == 0)
            {
                root->balance = -1;
                shorter = false;
            }
            else
            {
                Node* leftTree = root->left;
                if (leftTree->balance == 1)
                {
                    Node* rightTree = leftTree->right;
                    if (rightTree->balance == 1)
                    {
                        leftTree->balance = -1;
                        root->balance = 0;
                    }
                    else if (rightTree->balance == 0)
                    {
                        root->balance = 0;
                        leftTree->balance = 0;
                    }
                    else
                    {
                        root->balance = 1;
                        leftTree->balance = 0;
                    }
                    rightTree->balance = 0;
                    this->rotateLeft(root->left);
                    this->rotateRight(root);
                }
                else
                {
                    if (leftTree->balance == -1)
                    {
                        root->balance = 0;
                        leftTree->balance = 0;
                    }
                    else
                    {
                        root->balance = -1;
                        leftTree->balance = 1;
                        shorter = false;
                    }
                    this->rotateRight(root);
                }
            }
        }
        void deleteRightBalance(Node*& root, bool& shorter)
        {
            if (root->balance == -1)
                root->balance = 0;
            else if (root->balance == 0)
            {
                root->balance = 1;
                shorter = false;
            }
            else
            {
                Node* rightTree = root->right;
                if (rightTree->balance == -1)
                {
                    Node* leftTree = rightTree->left;
                    if (leftTree->balance == -1)
                    {
                        rightTree->balance = 1;
                        root->balance = 0;
                    }
                    else if (leftTree->balance == 0)
                    {
                        root->balance = 0;
                        rightTree->balance = 0;
                    }
                    else
                    {
                        root->balance = -1;
                        rightTree->balance = 0;
                    }
                    leftTree->balance = 0;
                    this->rotateRight(root->right);
                    this->rotateLeft(root);
                }
                else
                {
                    if (rightTree->balance == 1)
                    {
                        root->balance = 0;
                        rightTree->balance = 0;
                    }
                    else
                    {
                        root->balance = 1;
                        rightTree->balance = -1;
                        shorter = false;
                    }
                    this->rotateLeft(root);
                }
            }
        }
        void del(Node*& root, K key, bool& shorter, bool& success)
        {
            if (root == NULL)
            {
                //shorter = false;
                //success = false;
                throw "Not found";
            }

            if (key < root->getKey())
            {
                this->del(root->left, key, shorter, success);
                if (shorter == true)
                    this->deleteRightBalance(root, shorter);
            }
            else if (key > root->getKey())
            {
                this->del(root->right, key, shorter, success);
                if (shorter == true)
                    this->deleteLeftBalance(root, shorter);
            }
            else
            {
                Node* deleteNode = root;
                if (root->right == NULL) // no left subtree
                {
                    root = root->left;
                    success = true;
                    shorter = true;
                    deleteNode->clearEntry();
                    delete deleteNode;
                    return;
                }
                else if (root->left == NULL) // no right subtree
                {
                    root = root->right;
                    success = true;
                    shorter = true;
                    deleteNode->clearEntry();
                    delete deleteNode;
                    return;
                }
                else
                {
                    Node* exchPtr = root->left;
                    while (exchPtr->right != NULL)
                        exchPtr = exchPtr->right;
                    root->entry->key = exchPtr->entry->key;
                    root->entry->value = exchPtr->entry->value;
                    this->del(root->left, exchPtr->getKey(), shorter, success);
                    if (shorter == true)
                        this->deleteRightBalance(root, shorter);
                }
            }
        }
        void remove(K key)
        {
            bool shorter, success;
            if (this->root == NULL)
                return;
            this->del(this->root, key, shorter, success);
        }

        V searchRec(K key, Node* root)
        {
            if (root == NULL)
                throw "Not found";
            if (root->getKey() == key)
                return root->getValue();
            else if (root->getKey() > key)
                return this->searchRec(key, root->left);
            else return this->searchRec(key, root->right);
        }
        V search(K key)
        {
            return this->searchRec(key, this->root);
        }

        // for bku tree
        V searchRec(Node* root, Node* parentRoot, K key, vector<K>& traversedList, typename SplayTree::Node*& SplayCorr, Node* traversedRoot = NULL)
        {
            if (traversedRoot != NULL && traversedRoot == root)
            {
                throw "Not found";
            }
            if (root == NULL)
            {
                if (traversedRoot == NULL)
                    return this->searchRec(this->root, NULL, key, traversedList, SplayCorr, parentRoot);
                throw "Not found";
            }
            else if (root->getKey() < key)
            {
                traversedList.push_back(root->getKey());
                return this->searchRec(root->right, root, key, traversedList, SplayCorr, traversedRoot);
            }
            else if (root->getKey() > key)
            {
                traversedList.push_back(root->getKey());
                return this->searchRec(root->left, root, key, traversedList, SplayCorr, traversedRoot);
            }
            else
            {
                SplayCorr = root->corr;
                return root->getValue();
            }
        }

        void traverseNLRRec(void (*func)(K key, V value), Node* root)
        {
            if (root == NULL)
                return;
            (*func)(root->getKey(), root->getValue());
            this->traverseNLRRec(func, root->left);
            this->traverseNLRRec(func, root->right);
        }
        void traverseNLR(void (*func)(K key, V value))
        {
            this->traverseNLRRec(func, this->root);
        }

        void clearRec(Node*& root)
        {
            if (root == NULL)
                return;
            this->clearRec(root->left);
            this->clearRec(root->right);
            root->clearEntry();
            delete root;
            root = NULL;
        }
        void clear()
        {
            this->clearRec(this->root);
        }
    };
};

