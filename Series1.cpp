#include "Series1.h"
#include "Product1_Series_Rep.h"

Series1::Series1(Series fParam, Series gParam) : f(fParam), g(gParam) {
	sRep = new Product1_Series_Rep(fParam, gParam);
}

Series1::~Series1() {
	delete sRep;
	sRep = nullptr;
}

