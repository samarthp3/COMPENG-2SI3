#include "BSTSet.h"
#include <iostream>
#include <vector>
using namespace std;

// create and use class MyStack

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//The stack is implemented using a singly linked list where each node contains a pointer to a TNode
//object and a pointer to the next node in the stack.

class MyStack {
private:
	struct Node {
		TNode* data;
		Node* next;
	};
	Node* head;
	
	// private nested class with two fields
	// pointer to a TNode object, and a pointer to the next Node in the stack.


public:
	MyStack() {
		head = NULL; //initialize head to NULL
	}

	bool isEmpty() {				//check if stack is empty. If head == NULL, returns true. otherwise false
		return (head == NULL);
	}

	void push(TNode* val) {			//adds a new Node containing the given TNode* val to the top of the stack.
		Node* newNode = new Node();	//create new node
		newNode->data = val;		//set data value to val
		newNode->next = head;		//set next value to head
		head = newNode;				
	}

	TNode* pop() {					
		if (head == NULL) {
			return NULL;			//empty stack returns NULL
		}
		TNode* popVal = head->data;	//saves the TNode* contained in the top Node in the stack (head->data) to popVal
		Node* temp = head;			
		head = head->next;
		delete temp;				//reasign linkages between nodes to delete the popped node
		return popVal;
	}
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

BSTSet::BSTSet()
{
	root = NULL;			//initialize an empty tree by setting root = NULL

	//T = n
	//S(n) = 1
}

BSTSet::BSTSet(const vector<int>& input)
{
	if (input.empty()) {					
		root = NULL;						//set root to NULL if input is empty
		return;
	}
	root = new TNode(input[0], NULL, NULL);	//create new node
	
	int i = 1;
	while (i < input.size()) {
		add(input[i]);						
		i++;
	}
	// it creates a new tree node with the first element of the input vector as its data,
	//and inserts the remaining elements of the input vector into the tree by calling the add() method.

	//O(n) = n
	//S(n) = n
}

void BSTSet::Delete(TNode* &p)			//Delete function which uses recursion
{
	if (p != NULL) {
		Delete(p->right);
		Delete(p->left);
		delete p;
	}

	// The function takes a reference to a pointer to a tree node as input. If the node is not NULL,
	//the function recursively deletes its left and right subtrees and then deletes the node itself.

	//O(n) = n
	//S(n) = n
}


BSTSet::~BSTSet()
{
	Delete(root);

	//function with the root node of the tree as input, which recursively deletes all nodes in the tree

	//O(n) = n
	//S(n) = n
}


bool BSTSet::RecursiveIsIn(int v, TNode*& p)
//The function takes a reference to a pointer to a tree node p and an integer value v as input. 
{
	if (p == NULL)
		return false;

	if (v == p->element)
		return true;

	//returns true if v is found in the tree rooted at p, and false otherwise.

	if (v < p->element)
		return RecursiveIsIn(v, p->left);

	return RecursiveIsIn(v, p->right);
 
	//performs a binary search starting at the root node p and recursively traverses 
	//the tree based on the value of v until the value is found or the search reaches a leaf node.
}



bool BSTSet::isIn(int v)
{
	return RecursiveIsIn(v, root);

	//calls recursive isIn to find if a element v is in the structure

	//time: n
	//space: n
}


void BSTSet::RecursiveAdd(int v, TNode*& p)
//takes a reference to a pointer to a tree node p and an integer value v as input

{
	if (p == NULL) {
		p = new TNode(v, NULL, NULL);
		return;

	//If the search reaches a leaf node, the function creates a new node with the value v and inserts it as a child of the leaf node.
	}

	if (v < p->element)
		RecursiveAdd(v, p->left);

	else if (v > p->element)
		RecursiveAdd(v, p->right);

	//recursively search the tree for an empty position to insert the new element v
	return;
}

void BSTSet::add(int v)
{
	RecursiveAdd(v, root);

	//call recursiveadd function to add an element v to the tree

	//O(n log n)
	//S(n)
}


TNode* BSTSet::getMaximum(TNode*& p) { //takes a reference to a pointer to a tree node p as input
	if (p == NULL)
		return NULL;

	if (p->right == NULL)
		return p;

	return getMaximum(p->right);

	// and returns a pointer to the node with the maximum value in the subtree rooted at p.
	//recursively traverses the right subtree of the node until it reaches a leaf node, which is the maximum value in the subtree.
}


bool BSTSet::RecursiveRemove(int v, TNode*& p)

// takes two arguments: the value to remove v, and a pointer p to the current node being considered.
{
	if (p == NULL)		//v not found in tree, so return false
		return false;
	
	if (v < p->element)
		return RecursiveRemove(v, p->left);
	//If v < current node value, function calls itself, passing the value and the current node's left child as arguments.


	else if (v > p->element)
		return RecursiveRemove(v, p->right);
	//If v > than the current node value, function does same but passes current node right child


	else {
		if (p->left == NULL && p->right == NULL) {  //If v == current node value, checks if the node has no children
			delete p;
			p = NULL;
			return true;
		}
		//if true, set the p to NULL, return true (value was successfully removed)

		else if (p->left == NULL) {
			TNode* t = p;
			p = p->right;
			delete t;
			return true;
		}
		else if (p->right == NULL) {
			TNode* t = p;
			p = p->left;
			delete t;
			return true;
		}
		//If the node has one child (left or right), replace current node with its child, deletes original node, and return true. 

		else {
			p->element = getMaximum(p->left)->element;
			return RecursiveRemove(p->element, p->left);

			// if node has two children, find max value in left subtree of the current node, replaces current node's value 
			// with that max value, and then recursively call itself to remove the max val from the left subtree
		}
	}
}

bool BSTSet::remove(int v)
{
	return RecursiveRemove(v, root);

	//O(log n)
	//S(n)
}

void BSTSet::RecursiveUnion(const BSTSet& s, TNode* p)

//takes a constant reference to another BSTSet object s, and a pointer to a TNode object p in the current BST
{
	if (p == NULL) 
		return;
	else if (isIn(p->element) == 0)
		add(p->element);

	RecursiveUnion(s, p->left);
	RecursiveUnion(s, p->right);

	//THis function recursively traverses the current BST, and for each node that is not already in the tree,
	//it adds the node value to the BST through add method

	//O(n^2)
	//S(n)

}

void BSTSet::Union(const BSTSet &s)
{
	RecursiveUnion(s, s.root);
}

void BSTSet::RecursiveIntersection(TNode *p, BSTSet *t)
{
	if (p == NULL) 
		return;

	if (isIn(p->element))
		t->add(p->element);

	//recursively traverses p to find nodes that are also present in the current BST
	//For each node that is found in both sets, it adds the node's value to t using the add method

	RecursiveIntersection(p->left, t);
	RecursiveIntersection(p->right, t);
}

void BSTSet::intersection(const BSTSet& s)
{
	// takes a constant reference to another BSTSet object s, creates a new BSTSet object p, 
	//and recursively traverses the current BST to find nodes that are also present in s

	BSTSet* p = new BSTSet();
	RecursiveIntersection(s.root, p);
	//if intersection is found, then add the node's value to 'p'

	Delete(root);
	root = p->root;
	//deletes the current BST root and replaces it with p

	//O(n^2)
	//S(n)
}

void BSTSet::RecursiveDifference(TNode *p)
{
	if (p == NULL) 
		return;
	
	else { 	//recursively traverses p to find nodes that are not present in s
		RecursiveDifference(p->left);
		remove(p->element);
		//For each node that is found only in the current BST, it removes the node's value using the remove method

		RecursiveDifference(p->right);
	}
}

void BSTSet::difference(const BSTSet& s)
{
	RecursiveDifference(s.root);

	//O(n^2)
	//S(n)
}

int BSTSet::RecursiveSize(TNode* p)
{
	if (p == NULL)
		return 0;

	int leftCount = RecursiveSize(p->left);
	int rightCount = RecursiveSize(p->right);

	return 1 + leftCount + rightCount;

	//It recursively traverses the BST, counting the number of nodes in the left and right subtrees,
	//and adds 1 to include the current node. The method returns the sum of the counts.
}

int BSTSet::size()
{
	return RecursiveSize(root);

	//O(n)
	//S(1)
}


int BSTSet::RecursiveHeight(TNode* p)
{
	if (p == NULL)
		return -1;							//If the node is NULL, the height is defined as -1

	int lHeight = RecursiveHeight(p->left);	
	int RHeight = RecursiveHeight(p->right);

	//function calls itself recursively on the left and right subtrees of the node and takes the maximum height of the two subtrees

	return max(lHeight, RHeight) + 1; //standard library function to return the maximum of the two subtrees and add one to get the height of the current node
}

int BSTSet::height()
{
	return RecursiveHeight(root);

	//O(n)
	//S(1)
}


void BSTSet::printNonRec() {
	if (root == NULL)
		return;					//returns if root is NULL

	MyStack S;				//initialize a MyStack object
	TNode* current = root;		//create pointer called current and set to the root

	while (current != NULL || !S.isEmpty()) {	//traverse structure while current isn't null and stack isn't empty
		if (current != NULL) {
			S.push(current);					//push current value onto the stack
			current = current->left;				//move current to next node
		}
		else {
			current = S.pop();					
			cout << current->element << ",";
			current = current->right;

			//pops top node from stack, prints its value, and sets current to point to the popped node's right child.
		}
	}

	//O(n)
	//S(1)
}

//do not change these functions

void BSTSet::printBSTSet()
{
	if (root == NULL)
		std::cout << "";
	else {
		printBSTSet(root);
	}
}

void BSTSet::printBSTSet(TNode* t)
{
	if (t != NULL) {
		printBSTSet(t->left);
		std::cout << t->element << ",";
		printBSTSet(t->right);
	}
}