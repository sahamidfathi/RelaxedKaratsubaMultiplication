#ifndef SERIES2_H
#define SERIES2_H

#include <gmp.h>
#include "Series.h"

class Series2 : public Series {
	public:
	
		// the constructor calculates upto Threshold
		Series2(Series fParam, Series gParam);
		
		void getCoefficient(int k, mpq_t coeff) override;
		
	private:
		Series f, g;
		bool isThresholdComputed = false;
};
#endif // SERIES2_H
