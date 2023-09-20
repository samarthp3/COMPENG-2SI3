#include "Poly.h"
using namespace std;

Poly::Poly()
{
	head = new PolyNode(-1, 0, NULL);

	//O(1) and S(1)
}

Poly::Poly(const std::vector<int>& deg, const std::vector<double>& coeff)
{
	// Create a new PolyNode object that will serve as the head of the linked list
	head = new PolyNode(-1, 0, NULL);

	// Create a pointer to the head of the linked list
	PolyNode* p = head;

	// Loop over the coefficients and degrees of each term in the polynomial
	for (int i = 0; i < coeff.size(); i++) {
		// Create a new PolyNode object for the current term
		PolyNode* newNode = new PolyNode(deg[i], coeff[i], NULL);

		// Link the new node to the previous node in the linked list
		p->next = newNode;

		// Update the pointer to the current node to the new node
		p = p->next;
	}

	// The time complexity of this constructor is O(n), where n is the number of terms in the polynomial.
	// The space complexity of this constructor is also O(n), since it creates a linked list of n nodes.
}

Poly::~Poly()
{
	// Create two pointers to the head of the linked list
	PolyNode* p = head;
	PolyNode* tmp = head;

	// Loop over the linked list and delete each node
	while (p->next != NULL) {
		// Update the pointer to the current node and delete the previous node
		p = p->next;
		delete tmp;

		// Update the pointer to the previous node to the current node
		tmp = p;
	}

	// Delete the final node in the linked list
	delete p;

	//time complexity of O(n) and S(1)
}

void Poly::addMono(int i, double c)
{
	// If the coefficient is 0, return immediately
	if (c == 0)
		return;

	// Create two pointers to the head of the linked list
	PolyNode* tmp;
	PolyNode* p = head;

	// Loop over the linked list to find the correct location for the new term
	for (p; p->next != NULL; p = p->next) {
		// If a term with the same degree already exists, add the new coefficient to it
		if (i == p->next->deg) {
			p->next->coeff += c;

			// If the new coefficient is 0, remove the term from the polynomial
			if (p->next->coeff == 0) {
				tmp = p->next;
				p->next = p->next->next;
				delete tmp;
			}
			return;
		}
		// If the new term should be inserted before the current node, create a new node and insert it
		else if (i > p->next->deg) {
			PolyNode* x = new PolyNode(i, c, p->next);
			p->next = x;
			return;
		}
	}

	// If the new term should be inserted at the end of the linked list, create a new node and append it
	p->next = new PolyNode(i, c, NULL);
	return;

	//O(n) and S(1)

}

void Poly::addPoly(const Poly& p)
{
	PolyNode* q = p.head;  // create a pointer "q" to point to the head of the linked list representation of the polynomial "p"

	for (q; q->next; q = q->next) {  // iterate over each node in the linked list representation of "p", starting from the head and stopping at the second-to-last node
		addMono(q->next->deg, q->next->coeff);  // call the "addMono" function to add the monomial with the given degree and coefficient to the polynomial
	}

	//O(n^2) and S(1)
}

void Poly::multiplyMono(int i, double c)
{
	PolyNode* p = head;  // initialize a pointer "p" to point to the head of the linked list representation of the polynomial

	// if coeff is 0 delete all nodes except head and point head to NULL
	if (c == 0 && getDegree() != -1) {  // if the coefficient "c" is 0 and the polynomial is not already empty...
		p = p->next;  // set "p" to point to the second node in the linked list
		PolyNode* tmp = p;  // create a temporary pointer "tmp" to point to the second node

		for (p; p->next != NULL; tmp = p) {  // iterate over each node in the linked list, starting from the second node and stopping at the last node
			p = p->next;  // move "p" to point to the next node in the linked list
			delete tmp;  // delete the node pointed to by "tmp"
		}
		delete p;  // delete the last node in the linked list
		head->next = NULL;  // set the "next" pointer of the head node to NULL, effectively deleting the polynomial
		return;  // exit the function early
	}

	for (p; p->next != NULL; p = p->next) {  // iterate over each node in the linked list, starting from the head node and stopping at the second-to-last node
		p->next->deg += i;  // add the given degree "i" to the degree of the next node in the linked list
		p->next->coeff *= c;  // multiply the coefficient of the next node in the linked list by the given coefficient "c"
	}

	//O(n) and S(1)
}


void Poly::multiplyPoly(const Poly& p)
{
	// Check if this polynomial is 0
	if (getDegree() == -1)
		return;

	// Create a new polynomial object "multiplier" and duplicate the current polynomial into it
	Poly* multiplier = new Poly();
	duplicate(*multiplier);

	// Create two more polynomial objects "tmp" and "product" to store intermediate values and the final product
	Poly* tmp = new Poly();
	Poly* product = new Poly();

	// Create a pointer "q" to point to the head of the linked list representation of the polynomial "p"
	PolyNode* q = p.head;

	while (q->next != NULL) {
		// Multiply the current term of p with this polynomial
		// Duplicate the multiplier into the temporary polynomial "tmp" and multiply it with the monomial specified by the degree and coefficient of the current term of p
		multiplier->duplicate(*tmp);
		tmp->multiplyMono(q->next->deg, q->next->coeff);

		// Add the resulting product to the overall product
		product->addPoly(*tmp);
		q = q->next;
	}

	// Update the polynomial by setting it equal to the product
	this->head = product->head;

	// Comment explaining why we create new Poly objects for the multiplier, temp, and product, rather than reusing the Poly object of the polynomial itself.
	// This avoids modifying the polynomial while multiplying it with p. We then iterate through the terms of p, multiplying each term with the multiplier polynomial and adding the resulting product to the product polynomial. Finally, we update the polynomial by setting its head to the head of the product polynomial.
	// Comment indicating the time complexity of the function is O(n^3) and the space complexity is S(n^2)
	// where "n" is the maximum degree of the two input polynomials.
	// This is because we iterate through each term of the input polynomial "p", and for each term, we create and duplicate two polynomial objects, one for the multiplier and one for the temporary product, and call the "addPoly" and "multiplyMono" functions, each of which iterate through the polynomial. Therefore, the overall time complexity is O(n^3).
	// The space complexity is also O(n^2) because we create up to three polynomial objects of size "n" each (i.e., "multiplier", "tmp", and "product").

	//O(n^3) and S(n^2)
}


void Poly::duplicate(Poly& outputPoly)
{
	PolyNode* p = head, * q = outputPoly.head, * prev = q;
// Iterate through each node in the current polynomial
	while (p->next) {
	// If the output polynomial has a node for the current degree, copy the coefficient and degree
	if (q->next) {
		q->next->coeff = p->next->coeff;
		q->next->deg = p->next->deg;
	}
	// If the output polynomial does not have a node for the current degree, create a new node and insert it
	else {
		q->next = new PolyNode(p->next->deg, p->next->coeff, NULL);
	}
	// Move to the next node in the current and output polynomials
	p = p->next;
	prev = q;
	q = q->next;

	}

	// If the output polynomial has more nodes than the current polynomial, delete the extra nodes
	if (q->next) {
	PolyNode* tmp;
	
	while (q->next) {
		tmp = q->next;
		q->next = q->next->next;
		delete tmp;
	}
	// Set the next pointer of the last node in the output polynomial to NULL
	prev->next = NULL;
}

// Time complexity: O(n)
// Space complexity: O(n)
}


int Poly::getDegree()
{
	// If the polynomial is empty, return the degree of the head node
	if (head->next == NULL) {
		return head->deg;
	}
	// Otherwise, return the degree of the first term in the polynomial
	return head->next->deg;

	// This function has time complexity O(1) and space complexity S(1)
}


int Poly::getTermsNo()
{
	// Initialize a pointer to the first term of the polynomial and a counter for the number of terms
	PolyNode* p = head->next;
	int term = 0;

	// Loop through each term of the polynomial and increment the counter
	while (p != NULL) {
		term++;
		p = p->next;
	}
	// Return the number of terms in the polynomial
	return term;

	// This function has time complexity O(n) and space complexity S(1)
}

double Poly::evaluate(double x)
{
	// TODO
	double total = 0;
	double count;

	// loop through each term in the polynomial
	for (PolyNode* p = head->next; p != NULL; p = p->next) {

		count = 1; // initialize count to 1
		// calculate the value of x raised to the degree of the term
		for (int i = 0; i < p->deg; i++) {
			count *= x;
		}
		// multiply the result by the coefficient of the term and add to total
		count *= p->coeff;
		total += count;
	}
	// return the final total
	return total;

	// O(n) and S(1)
}

string Poly::toString()
{
	// Initialize an empty output string and a pointer to the second element of the linked list
	string output = "";
	PolyNode* p = head->next;

	// Add the degree of the polynomial to the output string
	output += "degree=" + to_string(getDegree()) + ";";

	// If the polynomial is empty, add the string "a(-1)=0;" to the output string
	if (p == NULL) {
		output += " a(-1)=0;";
	}

	// Loop through each term of the polynomial and add its string representation to the output string
	for (int i = 0; i < getTermsNo(); i++, p = p->next) {
		output += " a(" + to_string(p->deg) + ")=" + to_string(p->coeff) + ";";
	}

	// Return the output string
	return output;

	//O(n) and S(1)
}
