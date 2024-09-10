#include "Product1_Series_Rep.h"

Product1_Series_Rep::Product1_Series_Rep(Series fParam, Series gParam) : 
	f(fParam), g(gParam) {}	
		
void Product1_Series_Rep::next() {
	n++;

	phi->ensureOrder(n);
			
	mpq_t coeff1, coeff2, product, productSum;
	mpq_init(coeff1);
	mpq_init(coeff2);
	mpq_init(product);
	mpq_init(productSum);

	// since elements are added one by one, the multiplication
	// algorithm is different from that of TPS
	for (int i = 0; i < n; ++i) {

		f.getCoefficient(i, coeff1);
		g.getCoefficient(n - i - 1, coeff2);
		mpq_mul(product, coeff1, coeff2);
						
		mpq_add(productSum, productSum, product);					
	}
			
	phi->setElement(n - 1, productSum);
			
	mpq_clear(coeff1);
	mpq_clear(coeff2);
	mpq_clear(product);
	mpq_clear(productSum);
}
