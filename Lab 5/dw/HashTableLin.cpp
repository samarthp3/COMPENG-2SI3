#include "HashTableLin.h"
#include <iostream>
using namespace std;

HashTableLin::HashTableLin(int maxNum, double load)
{
    // TODO
	LOAD_MAX = load;
	keyCount = 0;
	size =(maxNum/load)+0.9;

	bool prime = false;
	if(size>3){
		while(!prime){
			if (size % 2 == 0 || size % 3 == 0){
				size++;
				continue;
			}
			prime = true;
			for (int i = 5; i * i <= size; i = i + 6){
				if (size % i == 0 || size % (i + 2) == 0){
					size++;
					prime = false;
					break;
				}
			}
		}
	}
	for(int i=0; i<size;i++){
		table.push_back(0);
	}
}

void HashTableLin::insert(int n)
{
    // TODO
	if(isIn(n)){return;}
	double load = (keyCount+1.0)/size;
	if(load > LOAD_MAX){
		rehash();
	}

	int i = n%size;

	if(table[i]==0){
		table[i] = n;
		keyCount++;
	}
	else{
		while(table[i]!=0){
			i = (i+1)%size;
		}
		table[i]=n;
		keyCount++;
	}
}

void HashTableLin::rehash()
{
    // TODO
	int temp = size;
	size = size*2;
	double maxNum = size*LOAD_MAX;
	HashTableLin bigHash = HashTableLin(maxNum, LOAD_MAX);

	for(int i=0;i<temp;i++){
		if(table[i]!=0){
			bigHash.insert(table[i]);
		}
	}
	table = bigHash.table;
	size = bigHash.size;
	keyCount = bigHash.keyCount;
}

bool HashTableLin::isIn(int n)
{
    // TODO, change following code after completing this function
	int i = n%size;
	if(table[i] != n){
		i = (i+1)%size;
		while(i != n%size && table[i]!=0){
			if(table[i] == n){return true;}
			i = (i+1)%size;
		}
		return false;
	}

    return true;
}

void HashTableLin::printKeys()
{
    // TODO
	for(int i=0;i<size;i++){
		if(table[i]!=0){
			std::cout<<table[i]<<',';
		}
	}
}

void HashTableLin::printKeysAndIndexes()
{
    // TODO
    for(int i=0;i<size;i++){
		if(table[i]!=0){
			std::cout<<'\n'<<"Index: "<<i<<'\t'<<"Value: " << table[i];
		}
	}
}

int HashTableLin::getNumKeys() {
	// TODO, change following code after completing this function
    return keyCount;
}

int HashTableLin::getTableSize() {
	// TODO, change following code after completing this function
    return size;
}

double HashTableLin::getMaxLoadFactor() {
	// TODO, change following code after completing this function
    return LOAD_MAX;
}

int HashTableLin::insertCount(int n) {
    // TODO, change following code after completing this function
    //int startingIndex = keyCount%size;
    int index = n%size;
    int count = 0;

    // perform insertion
    for (int i = 1; i <= size; i++) {
        // if element is empty, insert
        if (table[index] == 0) {
            count++;
            break;
        }
        // if element is not empty
        else {
            // if there is duplicate value, exit loop
            if (table[index] == n) {
                count = 0;
                break;
            }
            // if there is another value, then perform linear probing
            else {
                index = (index+1)%size;
                count++;
            }
        }
    }

    return count;
}

std::vector<double> HashTableLin::simProbeSuccess()
{
    // TODO, change following code after completing this function
    vector<double> result;
    double avg = 0;
    double real_avg = 0;
    int probes = 0;

    for (double i = 0.1; i <= 0.9; i += 0.1) {
        for (int j = 0; j < 100; j++) {
            HashTableLin table = HashTableLin(100000, i);

            while (table.keyCount != 100000) {
                probes += table.insertCount(rand());
            }

            avg += probes/100000.0;
            probes = 0;

        }
        real_avg = avg/100.0;
        avg = 0;
        result.push_back(real_avg);
    }

    return result;
}

// for bonus
std::vector<double> HashTableLin::simProbeUnsuccess()
{
    // TODO, change following code after completing this function
    vector<double> result(9);
    return result;
}
