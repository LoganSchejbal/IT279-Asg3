//============================================================================
// File       : AVL.h (Students should not modify this program)
// Author     : Chung-Chih Li
// Date       : Jan 31, 2026 Created/Modified
// Description: AVL herder file
//============================================================================

#ifndef AVL_H_
#define AVL_H_

/*******************************************************/
/*  AVL Tree Node                    by  Chung-Chih Li */
/*******************************************************/
template<typename T>
class Node {
public:
		Node(int key, T data):
			key(key), height(1), data(data), left(NULL), right(NULL) {
		}
		int updateHeight() {
			int l = (left == NULL ? 0 : left->height);
			int r = (right == NULL ? 0 : right->height);
			height = 1+(l > r ? l : r);
			return height;
		}
		int key;
		int height;
		T data;  // Data
		Node<T> *left, *right;
};

template<typename T>
class AVL {
public:
		AVL(): root(NULL) {};
		void insert(int key, T data);
		void remove(int key);  // Remove a node with this key
		T get(int key);  // Get the data with key
		bool hasKey(int key); // True if key exists

		int size();    // Return the number of nodes of this tree
		int height();  // Return the height of this tree
		int maxKey();  // Return the maximum key
		int minKey();  // Return the minimum key
		T max();       // Return a pointer of data with max key
		T min();       // Return a pointer of data with min key
		T *inorder();  // Return an array of pointers of all data in inorder
		T *preorder(); // Return an array of pointers of all data in preorder
		T *postorder();// Return an array of pointers of all data in postorder

		AVL<T> & operator = (const AVL<T> & a); // Big Three
		AVL(const AVL<T> & root);
		~AVL();

private:
		Node<T> * root;

friend void testDescructor(int n);
};

#endif /* AVL_H_ */
