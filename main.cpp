#include <iostream>
#include <gmp.h>
#include <chrono>
#include <math.h> 

#include "TPS.h"
#include "Series_Rep.h"
#include "Series.h"
#include "Product1_Series_Rep.h"
#include "Series1.h"
#include "Product2_Series_Rep.h"
#include "Series2.h"

// PS coefficient generator function f0 (n=0), f1 (n=1), ...
//  1 + 2x + 3x^2 + 4x^3 + ...
void PSCG1(int n, int* numerator, int* denominator) {
	*numerator = (n + 1);
	*denominator = 1;
}

void PSCG2(int n, int* numerator, int* denominator) {
	*numerator = 2 * n;
	*denominator = 1;
}

void customPSCG1(int n, int* numerator, int* denominator) {
	switch (n) {
		case 0:
			*numerator = 1;
			*denominator = 1;
			break;
			
		case 5:
			*numerator = 1;
			*denominator = 1;
			break;
		
		case 7:
			*numerator = 1;
			*denominator = 1;
			break;

		case 10:
			*numerator = 1;
			*denominator = 1;
			break;
			
		case 12:
			*numerator = 2;
			*denominator = 1;
			break;
			
		case 14:
			*numerator = 1;
			*denominator = 1;
			break;
		
		default:
			*numerator = 0;
			*denominator = 1;
	}
}

void customPSCG2(int n, int* numerator, int* denominator) {
	switch (n) {
		case 0:
			*numerator = 1;
			*denominator = 1;
			break;
			
		case 7:
			*numerator = 1;
			*denominator = 1;
			break;
		
		case 11:
			*numerator = 1;
			*denominator = 1;
			break;

		case 14:
			*numerator = 1;
			*denominator = 1;
			break;
			
		case 18:
			*numerator = 2;
			*denominator = 1;
			break;
			
		case 22:
			*numerator = 1;
			*denominator = 1;
			break;
		
		default:
			*numerator = 0;
			*denominator = 1;
	}
}

void customPSCG3(int n, int* numerator, int* denominator) {
	switch (n) {
		case 0:
			*numerator = 1;
			*denominator = 1;
			break;
			
		case 7:
			*numerator = 1;
			*denominator = 1;
			break;
		
		case 11:
			*numerator = 1;
			*denominator = 1;
			break;

		case 14:
			*numerator = 1;
			*denominator = 1;
			break;
			
		case 18:
			*numerator = 2;
			*denominator = 1;
			break;
			
		case 22:
			*numerator = 1;
			*denominator = 1;
			break;
			
		case 21:
			*numerator = 1;
			*denominator = 1;
			break;
			
		case 25:
			*numerator = 3;
			*denominator = 1;
			break;
			
		case 29:
			*numerator = 3;
			*denominator = 1;
			break;
			
		case 33:
			*numerator = 1;
			*denominator = 1;
			break;
			
		default:
			*numerator = 0;
			*denominator = 1;
	}
}

// Lazy method uses what has already been computed.
// if we put the for loop inside like below, it is lazy.
// if we want to start the computation from scratch each time,
// we need to put the for loop outside (in caller).
void naiveMultiplyReuse() {

	Series f(PSCG1);
	Series g(PSCG1);
	Series1 sr1(f, g);
	mpq_t coeff;
	mpq_init(coeff);
	
	for (int power = 1; power < 19; ++power) {
		std::cout << "power is: "<< power << std::endl; 

		// coeffIdx is the product precision.
		// precision values are from 0 to (powers of 2) - 1.
		int coeffIdx = pow(2, power) - 1;
	
		//startTimer();
		auto start = std::chrono::high_resolution_clock::now();	
	
		sr1.getCoefficient(coeffIdx, coeff);
	
		//stopTimer();
		auto end = std::chrono::high_resolution_clock::now();
		
		std::chrono::duration<double> duration = end - start;
	
		// sr1.print();
	
		//std::cout << "Size of product terms known so far" 
		//" (only first half is complete): "
		// << sr1.getSeriesRep()->getSizeKnownSoFar() << std::endl;

		std::cout << "Requested coefficient (index: " << coeffIdx 
			<< ") by naive (lazy) multiplication is: ";
		mpq_out_str(stdout, 10, coeff);
		std::cout << '\n';
		std::cout << "Execution time: " << duration.count() << " seconds"
		<< std::endl;
	}
}

// Relaxed multiplication
void DnCMultiplyReuse() {

	//Series f(customPSCG3);
	//Series g(customPSCG3);
	
	Series f(PSCG1);
	Series g(PSCG1);

	Series2 sr2(f, g);
	mpq_t coeff;
	mpq_init(coeff);

	for (int power = 1; power < 19; ++power) {
		// product precision.
		int coeffIdx = pow(2, power) - 1;

		//startTimer();
		auto start = std::chrono::high_resolution_clock::now();	
		sr2.getCoefficient(coeffIdx, coeff);
		//stopTimer();
		auto end = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> duration = end - start;

		// sr2.print();

		//std::cout << "Size of product terms known so far" 
		//" (only first half is complete): "
		// << sr2.getSeriesRep()->getSizeKnownSoFar() << std::endl;

		std::cout << "Requested coefficient (index: " << coeffIdx 
		<< ") by relaxed DnC is: ";
		mpq_out_str(stdout, 10, coeff);
		std::cout << '\n';
		std::cout << "Execution time: " << duration.count() << " seconds"
		<< std::endl;
	
		std::cout << power << "\t" << duration.count() << std::endl;
	}
}

void naiveMultiply() {
	
	for (int power = 1; power < 16; ++power) {

		Series f(PSCG1);
		Series g(PSCG1);
		Series1 sr1(f, g);
		mpq_t coeff;
		mpq_init(coeff);
		
		std::cout << "power is: "<< power << std::endl; 

		// coeffIdx is the product precision.
		// precision values are from 0 to (powers of 2) - 1.
		int coeffIdx = pow(2, power) - 1;
	
		//startTimer();
		auto start = std::chrono::high_resolution_clock::now();	
	
		sr1.getCoefficient(coeffIdx, coeff);
	
		//stopTimer();
		auto end = std::chrono::high_resolution_clock::now();
		
		std::chrono::duration<double> duration = end - start;
	
		// sr1.print();
	
		//std::cout << "Size of product terms known so far" 
		//" (only first half is complete): "
		// << sr1.getSeriesRep()->getSizeKnownSoFar() << std::endl;

		std::cout << "Requested coefficient (index: " << coeffIdx 
			<< ") by naive (lazy) multiplication is: ";
		mpq_out_str(stdout, 10, coeff);
		std::cout << '\n';
		std::cout << "Execution time: " << duration.count() << " seconds"
		<< std::endl;
	}
}

// changing the DnC threshold affects runtime.
void DnCMultiply() {

	for (int power = 1; power < 16; ++power) {
		//Series f(customPSCG3);
		//Series g(customPSCG3);
	
		Series f(PSCG1);
		Series g(PSCG1);

		Series2 sr2(f, g);
		mpq_t coeff;
		mpq_init(coeff);

		// product precision.
		int coeffIdx = pow(2, power) - 1;

		//startTimer();
		auto start = std::chrono::high_resolution_clock::now();	
		sr2.getCoefficient(coeffIdx, coeff);
		//stopTimer();
		auto end = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> duration = end - start;

		// sr2.print();

		//std::cout << "Size of product terms known so far" 
		//" (only first half is complete): "
		// << sr2.getSeriesRep()->getSizeKnownSoFar() << std::endl;

		std::cout << "Requested coefficient (index: " << coeffIdx 
		<< ") by relaxed DnC is: ";
		mpq_out_str(stdout, 10, coeff);
		std::cout << '\n';
		std::cout << "Execution time: " << duration.count() << " seconds"
		<< std::endl;
	
		std::cout << power << "\t" << duration.count() << std::endl;
	}
}

int main(int argc, char** argv)
{
	//naiveMultiply();
	DnCMultiply();

	//naiveMultiplyReuse();
	//DnCMultiplyReuse();
	
	return 0;
}
