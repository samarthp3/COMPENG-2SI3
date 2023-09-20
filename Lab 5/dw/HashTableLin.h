#ifndef HASHTABLELIN_H_
#define HASHTABLELIN_H_

#include <vector>

class HashTableLin
{
private:
	// must contain this private field
	std::vector<int> table;
	// define other private fields to store:
	// 1. the size of the table
	int size;
	// 2. the number of keys stored in the table
	int keyCount;
	// 3. the maximum load factor allowed (total # of keys stored/size of array)
	double LOAD_MAX;

public:
	// required constructor
	HashTableLin(int maxNum, double load);

	// required methods
	void insert(int n);
	bool isIn(int n);
	void printKeys();
	void printKeysAndIndexes();
	int getNumKeys();
	int getTableSize();
	double getMaxLoadFactor();
	static std::vector<double> simProbeSuccess();
	static std::vector<double> simProbeUnsuccess(); // for bonus

	// used for testing
	std::vector<int> getTable() {
		return table;
	};

private:
	// required methods
	void rehash();

	//helper methods
	int insertCount(int x);
};

#endif /* HASHTABLELIN_H_ */
