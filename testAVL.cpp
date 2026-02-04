//============================================================================
// File       : testAVL.cpp (Students should not modify this program)
// Author     : Chung-Chih Li
// Date       : Jan 31, 2026 Created/Modified
// Description : This is a test program for AVL trees.
//============================================================================
#include <time.h>       /* clock_t, clock, CLOCKS_PER_SEC */
#include <cstdlib>
#include <iostream>
#include <string>
#include "AVL.h"
#include "AVL.cpp"
using namespace std;

/**
 * Convert int to string
 */
string inttostr(int n) {
	string digits="0123456789";
	if (n<0) return "-"+inttostr(-1*n);
	string a = digits.substr(n%10,1);
	if (n<10) return a;
	return inttostr(n/10)+a;
}

/**
 * Convert string to int
 */
int strtoint(string s) {
	if (s=="")
		return 0;
	return strtoint(s.substr(0,s.length()-1))*10+(s.at(s.length()-1)-'0');
}

/**
 * New an int array of size n and put in n random from 0 to n-1, no duplication
 */
int * populateRandomKeys(int n) {
	int temp, k, *a = new int[n];
	for (int i=0;i<n;i++) a[i]=i*10;
	for (int i=0;i<n;i++) {
		k = rand()%n;
		temp = a[i];
		a[i]=a[k];
		a[k]=temp;
	}
	return a;
}

/**
 *  Print first n items of a, size is the size of a
 */
template<typename T>
void printArray(T a[], int n, int size) {
	cout << " [";
	for (int i=0; i< size && i < n; i++) {
			cout << *(a+i) << " ";
	}
	if (n >= size)
		cout << "]";
	else
		cout <<  ".........";
}



/**
 * Add n keys in array keys to AVL tree X
 */
void addmore(AVL<string> * t, int *keys, int n) {
	cout << "Adding " << n << " items: ";
	for (int i=0;i<n;i++) {
		if (i < 15) cout << keys[i] << " ";
		string data = inttostr(keys[i]);
		t->insert(keys[i], data);
	}
	if (n >= 15)
		cout << ".........";
	cout << endl;
}


/**
 *  Remove first n items from X with keys in the keys array
*/
void removesome(AVL<string> * t,  int *keys, int n) {
	for (int i=0;i<n;i++) {
		t->remove(keys[i]);
	}
}

/**
 *  Test copy constructor. The  argument will be copied in first.
 */
void testCopyConstructor(AVL<string> t){
	cout << "\n\n** Test copy constructor";
	AVL<string> x(t);
	t.remove(t.minKey());
	t.insert(0,"0");
	cout << "\nt: "; printArray(t.inorder(),10,10);
	cout << "\nx: "; printArray(x.inorder(),10,10);
}

/**
 *  Test Assignment Operator
 */
void testAssignmentOperator(AVL<string> t){
	cout << "\n\n** Test Assignment Operator";
	AVL<string> x,y,z;
	x = y = z = t;
	t.remove(t.minKey());
	x.remove(x.minKey());
	y.remove(y.minKey());
	z.remove(z.minKey());
	cout << "\nt: "; printArray(t.inorder(),9,9);
	cout << "\nx: "; printArray(x.inorder(),9,9);
	cout << "\ny: "; printArray(y.inorder(),9,9);
	cout << "\nz: "; printArray(z.inorder(),9,9);
}


/**
 *  Test destructor
 */
void testDescructor(int n){
	cout << "\n\n** Test destructor: (should not keep growing)";
	Node<int> *firstRoot=NULL;
	AVL<int> *x = new AVL<int>;
	for (int i=0;i<n;i++) x->insert(i,i);

	for (int i=0;i<10;i++) {
		AVL<int> y(*x);
		cout << "\n" << "i=" << i << " local AVL at " << y.root;
		if (i==0) {
			firstRoot=y.root;
		} else {
			cout << " local root address difference: " << (long) y.root - (long) firstRoot;
		}
	}
	delete x;
}

void standard_test() {
	cout << "\n****** Standard AVL Test ********\n";
	AVL<string> *t = new AVL<string>;
	int keys[10] = {535,43,458,134,368,461,409,211,457,121};
	for (int i=0;i<10;i++)
		t->insert(keys[i],inttostr(keys[i]));
	cout << "Insert ";
	printArray(keys,10,10);
	cout <<  " -> X\nSearch: ";
	for (int i=0;i<10;i++) {
		int key=keys[i]+(i%2);
		if (t->hasKey(key))
			cout << "[" << key << ":" << t->get(key) << "]";
		else
			cout << "[" << key << ":" << "X]";
	}
	cout << "\n\nSize:" << t->size();
	cout << "  Height:" << t->height();
	cout << "  Max:" << t->max();
	cout << "  Min:" << t->min();
	cout << "\nInorder:    ";
	printArray(t->inorder(),20,10);
	cout << "\nPreorder:   ";
	printArray(t->preorder(),20,10);
	cout << "\nPostorder:  ";
	printArray(t->postorder(),20,10);
	testCopyConstructor(*t);
	testAssignmentOperator(*t);
	testDescructor(100);
	cout << "\n*** Done ***\n\n";

}

/**
 * Insert n random item then remove 3/4 of them.
 */
void test(int n) {
	cout << "\n****** Test n = " << n << " ********\n";
	AVL<string> *T;
	T= new AVL<string>;
	int rn = (n*3/4), *a = populateRandomKeys(n);
	long ms = clock();
	addmore(T,a,n);
	cout << "Height = " << T->height();
	removesome(T,a,rn);
	ms = ((clock()-ms)*1000.0)/CLOCKS_PER_SEC;
	cout << "\nRemoving " << rn << " items" <<  "\nHeight -> " << T->height() <<  "\nUse " <<  ms << " ms\n";
	cout << "\nSize:   " << T->size() << endl;
	cout << "Height: " << T->height() << endl;
	cout << "Max:    " << T->max() << endl;
	cout << "Min:    " << T->min() << endl;
	cout << "Inorder:    ";
	printArray(T->inorder(),20,T->size());
	cout << "\nPreorder:   ";
	printArray(T->preorder(),20,T->size());
	cout << "\nPostorder:  ";
	printArray(T->postorder(),20,T->size());
	testDescructor(n);
	delete [] a;
	delete T;
	cout << "\n*** Done ***\n";
}

int main(int n, char * args[]) {

	srand(2026);

	if (n==1) {
		standard_test();
		test(1000);
		test(50);
	}
	else
	    test(strtoint(args[1]));
	return 0;
}

