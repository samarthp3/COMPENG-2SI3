#ifndef HASHTABLEQUAD_H_
#define HASHTABLEQUAD_H_

#include <vector>

class HashTableQuad
{
private:
	std::vector<int> table;
	int tableSize = 0;
	int noKeys = 0;
	double loadMax = 0;

public:
	// required constructor
	HashTableQuad(int maxNum, double load);

	// required methods
	void insert(int n);
	bool isIn(int n);
	void printKeys();
	void printKeysAndIndexes();
	int getNumKeys();
	int getTableSize();
	double getMaxLoadFactor();
	static std::vector<double> simProbeSuccess();

	// used for testing
	std::vector<int> getTable() {
		return table;
	}
	
	int primeCheck(int n);
	int countInst(int n);
	

private:
	// required methods
	void rehash();
};

#endif /* HASHTABLEQUAD_H_ */