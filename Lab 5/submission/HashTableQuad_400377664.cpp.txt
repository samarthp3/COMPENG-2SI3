#include "HashTableQuad.h"
#include <iostream>
#include <random>
using namespace std;

HashTableQuad::HashTableQuad(int maxNum, double load)
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

int HashTableQuad::primeCheck(int p) {
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

void HashTableQuad::insert(int n)
{
	// Check if n is already in the table
	if (isIn(n) == NULL) {
		// Compute the hash value for n
		int hash = n % getTableSize();
		// Keep track of the number of probes
		int probe = 0;

		// Linear probing
		while (table[hash] != 0) {
			// Compute the next hash value using quadratic probing
			++probe;
			hash = (n + (probe * probe)) % getTableSize();

			// If the number of probes exceeds the table size, exit the loop
			if (probe > getTableSize()) {
				return;
			}
		}

		// Insert n into the table
		table[hash] = n;
		noKeys++;

		// Check if the load factor exceeds the maximum load factor, and rehash if necessary
		double keys = (double)getNumKeys() + 1;
		double size = (double)getTableSize();
		if ((keys / size) > getMaxLoadFactor()) {
			rehash();
		}
	}
}

void HashTableQuad::rehash()
{
	// Get new table size by doubling the current size and finding the next prime number
	int newSize = primeCheck(getTableSize() * 2);

	// Copy the contents of the current table to a new vector
	vector<int> tableCopy = table;
	// Create a new table with the new size, initialized with 0s
	vector<int> newTable(newSize, 0);

	// Replace the current table with the new one, and update the table size and number of keys
	table = newTable;
	tableSize = newSize;
	noKeys = 0;

	// Re-insert the keys from the old table into the new table
	int i = 0;
	while (i < tableCopy.size()) {
		if (tableCopy[i] != 0) {
			insert(tableCopy[i]);
		}
		++i;
	}
}


// This function checks if the given integer is present in the hash table.
// It uses quadratic probing to resolve collisions.
// It returns true if the integer is present, false otherwise.
bool HashTableQuad::isIn(int n)
{
	// calculate the hash value for the integer
	int hash = n % getTableSize();

	// loop through the table using quadratic probing to resolve collisions
	for (int probe = 0; table[hash] != 0 && probe <= getTableSize(); probe++) {

		// if the integer is found, return true
		if (table[hash] == n) {
			return true;
		}

		// calculate the next hash value using quadratic probing
		hash = (n + (probe * probe)) % getTableSize();
	}

	// the integer is not found, return false
	return false;
}

// This function prints all the keys in the hash table.
void HashTableQuad::printKeys()
{
	// loop through the table and print the keys that are not 0
	int i = 0;
	while (i < getTableSize()) {
		if (table[i] != 0) {
			cout << table[i] << ", ";
		}
		i++;
	}
}

// This function prints all the keys and their corresponding indexes in the hash table.
void HashTableQuad::printKeysAndIndexes()
{
	// loop through the table and print the keys and their indexes that are not 0
	int i = 0;
	while (i < getTableSize()) {
		if (table[i] != 0) {
			cout << i << "-" << table[i] << ", ";
		}
		i++;
	}
}


int HashTableQuad::getNumKeys() {
	return this->noKeys;
}

int HashTableQuad::getTableSize() {
	return this->tableSize;
}

double HashTableQuad::getMaxLoadFactor() {
	return this->loadMax;
}

// This function takes an integer n and returns the number of probes required to insert it into the hash table
// If the element is already in the hash table, the function returns 0
// If the function can't find a free spot to insert the element after probing the entire table, it returns -1

int HashTableQuad::countInst(int n) {
	// Initialize the probe number and hash value for the given element
	int probe = 1;
	int hash = n % getTableSize();

	// Keep probing the table until a free spot is found or the entire table is probed
	while (table[hash] != 0) {
		// If the element is already in the hash table, return 0
		if (table[hash] == n) {
			return 0;
		}

		// Increment the probe number and update the hash value
		probe++;
		hash = (n + (probe * probe)) % getTableSize();

		// If the probe number exceeds the size of the table, return -1
		if (probe > getTableSize()) {
			return -1;
		}
	}

	// If a free spot is found, insert the element into the hash table and increment the number of keys in the table
	table[hash] = n;
	noKeys++;

	// Return the number of probes required to insert the element
	return probe;
}

// This function performs a simulation of probing for keys in the hash table with quadratic probing
// The function returns a vector of doubles containing the average number of probes required for various load factors
std::vector<double> HashTableQuad::simProbeSuccess()
{
	// Initialize variables for keeping track of the number of probes and the average number of probes
	vector<double> final;
	int proCount = 0;
	double avgProbes = 0;
	double totalAvg = 0;

	// Loop through different load factors from 0.1 to 0.9 in increments of 0.1
	for (double i = 0.1; i < 1; i += 0.1) {
		// Perform 100 simulations for each load factor
		for (int j = 0; j < 100; j++) {

			// Create a new hash table with 10000 slots and the current load factor
			HashTableQuad probeTable(10000, i);

			// Reset the probe count and generate a random integer using the default random engine and uniform distribution
			proCount = 0;
			default_random_engine gen;
			uniform_int_distribution<int> distrib;

			// Keep inserting random integers into the hash table until all slots are filled
			while (probeTable.getNumKeys() != 10000) {
				proCount += probeTable.countInst(distrib(gen));
			}

			// Calculate the average number of probes required to insert all the keys into the hash table
			double x = (double)proCount;
			double y = (double)probeTable.getNumKeys();
			avgProbes += (x / y);
		}

		// Calculate the average number of probes required for the current load factor and add it to the final vector
		totalAvg = avgProbes / 100.0;
		avgProbes = 0;
		final.push_back(totalAvg);
	}

	// Return the vector containing the average number of probes for various load factors
	return final;
}