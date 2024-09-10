#ifndef PRODUCT1_SERIES_REP_H
#define PRODUCT1_SERIES_REP_H

#include <gmp.h>
#include "Series_Rep.h"
#include "Series.h"

class Product1_Series_Rep : public Series_Rep {
	// The constructor takes two series on input which are stored in f and g.
	public:
		// TPS *phi, int n, int getSizeKnownSoFar() const, 
		// mpq_t* getCoefficientArray() const are inherited.

		Product1_Series_Rep(Series fParam, Series gParam);
		
		void next() override;
		
	private:	
		Series f, g;
};

#endif // PRODUCT1_SERIES_REP_H
