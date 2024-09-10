#ifndef PRODUCT2_SERIES_REP_H
#define PRODUCT2_SERIES_REP_H

#include <gmp.h>
#include "Series_Rep.h"
#include "Series.h"

class Product2_Series_Rep : public Series_Rep {
	// The constructor takes two series on input which are stored in f and g.
	public:
		// TPS *phi, int n = 0, int getSizeKnownSoFar() const, 
		// mpq_t* getCoefficientArray() const are inherited.

		//DAC_Rep *h;

		Product2_Series_Rep(Series fParam, Series gParam);
		
		void ordinaryMultiplication(mpq_t *f, mpq_t *g, mpq_t *product, 
			int indSeriesSize) override;
		
		// relaxed DAC
		// product is an empty array of size 2 * indSeriesSize - 1 to be filled
		void DAC(mpq_t *f, mpq_t *g, int indSeriesSize, mpq_t *product, 
			int preProductSize) override;
		
		// static DAC		
		void DAC(mpq_t *f, mpq_t *g, mpq_t *product, int indSeriesSize); 
		//void DAC(mpq_t *f, mpq_t *g, mpq_t *product, int indSeriesSize); 

	private:
		Series f, g;
};

#endif // PRODUCT2_SERIES_REP_H
