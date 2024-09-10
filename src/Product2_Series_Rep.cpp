#include "../include/Product2_Series_Rep.h"

Product2_Series_Rep::Product2_Series_Rep(Series fParam, Series gParam) : 
	f(fParam), g(gParam) {
	//	h = new DAC_Rep(f, g, Threshold);
}
		
void Product2_Series_Rep::ordinaryMultiplication(mpq_t *f, mpq_t *g, 
	mpq_t *product, int indSeriesSize) {
	mpq_t partialProduct;
	mpq_init(partialProduct);
				
	for (int i = 0; i < indSeriesSize; ++i) {
		for (int j = 0; j < indSeriesSize; ++j) {
			mpq_mul(partialProduct, f[i], g[j]);
			mpq_add(product[i + j], product[i + j], partialProduct);
		}
	}
	mpq_clear(partialProduct);
}
		
// relaxed DAC
// product is an empty array of size 2 * indSeriesSize - 1 to be filled
void Product2_Series_Rep::DAC(mpq_t *f, mpq_t *g, int indSeriesSize, 
	mpq_t *product, int preProductSize) {
					
	// preProduct (previously computed product) is inside of product
	mpq_t *loArray;
	mpq_t *midArray;
	mpq_t *hiArray;
	mpq_t *f_up_star;
	mpq_t *g_up_star;
	mpq_t *f_low_plus_up;
	mpq_t *g_low_plus_up;
				
	while (preProductSize < 2 * indSeriesSize - 1) {
			
		loArray = product;
				
		// we start from the already known (pre.prod.) and work our way
		// to the requested product size which is 2 * indSeriesSize - 1
		midArray = new mpq_t[preProductSize];
		hiArray = new mpq_t[preProductSize];
				
		for (int i = 0; i < preProductSize; ++i) {
			mpq_init(midArray[i]);
			mpq_init(hiArray[i]);
		}
				
		f_up_star = new mpq_t[(preProductSize + 1) / 2];
		g_up_star = new mpq_t[(preProductSize + 1) / 2];
		f_low_plus_up = new mpq_t[(preProductSize + 1) / 2];
		g_low_plus_up = new mpq_t[(preProductSize + 1) / 2];
				
		for (int i = 0; i < (preProductSize + 1) / 2; ++i) {
			mpq_init(f_up_star[i]);
			mpq_init(g_up_star[i]);
			mpq_init(f_low_plus_up[i]);
			mpq_init(g_low_plus_up[i]);

			mpq_set(f_up_star[i], f[(preProductSize + 1) / 2 + i]);
			mpq_set(g_up_star[i], g[(preProductSize + 1) / 2 + i]);
					
			mpq_add(f_low_plus_up[i], f[i], 
				f[(preProductSize + 1) / 2 + i]);
			mpq_add(g_low_plus_up[i], g[i], 
				g[(preProductSize + 1) / 2 + i]);
		}
		
		// lo is already computed, calculating mid and hi
		DAC(f_up_star, g_up_star, hiArray, (preProductSize + 1) / 2); //
		DAC(f_low_plus_up, g_low_plus_up, midArray, (preProductSize + 1) / 2); // 

		// from here, mid = mid - lo - hi
		for (int i = 0; i < preProductSize; ++i) {
			mpq_sub(midArray[i], midArray[i], loArray[i]);
			mpq_sub(midArray[i], midArray[i], hiArray[i]);
		}
				
		// assemble product array, it already has lo, add mid and hi
		for (int i = 0; i < preProductSize; ++i)
			mpq_add(product[(preProductSize + 1) / 2 + i], 
			product[(preProductSize + 1) / 2 + i], midArray[i]);
			
		for (int i = 0; i < preProductSize; ++i)
			mpq_add(product[preProductSize + 1 + i], 
				product[preProductSize + 1 + i], hiArray[i]);
				
		// clearing arrays
		for (int i = 0; i < (preProductSize + 1) / 2; ++i) {
			mpq_clear(f_up_star[i]);
			mpq_clear(g_up_star[i]);
			mpq_clear(f_low_plus_up[i]);
			mpq_clear(g_low_plus_up[i]);
		}
		for (int i = 0; i < preProductSize; ++i) {
			mpq_clear(midArray[i]);
			mpq_clear(hiArray[i]);
		}
				
		delete[] f_up_star;
		f_up_star = nullptr;
		delete[] g_up_star;
		g_up_star = nullptr;
		delete[] f_low_plus_up;
		f_low_plus_up = nullptr;
		delete[] g_low_plus_up;
		g_low_plus_up = nullptr;
				
		delete[] midArray;
		midArray = nullptr;
		delete[] hiArray;
		hiArray = nullptr;
				
		// number of computed product = 2 * preProductSize + 1
		preProductSize = 2 * preProductSize + 1;
	}
}
		
// static DAC, it uses recursive calls		
void Product2_Series_Rep::DAC(mpq_t *f, mpq_t *g, mpq_t *product, int indSeriesSize) {	//

	// if indSeriesSize <= Threshold, we don't apply DAC,
	// we perform ordinary multiplication
	if (indSeriesSize <= DnCThreshold) {
		ordinaryMultiplication(f, g, product, indSeriesSize);
	} else {
		// instead of copying data from f and g to f_*, f^*, g_*, g^*,
		// we change the address the pointer is holding.
		mpq_t *f_low_star = f;
		mpq_t *g_low_star = g;
		mpq_t *f_up_star = f + indSeriesSize / 2;
		mpq_t *g_up_star = g + indSeriesSize / 2;
		mpq_t *f_low_plus_up = new mpq_t[indSeriesSize / 2];
		mpq_t *g_low_plus_up = new mpq_t[indSeriesSize / 2];

		for (int i = 0; i < indSeriesSize / 2; ++i) {
			mpq_init(f_low_plus_up[i]);
			mpq_init(g_low_plus_up[i]);

			mpq_add(f_low_plus_up[i], f[i], f[indSeriesSize / 2 + i]);
			mpq_add(g_low_plus_up[i], g[i], g[indSeriesSize / 2 + i]);
		}

		// lo = (f_*)(g_*)
		// mid = (f_* + f^*)(g_* + g^*)
		// hi = (f^*)(g^*)
		// size (# of elements) of these = indSeriesSize - 1
			
		mpq_t *loArray = new mpq_t[indSeriesSize - 1];
		mpq_t *midArray = new mpq_t[indSeriesSize - 1];
		mpq_t *hiArray = new mpq_t[indSeriesSize - 1];

		for (int i = 0; i < indSeriesSize - 1; ++i) {
			mpq_init(loArray[i]);
			mpq_init(midArray[i]);
			mpq_init(hiArray[i]);
		}

		DAC(f_low_star, g_low_star, loArray, indSeriesSize / 2);	 
		DAC(f_up_star, g_up_star, hiArray, indSeriesSize / 2);	 
		DAC(f_low_plus_up, g_low_plus_up, midArray, indSeriesSize / 2);

		// from here, mid = mid - lo - hi
		for (int i = 0; i < indSeriesSize - 1; ++i) {
			mpq_sub(midArray[i], midArray[i], loArray[i]);
			mpq_sub(midArray[i], midArray[i], hiArray[i]);
		}
			
		// assemble product array from lo, hi, mid - lo - hi
		// add lo to the array
		for (int i = 0; i < indSeriesSize - 1; ++i)
			mpq_add(product[i], product[i], loArray[i]);
		
		// mid is actually mid - lo - hi
		for (int i = 0; i < indSeriesSize - 1; ++i)
			mpq_add(product[indSeriesSize / 2 + i], 
			product[indSeriesSize / 2 + i], midArray[i]);
			
		// add hi
		for (int i = 0; i < indSeriesSize - 1; ++i)
			mpq_add(product[indSeriesSize + i], 
				product[indSeriesSize + i], hiArray[i]);
			
		// clearing arrays
		for (int i = 0; i < indSeriesSize / 2; ++i) {
			mpq_clear(f_low_plus_up[i]);
			mpq_clear(g_low_plus_up[i]);
		}
		for (int i = 0; i < indSeriesSize - 1; ++i) {
			mpq_clear(loArray[i]);
			mpq_clear(midArray[i]);
			mpq_clear(hiArray[i]);
		}
		
		delete[] f_low_plus_up;
		f_low_plus_up = nullptr;
		delete[] g_low_plus_up;
		g_low_plus_up = nullptr;
		
		delete[] loArray;
		loArray = nullptr;
		delete[] midArray;
		midArray = nullptr;
		delete[] hiArray;
		hiArray = nullptr;
	}
}

