#ifndef SERIES_H
#define SERIES_H

#include <gmp.h>
#include "Series_Rep.h"

// threshold of DnC multiplication.
const int DnCThreshold = 16;

class Series {

	public:
		
		// constructor takes a function that produces a term given an index
		Series(void (*inputPSCG)(int, int*, int*));
		Series();
		
		Series_Rep* getSeriesRep() const {return sRep;}
		
		// A return of an mpz_t etc doesn’t return the object. Hence,
		// parameter is passed to it to fill.
		// https://gmplib.org/manual/Parameter-Conventions
		// k starts from 0. f0, f1, ..., but n starts from 1.
		virtual void getCoefficient(int k, mpq_t coeff);
		
		void print();
		
	protected: 
		Series_Rep *sRep;
		void (*PSCG)(int, int*, int*);
};
#endif // SERIES_H
