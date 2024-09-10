#include <iostream>
#include "../include/TPS.h"

TPS::TPS(int* numerator, int* denominator, int arraySize) : k(arraySize)
{
	coefficientArray = new mpq_t[k];
	for (int i = 0; i < k; ++i) {
		mpq_init(coefficientArray[i]);
		mpq_set_si(coefficientArray[i], numerator[i], denominator[i]);
	}
}

TPS::TPS(mpq_t* coeffArray, int arraySize) : k(arraySize)
{
	coefficientArray = new mpq_t[k];
	for (int i = 0; i < k; ++i) {
		mpq_init(coefficientArray[i]);
		mpq_set(coefficientArray[i], coeffArray[i]);
	}
}

TPS::~TPS() {	
	for (int i = 0; i < k; ++i)
		mpq_clear(coefficientArray[i]);
				
	delete[] coefficientArray;
	coefficientArray = nullptr;
}

void TPS::setElement(int index, const mpq_t &value) {
	mpq_set(coefficientArray[index], value);
}

void TPS::ensureOrder(int kNew) {
	if (kNew > k) {
		int kOld = k;
		mpq_t* tempArray = new mpq_t[k];
	
		// deallocate previous array
		for (int i = 0; i < k; ++i) {
			mpq_init(tempArray[i]);
			mpq_set(tempArray[i], coefficientArray[i]);
			mpq_clear(coefficientArray[i]);
		}
		delete[] coefficientArray;
		coefficientArray = nullptr;
				
		k = kNew;
				
		// create new array
		coefficientArray = new mpq_t[k];
		for (int i = 0; i < kOld; ++i) {
			mpq_init(coefficientArray[i]);
			mpq_set(coefficientArray[i], tempArray[i]);
			mpq_clear(tempArray[i]);
		}
		for (int i = kOld; i < k; ++i) {
			mpq_init(coefficientArray[i]);
		}
		delete[] tempArray;
		tempArray = nullptr;	
	}
}

TPS& TPS::operator+= (TPS& other) {
	if (k != other.k) {
		std::cerr << "Error: Incompatible truncated power series!";
		exit(1);	
	}
	for (int i = 0; i < k; ++i) {
		mpq_add(coefficientArray[i], coefficientArray[i], 
			other.coefficientArray[i]);			
	}
	return *this;		
}

TPS& TPS::operator*= (TPS& other) {
	mpq_t product;
	mpq_init(product);

	mpq_t productCoefficientArray[k];
	for (int i = 0; i < k; ++i)
		mpq_init(productCoefficientArray[i]);

	for (int i = 0; i < k; ++i) {
		for (int j = 0; j < k; ++j) {
			if (i + j < k) {
				mpq_mul(product, coefficientArray[i],
					       	other.coefficientArray[j]);
				mpq_add(productCoefficientArray[i + j],
					       	productCoefficientArray[i + j],
					       	product);
			}
		}	
	}
		
	mpq_clear(product);

	for (int i = 0; i < k; ++i) { 
		mpq_set(coefficientArray[i], productCoefficientArray[i]);	
		mpq_clear(productCoefficientArray[i]);	
	}

	return *this;	
}

void TPS::printTPS() {	
	char var = 'z';
	for (int i = 0; i < k; ++i) {
		// first deciding the sign
		if (mpq_sgn(coefficientArray[i]) > 0 && i != 0) {
			std::cout << "+";
		}

		if (mpq_sgn(coefficientArray[i]) == 0) {
			continue;	
		}

		mpq_out_str(stdout, 10, coefficientArray[i]);
		if (i == 0) {} else if (i == 1) {std::cout << var;}
		else {std::cout << var << "^" << i;}
	}
	std::cout << "\n";
}

