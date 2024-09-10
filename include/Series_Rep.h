#ifndef SERIES_REP_H
#define SERIES_REP_H

#include <gmp.h>
#include "TPS.h"

class Series_Rep {
	// upon creation of this class, an object of TPS is initialized.
	public:
		
		void (*PSCG)(int, int*, int*);
		Series_Rep(void (*PSCGParam)(int, int*, int*));
		
		// this constructor will be used when Product1_Series_Rep is created.
		// because the constructor of Product1_Series_Rep takes two series,
		// and it is not possible to initialize the above constructor.
		// (this default constructor is for when we want to calculate 
		// coefficient from say multiplication, not using PSCG)
		Series_Rep();
		
		~Series_Rep();
		
		TPS* getTruncatedPowerSeries() const {
			return phi;	
		}

		int getSizeKnownSoFar() const {
			return n;	
		}
		
		void setSizeKnownSoFar(const int size) {
			n = size;
		}
		
		// to be overriden and used only in DAC relaxed algorithm
		virtual void DAC(mpq_t *f, mpq_t *g, int indSeriesSize, mpq_t *product, 
			int preProductSize) {}			
		
		virtual void ordinaryMultiplication(mpq_t *f, mpq_t *g, mpq_t *product, 
		int indSeriesSize) {}
		
		virtual void next();
		
		void print();
		
	protected:
		TPS *phi;
		int n = 0; // size known so far
};

#endif // SERIES_REP_H
