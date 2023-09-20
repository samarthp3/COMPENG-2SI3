#include "HashTableLin.h"
#include <iostream>
#include <random>
using namespace std;

HashTableLin::HashTableLin(int maxNum, double load)
{
	int minPrime;
	loadMax = load;

	// Calculate the minimum prime number greater than or equal to maxNum/load
	minPrime = primeCheck(ceil(maxNum / load));

	// Resize the hash table to have a size of minPrime, with all entries initialized to NULL
	table.resize(minPrime, NULL);

	// Set tableSize to minPrime and noKeys to 0
	tableSize = minPrime;
	noKeys = 0;
}


int HashTableLin::primeCheck(int p) {
	bool minPrime = false;

	// Loop until a prime number is found
	while (!minPrime) {
		bool isPrime = true;

		// Check if p is a prime number
		if (p == 0 || p == 1) {
			isPrime = false;
		}
		for (int i = 2; i <= p / 2; i++) {
			if (p % i == 0) {
				isPrime = false;
				break;
			}
		}

		// If p is not a prime number, increment it and try again
		if (!isPrime) {
			p++;
		}
		else {
			minPrime = true;
			break;
		}
	}

	// Return the smallest prime number greater than or equal to the input value
	return p;
}


/*
* Run time: Average - Θ(1), Worst-Case - Θ(n)
* Space complexity: S(1)
*/

void HashTableLin::insert(int n) {
	// If the key is already in the table, do nothing
	if (isIn(n)) {
		return;
	}

	// Calculate the initial hash value for n
	int hash = n % getTableSize();

	// Probe linearly until an empty slot is found or the table is fully probed
	for (int probe = 0; table[hash] != 0 && probe < getTableSize(); probe++) {
		hash = (hash + 1) % getTableSize();
	}

	// If an empty slot is found, insert n and update the load factor
	if (table[hash] == 0) {
		table[hash] = n;
		noKeys++;

		// Check if the load factor exceeds the maximum load factor, and if so, rehash the table
		double x = (double)(getNumKeys() + 1);
		double y = getTableSize();
		if ((x / y) > getMaxLoadFactor()) {
			rehash();
		}
	}
}

/*
* Run time: Average - Θ(n)
* Space complexity: S(1)
*/
void HashTableLin::rehash()
{
	// Determine the new table size and create a copy of the old table
	int newSize = primeCheck(getTableSize() * 2);
	vector<int> tableCopy = table;

	// Create a new table of the new size with all elements initialized to 0
	vector<int> newTable(newSize, 0);

	// Set the hash table to the new table and update the table size and number of keys
	table = newTable;
	tableSize = newSize;
	noKeys = 0;

	// Reinsert each non-zero element from the old table into the new table
	int i = 0;
	while (i < tableCopy.size()) {
		if (tableCopy[i] != 0) {
			insert(tableCopy[i]);
		}
		i++;
	}
}


/*
* HashTableLin::isIn(int n)
* Run time: Average - Θ(1), Worst-Case - Θ(n)
* Space complexity: S(1)
*/

bool HashTableLin::isIn(int n) {
	// Get the current table size and check if it is zero
	int tableSize = getTableSize();
	if (tableSize == 0) {
		return false;
	}

	// Calculate the hash value for the input n using the modulo operator
	int hash = n % tableSize;

	// Probe the hash table linearly and check each slot for the input n
	for (int probe = 0; probe < tableSize && table[hash] != 0; probe++) {
		if (table[hash] == n) {
			return true;
		}
		// Calculate the next hash value using linear probing
		hash = (hash + 1) % tableSize;
	}

	// If the function has reached an empty slot or probed the entire table, return false
	return false;
}


void HashTableLin::printKeys()
{
	int i = 0;
	// Loop through the hash table and print out the keys separated by commas
	while (i < getTableSize()) {
		if (this->table[i] != 0) {
			cout << table[i] << ", ";
		}
		i++;
	}
}

void HashTableLin::printKeysAndIndexes()
{
	int i = 0;
	// Loop through the hash table and print out the keys and their indexes separated by hyphens
	while (i < getTableSize()) {
		if (this->table[i] != 0) {
			cout << i << "-";
			cout << table[i] << ", ";
		}
		i++;
	}
}


int HashTableLin::getNumKeys() {
	return noKeys;
}

int HashTableLin::getTableSize() {
	return tableSize;
}

double HashTableLin::getMaxLoadFactor() {
	return loadMax;
}


/*
* Run time: Average - Θ(1), Worst-Case - Θ(n)
* Space complexity: S(1)
*/

int HashTableLin::countInst(int n) {
	// If the key is already in the hash table, return 0
	if (isIn(n)) {
		return 0;
	}

	int hash = n % tableSize;
	int probe = 1;

	// Linear probing until an empty slot is found
	while (table[hash] != 0) {
		hash = (hash + probe) % tableSize;
		probe++;
	}

	// Insert the key and increment the number of keys in the hash table
	table[hash] = n;
	noKeys++;

	// Return the number of probes needed to insert the key
	return probe;
}


std::vector<double> HashTableLin::simProbeSuccess()
{
	// create a vector to store the final results
	vector<double> final;
	// create a variable to store the number of probes
	int proCount = 0;
	// create a variable to store the average number of probes
	double avgProbes = 0;
	// create a variable to store the total average number of probes
	double totalAvg = 0;

	// loop through different load factors from 0.1 to 0.9
	for (double i = 0.1; i < 1; i += 0.1) {
		// loop 100 times to get a better estimate of the average number of probes
		for (int j = 0; j < 100; j++) {
			// create a new hash table with a maximum of 10000 keys and the current load factor
			HashTableLin probeTable(10000, i);

			// reset the probe count
			proCount = 0;

			// create a random number generator
			default_random_engine gen;
			// create a uniform distribution over the range of integers
			uniform_int_distribution<int> distrib;

			// insert random numbers into the hash table until it reaches its maximum capacity
			while (probeTable.getNumKeys() != 10000) {
				// increment the probe count for each insertion
				proCount += probeTable.countInst(distrib(gen));
			}
			// calculate the average number of probes for this hash table
			double x = (double)proCount;
			double y = (double)probeTable.getNumKeys();
			avgProbes += (x / y);
		}
		// calculate the total average number of probes for all the hash tables with this load factor
		totalAvg = avgProbes / 100.0;
		// reset the average number of probes
		avgProbes = 0;
		// add the total average number of probes to the final vector
		final.push_back(totalAvg);
	}
	// return the vector of final results
	return final;
}


// for bonus
std::vector<double> HashTableLin::simProbeUnsuccess()
{
	// TODO, change following code after completing this function
	vector<double> result(9);
	return result;
}