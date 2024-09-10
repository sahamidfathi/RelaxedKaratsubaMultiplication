#include "../include/TPS.h"
#include "../include/Series_Rep.h"

Series_Rep::Series_Rep(void (*PSCGParam)(int, int*, int*)) : PSCG(PSCGParam)
{
	phi = new TPS;
}

Series_Rep::Series_Rep() {
	phi = new TPS;
}
		
Series_Rep::~Series_Rep() {
	delete phi;
}

void Series_Rep::next() {
	n++;
	phi->ensureOrder(n);
			
	int numerator, denominator;
	PSCG(n - 1, &numerator, &denominator);

	mpq_t coeff;
	mpq_init(coeff);
	mpq_set_si(coeff, numerator, denominator);
			
	phi->setElement(n - 1, coeff);
			
	mpq_clear(coeff);
}

void Series_Rep::print() {
	phi->printTPS();
}	







