#include <cmath>
#include "../include/Series.h"

Series::Series(void (*inputPSCG)(int, int*, int*)) : PSCG(inputPSCG)
{
	sRep = new Series_Rep(inputPSCG);
}
		
Series::Series() {sRep = new Series_Rep();}
		
void Series::getCoefficient(int k, mpq_t coeff) {
	int n = sRep->getSizeKnownSoFar(); // at the beginning, n = 0

	// k is requested index of series term starting from 0.(0th,1st,...)
			
	// we calculate the number of terms of size of powers of 2 e.g., if 
	// the coefficient of 6th term (index = 5) is requested, we
	// calculate the coefficients of the first 8 terms of the series
	// this can only be true for individual series.

	int reqSize = k + 1;

	if (reqSize > n) {
		int upBound = ((k == 0) ? 1 : pow(2, (int)(log2(k)) + 1));
		// inform sRep of upBound
		// making space for TPS array
		sRep->getTruncatedPowerSeries()->ensureOrder(upBound);
				
		for (int i = n + 1; i < (upBound + 1); ++i) {
			sRep->next();
			
			//std::cout << "next (individual series or product series)"
			//" being calculated ..." 
			//<< std::endl;
		}
	}	
	mpq_set(coeff, sRep->getTruncatedPowerSeries()->getCoefficientArray()[k]);	
}
		
void Series::print() {
	sRep->print();
}

