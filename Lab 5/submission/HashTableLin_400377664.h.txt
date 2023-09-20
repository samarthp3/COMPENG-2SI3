#ifndef HASHTABLELIN_H_
#define HASHTABLELIN_H_

#include <vector>

class HashTableLin
{
private:
	std::vector<int> table;
	int tableSize = 0;
	int noKeys = 0;
	double loadMax = 0;

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

	int primeCheck(int n);
	int countInst(int n);

	// used for testing
	std::vector<int> getTable() {
		return table;
	};

private:
	// required methods
	void rehash();
};

#endif /* HASHTABLELIN_H_ */