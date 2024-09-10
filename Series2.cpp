#include <cmath>
#include "Series2.h"
#include "Product2_Series_Rep.h"

Series2::Series2(Series fParam, Series gParam) : f(fParam), g(gParam) {
	sRep = new Product2_Series_Rep(fParam, gParam);
}
		
void Series2::getCoefficient(int k, mpq_t coeff) {
		
	int n = sRep->getSizeKnownSoFar(); // at the beginning, n = 0

	// k is requested index of series term starting from 0.(0th,1st,...)
			
	// for the product series, the number of terms is equal to 
	// 2*(n - 1) + 1 = 2n-1 in which n is the number of terms 
	// (upBound) of the individual series (which is a power of 2). e.g.
	// if n = 4, the product will have 7 terms. 
	int reqSize = k + 1;
			
	if (reqSize > n) {
		// number of terms the product should have if getCoefficient(k)
		// is requested is equal to upBound.
		// if reqSize is x, we compute the product up to the first power of 2
		// that is equal to y >= x. For this, indSeriesSize needs to be = y.
		// The size of the product will be 2*indSeriesSize-1, but the second
		// half orders are to be completed in the next round.
		int upBound = ((k == 0) ? 1 : pow(2, (int)(log2(k)) + 1));
		
		//int var = ((int)(log2(reqSize + 1) - 1) == 
		//	(log2(reqSize + 1) - 1)) ? (log2(reqSize + 1) - 1) : 
		//	((int)((log2(reqSize + 1) - 1)) + 1);
			
		//int upBound = pow(2, var + 1); //pow(2, var + 1) - 1
				
		// required individual series size to calculate product 
		// up to upBound
		// if requested product size is less then Threshold, we return up to 
		// Threshold.
		int Threshold = DnCThreshold;
		if (Threshold > upBound)
			upBound = Threshold;
			
		int indSeriesSize = upBound;
		
		// TODO: pass computed f and g to here, instead of computing 
		// them here.
		// calculate f and g up to indSeriesSize
		mpq_t dummyArg;
		mpq_init(dummyArg);
		f.getCoefficient(indSeriesSize - 1, dummyArg);
		g.getCoefficient(indSeriesSize - 1, dummyArg);
		
		// We set the size of product = 2 * indSeriesSize (or upBound) - 1
		// 
		mpq_t *product = new mpq_t[2 * indSeriesSize - 1];
		for (int i = 0; i < 2 * indSeriesSize - 1; ++i)
				mpq_init(product[i]);
		
		if (!isThresholdComputed) {
			mpq_t *tProduct = new mpq_t[2 * Threshold - 1];
			for (int i = 0; i < 2 * Threshold - 1; ++i)
				mpq_init(tProduct[i]);
						
			sRep->ordinaryMultiplication(f.getSeriesRep()->
				getTruncatedPowerSeries()->getCoefficientArray(), 
				g.getSeriesRep()->getTruncatedPowerSeries()
				->getCoefficientArray(), tProduct, Threshold);					
						
			sRep->getTruncatedPowerSeries()->ensureOrder(2 * Threshold - 1);
			sRep->getTruncatedPowerSeries()->setCoefficientArray(tProduct);
			sRep->setSizeKnownSoFar(2 * Threshold - 1);
					
			isThresholdComputed = true;
		}

		//TODO: if it's enough to give lo (previous product), don't 
		// calculate the next round.
		
		for (int i = 0; i < sRep->getSizeKnownSoFar(); ++i)
			mpq_set(product[i], sRep->
				getTruncatedPowerSeries()->getCoefficientArray()[i]);
		
		// the first sRep->getSizeKnownSoFar() elements of product
		// contains the previously computed product which will be used
		// as lo for the next round.
		sRep->DAC(f.getSeriesRep()->getTruncatedPowerSeries()
			->getCoefficientArray(), g.getSeriesRep()->getTruncatedPowerSeries()
			->getCoefficientArray(), indSeriesSize, product, 
			sRep->getSizeKnownSoFar());
				
		// making space for TPS array
		sRep->getTruncatedPowerSeries()->ensureOrder(2 * indSeriesSize - 1);
				
		// we should define a setTPSArray() which sets the 
		// TPS array pointer to the product pointer and avoid copying
		// the product array into the TPS array.
		// assigning TPS array pointer to point to the productArray
		sRep->getTruncatedPowerSeries()
			->setCoefficientArray(product);
		// size known so far, although the second half of the product is not 
		// complete (it is printed though).
		sRep->setSizeKnownSoFar(2 * indSeriesSize - 1); 
	}
				
	mpq_set(coeff, sRep->getTruncatedPowerSeries()->getCoefficientArray()[k]);
}	

