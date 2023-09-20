#ifndef BSTSET_H_
#define BSTSET_H_

#include "TNode.h"
#include <vector>
#include <cstddef>

using namespace std;

class BSTSet {
private:
	TNode* root = NULL;

public:
	BSTSet();
	BSTSet(const vector<int>& input);
	~BSTSet();

	bool isIn(int v);
	void add(int v);
	bool remove(int v);
	void Union(const BSTSet& s);
	void intersection(const BSTSet& s);
	void difference(const BSTSet& s);
	int size();
	int height();
	void printNonRec();

	void printBSTSet();

	TNode* getRoot()
	{
		return root;
	}

	bool RecursiveIsIn(int v, TNode*& p);
	TNode* getMaximum(TNode*& p);
	int RecursiveSize(TNode* p);
	void Delete(TNode*& p);

	void RecursiveDifference(TNode* p);
	int RecursiveHeight(TNode* p);
	void printBSTSet(TNode* t);

	void RecursiveAdd(int v, TNode*& p);
	bool RecursiveRemove(int v, TNode*& p);

	void RecursiveUnion(const BSTSet& s, TNode* p);
	void RecursiveIntersection(TNode* p, BSTSet* t);

};

#endif /* BSTSET_H_ */