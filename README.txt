Implemented by S. A. Hamid Fathi (sahamidfathi@gmail.com; sfathi4@uwo.ca)


The class structures and namings are based on "Relaxed, but don't be too lazy" paper by van der Hoeven.


In relaxed DAC, we start from "low" = what we already have. We calculate "mid" and "high". Combination of these three becomes "low" for the next round. We keep doing this until the requested precision is reached (or its power of 2 upper bound).


The relaxed method starts with "low". Below threshold, naive is used. Once the threshold is reached, we have the first "low". Now that we have "low", two DAC calls are done for "mid" and "high" (two calls inside void Product2_Series_Rep::DAC(mpq_t *f, mpq_t *g, int indSeriesSize,
mpq_t *product, int preProductSize) that can be parallelized). These two calls for "mid" and "high" are static DAC Karatsuba. Each DAC Karatsuba calls itself 3 times that can be parallelized as well. Once we have "mid" and "high" we combine it with available "low", and the result becomes "low" for the next round.



Reference:
Joris van der Hoeven,
Relax, but Don’t be Too Lazy,
Journal of Symbolic Computation,
Volume 34, Issue 6,
2002,
Pages 479-542,
ISSN 0747-7171,
https://doi.org/10.1006/jsco.2002.0562.
(https://www.sciencedirect.com/science/article/pii/S0747717102905626)


