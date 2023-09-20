#include "HugeInteger.h"
#include <vector>
#include <string>
#include <random>

using namespace std;

HugeInteger::HugeInteger(const std::string& val) {

	isNegative = 0;                                                 //flag for isNegativeative integer
	int i = 0;
	calls = false;

	if (val.empty()) {
		throw invalid_argument("Please enter only an integer");     //throw invalid argument if the string val is empty (using standard library)
	}

	if (val[0] == '-') {                                            //if the first element in the string is a negative sign then set flag to 1
		isNegative = 1;
		i++;
	}

	while (i < val.length()) {                                      //run while loop until the length. If the first item is a negative sign then the first element is "skipped" since i would have been incremented by 1
		if (val[i] >= 48 && val[i] <= 57) {                         //check if the element is actually between 0 to 9
			intList.push_back(int(val[i]) - 48);                    //add element to the vector intList (defined in header) 
			i++;
		}
		else {
			throw invalid_argument("Invalid character in HugeInteger string");
		}
	}
}


HugeInteger::HugeInteger(int n) {

	calls = false;

	if (n < 1) {
		throw invalid_argument("Please enter a positive digit count");
	}

	int firstDigit = (rand() % 3) + 1;           //use rand function and modulo with 3 (but you could chose any number). If the number was fully divisible by 3, then the result of the modulus would be 0 and you'd add 1 to the random number. This makes sure the first digit isn't 0
	intList.push_back(firstDigit);               //push to the intList vector                           

	for (int i = 1; i < n; i++) {                //keep adding random integers to intList until size n. Modulo 10 is done to make sure only one digit is added at a time
		intList.push_back(rand() % 10);
	}
}

HugeInteger::HugeInteger(std::vector<int> intList, bool negative) {

	this->intList = intList;
	this->isNegative = negative;
	calls = false;
}

HugeInteger::HugeInteger(std::vector<int> intList) {

	this->intList = intList;
	this->isNegative = false;
	calls = false;
}


HugeInteger HugeInteger::add(const HugeInteger& h) {

	string finalString = "";
	
	int count = 0;
	int carry = 0;
	int sum = 0;
	
	int tSize;

	vector<int> t1;
	vector<int> t2;


	if (h.intList.size() == 1 && (h.intList.at(0) == 0)) {
		HugeInteger H = HugeInteger(this->intList);
		H.isNegative = this->isNegative;
		return H;
	}

	if (!this->isNegative && h.isNegative) {
		HugeInteger t1 = h;
		t1.isNegative = false;
		return subtract(t1);
	}

	if (this->intList.size() == 1 && this->intList.at(0) == 0) {
		HugeInteger H = h;
		H.isNegative = h.isNegative;
		return H;
	}

	int big = 0, small = 0;

	if (h.intList.size() > this->intList.size()) {
		t1 = h.intList;
		t2 = this->intList;
		tSize = this->intList.size();

		big = h.intList.size();
		small = this->intList.size();
	}
	else if (h.intList.size() < this->intList.size()) {
		t1 = this->intList;
		t2 = h.intList;
		tSize = h.intList.size();

		big = this->intList.size();
		small = h.intList.size();

	}
	else {
		t1 = this->intList;
		t2 = h.intList;
		tSize = this->intList.size();

		big = this->intList.size();
		small = big;
	}

	big--;
	small--;

	while (big >= 0) {
		
		if (count < tSize) {
			sum = t1[big] + t2[small] + carry;
			carry = 0;
		}
		else {
			sum = t1[big] + carry;
			carry = 0;
		}

		if (sum >= 10) {
			sum = sum - 10;
			carry = 1;
		}

		finalString = to_string(sum) + finalString;
		count++;
		
		big--;
		small--;
	}

	if ((this->isNegative) && !(h.isNegative) && (!this->calls)) {
		HugeInteger t1(this->toString());
		t1.isNegative = false;
		HugeInteger t2 = t1.subtract(h);
		
		if (t2.toString() != "0") {
			t2.makeNegative();
		}
		return t2;
	}

	if (carry == 1) {
		finalString = to_string(carry) + finalString;
		carry = 0;
	}

	if (this->isNegative && h.isNegative) 
		finalString = "-" + finalString;
	else 
		this->isNegative = false;


	return HugeInteger(finalString);
}

void HugeInteger::makeNegative() {
	this->isNegative = !this->isNegative;
}


HugeInteger HugeInteger::subtract(const HugeInteger& h) {
	string finalString = "";
	HugeInteger caseReturn = h;

	HugeInteger secondObj(this->intList, this->isNegative);

	if (this->isNegative && !(h.isNegative)) {
		this->calls = true;
		HugeInteger caseReturn = h;
		caseReturn = this->add(caseReturn);
		
		caseReturn.isNegative = true;
		return caseReturn;
	}

	if ((h.isNegative) && !this->isNegative) {
		HugeInteger caseReturn = h;
		caseReturn.isNegative = false;
		return this->add(caseReturn);
	}

	if (this->compareTo(h) == 0) {
		return HugeInteger("0");
	}

	if (this->intList.size() == 1 && (this->intList.at(0) == 0)) {
		caseReturn.makeNegative();
		return caseReturn;
	}

	if (h.intList.size() == 1 && (h.intList.at(0) == 0)) {
		caseReturn = HugeInteger(this->intList);
		caseReturn.isNegative = this->isNegative;
		return caseReturn;
	}

	int size1 = this->intList.size() - 1;
	int size2 = h.intList.size() - 1;

	HugeInteger firstObject = h;
	while (size1 >= 0 || size2 >= 0) {
		int temp = 0;
		if (size1 >= size2) {
			if (size2 >= 0) {
				temp = this->intList.at(size1) - h.intList.at(size2);
			}
			if (size2 < 0 && size1 >= 0) {
				temp = this->intList.at(size1);
			}

			if (size2 >= 0 && size1 < 0) {
				temp = h.intList.at(size2);
			}
			if (temp < 0) {
				temp += 10;
				if (size1 >= 1) {
					intList[size1 - 1] = intList[size1 - 1] - 1;
				}

			}
		}
		else {
			if (size2 >= 0 && size1 >= 0) {
				temp = firstObject.intList.at(size2) - this->intList.at(size1);
			}
			if (size2 < 0 && size1 >= 0) {
				temp = firstObject.intList.at(size1);

			}
			if (size2 >= 0 && size1 < 0) {
				temp = firstObject.intList.at(size2);

			}
			if (temp < 0) {
				temp += 10;
				if (size2 >= 1) {
					firstObject.intList[size2 - 1] = firstObject.intList[size2 - 1] - 1;
				}
			}
		}

		finalString = to_string(temp) + finalString;

		temp = 0;
		size1--;
		size2--;
	}

	if (secondObj.compareTo(firstObject) == -1) {
		firstObject.isNegative = false;
		secondObj.isNegative = false;
		if (firstObject.intList.size() > secondObj.intList.size() && firstObject.compareTo(secondObj) == 1) {
			finalString = '-' + finalString;

		}
		else {
			finalString = "+" + finalString;
		}

	}
	else if (secondObj.compareTo(firstObject) == 1) {

		firstObject.isNegative = false;
		secondObj.isNegative = false;
		if (firstObject.intList.size() > secondObj.intList.size() && firstObject.compareTo(secondObj) == 1) {
			finalString = "+" + finalString;
		}
		else {
			finalString = '-' + finalString;
		}
	}

	return HugeInteger(finalString);
}

HugeInteger HugeInteger::multiply(const HugeInteger& h) {

	string product = "";					//string which returns the product
	int size1 = intList.size();				//size of h and HugeInteger
	int size2 = h.intList.size();

	vector <int> temp(size1 + size2, 0);					//vector of type int with a size of "size1 + size2" and initialize its elements to 0

	int holding = 0;										//holds intermediate values
	int carry = 0;											//for carrys
	int count1 = 0;
	int count2 = 0;											//count1 and count2 for indexing the values in the temp vector



	if (h.intList[0] == 0 || intList[0] == 0) {				//if either integers are zero, then return 0
		return HugeInteger("0");
	}

	for (int i = (intList.size() - 1); i >= 0; i--) {		//Loop through each element of the HugeInteger starting from the rightmost element to the leftmost element
		carry = 0;
		count1 = 0;

		for (int j = (h.intList.size() - 1); j >= 0; j--) {								//loop through h
			holding = intList[i] * h.intList[j] + temp[count1 + count2] + carry;		//preform the multiplication element by element, along with any carry values from the previous multiplication, and store the result in holding
			temp[(count1 + count2)] = holding % 10;										//store the least significant digit of hold in the temp vector
			carry = holding / 10;														//put most significant in the carry value for the next multiplication
			count1++;
		}
		if (carry > 0) {									//Check for carry from last multiplication, if yes -> add it to the next element of temp
			temp[count1 + count2] += carry;
		}
		count2++;
	}

	int a = temp.size() - 1;								//size of temp

	//right now, temp is storing the reverse of the actual product of h and HugeInteger
	//we want to remove any leading zeroes

	for (a; a >= 0 && temp[a] == 0; a--) {}

	// Once the loop finds the first non-zero element in temp, iteration stops and the index of the last zero element is returned
	//(or -1 if there were no zero elements in the vector temp).

	if (a == -1) {
		return 0;
	}
	for (a; a >= 0; a--) {
		product += to_string(temp[a]);		//store the contents of temp in product in the correct order
	}



	//If either h or HugeInteger is negative, put a "-" sign in front of the string 
	if (!isNegative && h.isNegative) {
		product = "-" + product;
	}
	else if (!h.isNegative && isNegative) {
		product = "-" + product;
	}

	return HugeInteger(product);
}

int HugeInteger::compareTo(const HugeInteger& h) {

	int check = 0;

	if (intList[0] == 0 && h.intList[0] == 0) {			//if both elements are zero, then return 0
		return 0;
	}

	if (!h.isNegative && isNegative) {					//the next two statements check for negatives. If one is negative and one is positive, then the according value is returned
		return -1;										//HugeInteger < h
	}
	else if (h.isNegative && !isNegative) {
		return 1;										//HugeInteger > h
	}


	else if (!h.isNegative && !isNegative) {			//if both h and Huge integer are positive
		if (intList.size() > h.intList.size()) {
			return 1;									//if HugeInteger > h
		}
		else if (intList.size() < h.intList.size()) {
			return -1;									//if HugeInteger < h
		}
		else {

			//If the lists are of equal length, it iterates through the lists from the end to the beginning and compares the corresponding digits.
			//If the digit in the first object is less than the digit in the second object, it returns -1, and vice versa.

			for (int i = intList.size() - 1; i >= 0; i--) {
				if (intList[i] < h.intList[i]) {
					return -1;
				}
				else if (intList[i] > h.intList[i]) {
					return 1;
				}
			}
		}
	}
	else if (h.isNegative && isNegative) {				//if both h and Huge integer are negative, same procedure as above is executed
		if (intList.size() > h.intList.size()) {
			return -1;
		}
		else if (intList.size() < h.intList.size()) {
			return 1;
		}
		else {
			for (int i = intList.size() - 1; i >= 0; i--) {
				if (intList[i] < h.intList[i]) {
					return 1;
				}
				else if (intList[i] > h.intList[i]) {
					return -1;
				}
			}
		}
	}

	return check;
}

std::string HugeInteger::toString() {

	int vectorSize = intList.size();                //find size of intList
	string intString = "";                          //define empty string that is to be filled and returned


	for (int i = 0; i < vectorSize; i++) {          //iterate through each element of the vector
		intString += to_string(intList[i]);         //add the element to the intString
	}

	if (isNegative == 1) {                          //if the flag from the constructor function flagged the string to have a negative sign at element 1 then add a '-' to the beginning of intList
		intString = '-' + intString;
	}
	return intString;
}