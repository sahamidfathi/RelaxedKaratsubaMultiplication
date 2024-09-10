#ifndef TPS_H
#define TPS_H

#include <gmp.h>

class TPS {
	
	public:
		
		// a constructor that takes two int arrays: numerators and 
		// denominators, and an array size.
		TPS(int* numerator, int* denominator, int arraySize);
		
		// a constructor that takes an mpq_t array.
		// new memory is allocated and contents of passed argument is coppied
		// because TPS will only deallocate memory (in its destructor) 
		// allocated by itself.
		TPS(mpq_t* coeffArray, int arraySize);
		
		// default constructor
		TPS() : k(0) {}
		
		// deallocating coefficientArray
		~TPS();
		
		// Call ensureOrder() before setElement()
		void setElement(int index, const mpq_t &value);
		
		// a function that allocates more space for coefficientArray if
		// necessay.
		// It is impossible that kNew > 2k since kNew is at most k + 1
		// (because in the next method, terms are added one by one).
		// TODO a better way than coppying, deallocating, allocating ...
		// maybe realloc in C.
		void ensureOrder(int kNew);

		// overloading +=
		TPS& operator+= (TPS& other);

		// overloading *=
		TPS& operator*= (TPS& other);

		// print the whole TPS in the form of 1+2x+3x^2+...
		// FOR THE MOMENT, VARIABLE IS X
		void printTPS();
		
		void setCoefficientArray(mpq_t* coeffArr) {
			coefficientArray = coeffArr;
		}
		
		int getArraySize() const {
			return k;	
		}

		mpq_t* getCoefficientArray() const {
			return coefficientArray;
		}
		
	protected:
		// k is the size of the array, regardless of being empty or not.
		int k; 
		
		// initialize to nullptr to be able to delete it whether or not newed.
		mpq_t *coefficientArray = nullptr; 
};

#endif // TPS_H
