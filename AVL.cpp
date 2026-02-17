#include "AVL.h"
using namespace std;

template <typename T>

/*
****************Inserttion Implementation******************************
*
*Takes an int key and a type, T, data and creates a new node at correct place in this tree
*Relies on recursive insertion function to find correct place in tree and insert node
*Also considers need for Rotation
*/
void AVL<T>::insert(int key, T data){
	if (root == nullptr){
        root = new Node<T>(key, data);
        return;
    }
    else{
        insertion(root, key, data);
        root = checkForRotations(root);
        rotationDriver(root);
    }
}

/*
*Takes a Node<T> pointer of tree location (can be subtree), an int key, and a type T data to insert new node
*Recursively locates empty node in correct location and adds key to the left/right pointer of tree
*/
template <typename T>
void insertion(Node<T>* tree, int key, T data){
    if (key < tree->key){
        if (tree->left == nullptr){
            tree->left = new Node<T>(key, data);
        }
        else{
            insertion(tree->left, key, data);
        }
    }
    else if (key > tree->key){
        if (tree->right == nullptr){
            tree->right = new Node<T>(key, data);
        }
        else{
            insertion(tree->right, key, data);
        }
    }
}



/*
 *******************Removal Implementation****************************
 *
 * Takes an int key to locate, remove, and adjust nodes
 * Uses searchKeys() function to return a location of a node which is to be destroyed. The case is then determined (number of children) to properly delete node
 * FindParent() function may be used to locate parent node of what is selected to be removed
 */
template <typename T>
void AVL<T>::remove(int key){
    Node<T>* node = searchKeys(root, key);
    if (node == nullptr){
        return;
    }

    //No children
    if (node->left != nullptr && node->right != nullptr){
        Node<T>* parent = node;
        Node<T>* child = node->left;
        while(child->right != nullptr){
            parent = child;
            child = child->right;
        }
        node->key = child->key;
        node->data = child->data;
        if (parent == node){
            parent->left = child->left;
        } else {
            parent->right = child->left;
        }
        delete child;
    }

    //Only left Child
    else if (node->left != nullptr){
        if (node == root){
            root = node->left;
        } else {
            Node<T>* parent = findParent(node->key, root);
            if (parent->left == node) parent->left = node->left;
            else parent->right = node->left;
        }
        delete node;
    }

    ///Only right child
    else if (node->right != nullptr){
        if (node == root){
            root = node->right;
        } else {
            Node<T>* parent = findParent(node->key, root);
            if (parent->left == node) parent->left = node->right;
            else parent->right = node->right;
        }
        delete node;
    }

    //No children
    else{
        if (node == root){
            root = nullptr;
        } else {
            Node<T>* parent = findParent(node->key, root);
            if (parent->left == node) parent->left = nullptr;
            else if (parent->right == node) parent->right = nullptr;
        }
        delete node;
    }
    root = checkForRotations(root);
    rotationDriver(root);
}

/*
* Takes in int key and Node<T> pointer, tree (may be subtree). 
* Locates the parent of key recursively
*/
template <typename T>
Node<T>* findParent(int key, Node<T>* tree){
    if (tree == nullptr) return nullptr;
    if ((tree->left != nullptr && tree->left->key == key) ||
        (tree->right != nullptr && tree->right->key == key)){
        return tree;
    }
    if (key < tree->key){
        return findParent(key, tree->left);
    } else {
        return findParent(key, tree->right);
    }
}



/**
 *****************Searching Functions********************
 * 
 * Takes in an int key of the desired node and returns a type, T, data 
 * Relies on searchKeys() function
 */
template <typename T>
T AVL<T>::get(int key){
    return (*searchKeys (root, key)).data;
}

/*
* Takes in an int key of the desired node and returns a bool True if the node exists or a bool False if it does not exist in the tree
* Relies on the searchKeys function to determine a key's existence. If the return is !nullptr, it exists. If return is nullptr it does not exist
*/
template <typename T>
bool AVL<T>::hasKey(int key){
    if (searchKeys(root, key) != nullptr){
        return true;
    }
    return false;
}

/*
* Takes in Node<T> pointer tree (can be a subtree) and an int key of desired node
* Uses recursion to compare keys until the key is found. If it is not found, it returns nullptr
*/
template<typename T>
Node<T>* searchKeys(Node<T>* tree, int key){
    if (tree == nullptr){
        return nullptr;
    }
    if (tree->key == key){
        return tree;
    }
    else{
        return tree->key < key ? searchKeys(tree->right, key) : searchKeys(tree->left, key);
    }
    return nullptr;
}


/**
 *****************Size and Height Related Functions**********************
 *
 * No parameters, returns an int of the total number of nodes in a tree
 * Relies on findSize() recursive helper function to visit each node and count it
 */
template <typename T>
int AVL<T>::size(){
    if (root == nullptr){
        return 0;
    }
    else {
        return findSize(root);
    }
}

/*
* Takes in a Node<T> pointer of tree (can be a subtree) and returns an int of all of its nodes
* Recursively adds 1 for every visited node or 0 for empty nodes
*/
template <typename T>
int findSize(Node<T>* tree){
    int sum = 0;
    if (tree == nullptr){
        return 0;
    }
    else{
        return 1 + findSize(tree->left) + findSize(tree->right);
    }
}

/*
* No parameters, returns the height to the deepest node in the tree
* returns stored int height in the Node<T> root
*/
template <typename T>
int AVL<T>::height(){
    return root->height;
}


/*
* Takes Node<T> pointer tree (can be a subtree) and triggers the updateHeight method in each node
* Recursively prompts all child nodes to update first before updating tree node. Returns when an empty node is reached
* Called whnever a potentially height changing event occurs
*/
template <typename T>
void forceUpdateHeight(Node<T>* tree){
    if (tree == nullptr){
        return;
    }
    forceUpdateHeight(tree->left);
    forceUpdateHeight(tree->right);
    tree->updateHeight();
    return;
}


/**
 *******************Min/Max Functions**********************
 *
 * No parameters, returns an int of the largest key
 * Searches from root and moves right until it can't anymore and returns its key. This will always be the max if the tree is properly constructed
 */
template <typename T>
int AVL<T>::maxKey(){
    Node<T>* currentPosition = root;
    while (currentPosition->right != nullptr){
        currentPosition = currentPosition->right;
    }
    return currentPosition->key;
}

/*
 * No parameters, returns an int of the smallest key
 * Searches from root and moves left until it can't anymore and returns its key. This will always be the min if the tree is properly constructed
 */
template <typename T>
int AVL<T>::minKey(){
    Node<T>* currentPosition = root;
    while (currentPosition->left != nullptr){
        currentPosition = currentPosition->left;
    }
    return currentPosition->key;
}

/*
* No parameters, returns a type T data of the maximum value of data in the tree
* relies on maxData() recursive helper function to search all nodes in tree. Returns 0 if root is empty
* Compares the value of the data. If string, it compares the actual value of the characters in the string, not the value itself
*/
template <typename T>
T AVL<T>::max(){
    if(root != nullptr){
        return maxData(root, root->data);
    }
    else{
        return 0;
    }
}

/*
* Takes in Node<T> pointer tree (can be a subtree) and type T max to compare to the current node's max
* Compares max to current data, kept if larger or returns max. Continues recursively until empty node is reached
*/
template<typename T>
T maxData(Node<T>* tree, T max){
    if (tree == nullptr){
        return max;
    }
    if(max < tree->data){
        max = tree->data;
    }
    if(tree->left != nullptr){
        max = maxData(tree->left, max);
    }
    if(tree->right!= nullptr){
        max = maxData(tree->right, max);
    }
    
    return max;

}

/*
* No parameters, returns a type T data of the minimum value of data in the tree
* relies on minData() recursive helper function to search all nodes in tree. Returns 0 if root is empty
* Compares the value of the data. If string, it compares the actual value of the characters in the string, not the value itself
*/
template <typename T>
T AVL<T>::min(){
    if(root != nullptr){
        return minData(root, root->data);
    }
    else{
        return 0;
    };
}

/*
* Takes in Node<T> pointer tree (can be a subtree) and type T min to compare to the current node's min
* Compares min to current data, kept if smaller or returns min. Continues recursively until empty node is reached
*/
template <typename T>
T minData(Node<T>* tree, T min){
    if (tree == nullptr){
        return min;
    }
    if(min > tree->data){
        min = tree->data;
    }
    if(tree->left != nullptr){
        min = minData(tree->left, min);
    }
    if(tree->right!= nullptr){
        min = minData(tree->right, min);
    }
    return min;
}



/**
 **********Exporting tree Functions**************
 * 
 * Returns a pointer of array<T> of all data values in in-order format, no parameters
 * Relies on createInorderList() recursive helper function to populate the array with the T data
 */
template <typename T>
T* AVL<T>::inorder(){
    T* results = new T[size()];
    int i = 0;
    if (root == nullptr){
        return results;
    }
    createInorderList(this->root, results, i);
    return results;
}

/*
 * Takes in a Node<T> pointer, tree, (can be a subtree) and a pointer of an array T pointer, results and the address of the current index number 
 * The Program goes to the leftmost leaf, before adding the data to the array, returning, adding the data and moving right. This continues until it reaches the end
 */
template <typename T>
void createInorderList(Node<T>* tree, T* results, int &i){
    if(tree->left != nullptr){
        createInorderList(tree->left, results, i);
    }
    results[i] = tree->data;
    ++i;
    if(tree->right != nullptr){
        createInorderList(tree->right, results, i);
    }

}

/*
* Returns a pointer to an array T of all data in preorder format
* Relies on createPreorderList() to recursively populate the array
*/
template <typename T>
T* AVL<T>::preorder(){
    T* results = new T[size()];
    int i = 0;
    if (root == nullptr){
        return results;
    }
    createPreorderList(this->root, results, i);
    return results;
}

/*
* Takes in a Node<T> pointer, tree (Can be a subtree), a pointer to the array and the address of an int i index
* First adds the data value, then moves left and applies the same function before moving right
*/
template <typename T>
void createPreorderList(Node<T>* tree, T* results, int &i){
    results[i] = tree->data;
    ++i;
    if(tree->left != nullptr){
        createPreorderList(tree->left, results,i);
    }
    if (tree->right != nullptr){
        createPreorderList(tree->right, results, i);
    }
}

/*
* Returns an array T pointer of all data values within the tree in postorder format
* Relies on createPostorderList to populate the array
*/
template <typename T>
T* AVL<T>::postorder(){
    T* results = new T[size()];
    int i = 0;
    if (root == nullptr){
        return results;
    }
    createPostorderList(this->root, results, i);
    return results;
}

/*
* Takes in a Node<T> pointer, tree (Can be a subtree), a pointer to the array and the address of an int i index
* Moves as far left as possible, adds its data, before moving to its sibling node and return up, adding data as it goes
*/
template <typename T>
void createPostorderList(Node<T>* tree, T* results, int &i){
    if(tree->left != nullptr){
        createPostorderList(tree->left, results, i);
    }
    if (tree->right != nullptr){
        createPostorderList(tree->right, results, i);
    }
    results[i] = tree->data;
    ++i;
}




/**
 ***********************Copy, Assignment, Destructor Implementations*********************************
 * 
 * Takes in the address of the second object when using the "=" operator and alters the first object to match it
 * Destroys the original object in most cases using the custom destroy() function and recreates the tree structure using constructTree
 */
template <typename T>
AVL<T> & AVL<T>::operator = (const AVL<T> & a){
    if (this == &a){
        return *this;
    }
    if(this->root != nullptr){
        destroy(this->root);
        this->root = nullptr;
    }
    if (a.root != nullptr){
        this->root = new Node<T>(a.root->key, a.root->data);
        constructTree(a.root, this->root);
    }
    else{
        this->root = nullptr;
    }
    forceUpdateHeight(this->root);
    return *this;
}

/*
 * This constructor accepts a parameter of another AVL<T> object's address 
 * The object is fully coppied to the new object using constructTree() which traverses the original and constructs the copy. The root node is initialized here for simplicity
 */
template <typename T>
AVL<T>::AVL(const AVL<T> & root){
    if (root.root == nullptr){
        this->root = nullptr;
    } 
    else{
        this->root = new Node<T>(root.root->key, root.root->data);
        constructTree(root.root, this->root);
    }
    forceUpdateHeight(this->root);
}

/*
 * The parameters for this function are two Node<T> pointers, the original which was given as aparameter in the constructor(Can be a subtree)and the new copy
 * The recursive function will always move left first, copying the nodes, before returning and moving right and copying the node
 */
template <typename T>
void constructTree(const Node<T>* original, Node<T>* copy){
    if(original->left != nullptr){
        copy->left = new Node<T>(original->left->key, original->left->data);
        constructTree(original->left, copy->left);
    }
    if (original->right != nullptr){
        copy->right = new Node<T>(original->right->key, original->right->data);
        constructTree(original->right, copy->right);
    }
}

/*
* Deletion function, no return or parametrs
* Calls the destroy() function to recursively destroy all Node<T> instances inside the tree to avoid memory leaks
*/
template <typename T>
AVL<T>::~AVL(){
    if(this->root != nullptr){
        destroy(root);
        root = nullptr;
        return;
    }
}

/*
* Takes in a Node<T> pointer, tree (can be a subtree) as the location and navigates from there
* Recursively moves left, calls, and then right, calls, before deleting itself
*/
template <typename T>
void destroy(Node<T>* tree){
    if (tree == nullptr){
        return;
    }
    destroy(tree->left);
    destroy(tree->right);
    delete tree;
}



/**
 ******************Actual AVL Functionality Implementations*******************************
 * 
 * Takes in a Node<T> pointer tree (can be a subtree) to navigate the tree recursively
 * This function drives the rotations as it will move through the tree and call checkForRotation
 */
template <typename T>
void rotationDriver(Node<T>* tree){
    if (tree == nullptr){
        return;
    }
    tree->left = checkForRotations(tree->left);
    tree->right = checkForRotations(tree->right);

    rotationDriver(tree->left);
    rotationDriver(tree->right);
    forceUpdateHeight(tree);
}

/*
* Accepts a pointer of Node<T> tree to check a node
* This function essentially compares the node's children to determine if they need to rotate. It may also do a secondary comparison to determine what rotation needs to be called
*/
template <typename T>
Node<T>* checkForRotations(Node<T>* tree){
    if (tree == nullptr){
        return nullptr;
    }

    //Initial hight comparison
    forceUpdateHeight(tree);
    int left = (tree->left == nullptr ? 0 : tree->left->height);
    int right = (tree->right == nullptr ? 0 : tree->right->height);

    //Right heavy subtree that needs to be balanced. Always calls RR or RL
    if (right - left >= 2){
        int left = (tree->right->left == nullptr ? 0 : tree->right->left->height);
        int right = (tree->right->right == nullptr ? 0 : tree->right->right->height);
        if (right - left >= 0){
            return RR(tree);
        }
        else{
            return RL(tree);
        }
    }

    //Left Heavy subtree that needs to be balanced. Always calls LL or LR
    else if (right - left <= -2){
        int left = (tree->left->left == nullptr ? 0 : tree->left->left->height);
        int right = (tree->left->right == nullptr ? 0 : tree->left->right->height);
        if (right - left >= 0){
            return LR(tree);
        }
        else{
            return LL(tree);
        }
    }
    return tree;
}

/*
* Moves around the nodes to balance out the tree
* The right half is essentially shifted the left
*/
template <typename T>
Node<T>* RR(Node<T>* tree){
    if (tree == nullptr){
        return nullptr;
    }
    Node<T>* newTree = tree->right;
    Node<T>* newLeft = tree;
    Node<T>* newLeftRight = tree->right->left;

    
    newLeft->right = newLeftRight;
    newTree->left = newLeft;
    forceUpdateHeight(newTree);
    return newTree;
}

/*
* Moves around the nodes to balance out the tree
* The right subtree is essentially shifted up and left
*/
template <typename T>
Node<T>* RL(Node<T>* tree){
    if (tree == nullptr){
        return nullptr;
    }
    Node<T>* newTree = tree->right->left;
    Node<T>* newRight = tree->right;
    Node<T>* newLeft = tree;

    newRight->left = newTree->right;
    newLeft->right = newTree->left;
    newTree->left = newLeft;
    newTree->right = newRight;
    forceUpdateHeight(newTree);
    return newTree;
}

/*
* Moves around the nodes to balance out the tree
* The left subtree is essentially shifted up and right
*/
template <typename T>
Node<T>* LR(Node<T>* tree){
    if (tree == nullptr){
        return nullptr;
    }
    Node<T>* newTree = tree->left->right;
    Node<T>* newRight = tree;
    Node<T>* newLeft = tree->left;

    newRight->left = newTree->right;
    newLeft->right = newTree->left;
    newTree->left = newLeft;
    newTree->right = newRight;
    forceUpdateHeight(newTree);
    return newTree;
}

/*
* Moves around the nodes to balance out the tree
* The left subtree is essentially just shifted right
*/
template <typename T>
Node<T>* LL(Node<T>* tree){
    if (tree == nullptr){
        return nullptr;
    }
    Node<T>* newTree = tree->left;
    Node<T>* newRight = tree;
    Node<T>* newRightLeft = tree->left->right;
    
    newRight->left = newRightLeft;
    newTree->right = newRight;
    forceUpdateHeight(newTree);
    return newTree;
}
