#ifndef SERIES1_H
#define SERIES1_H

#include "Series.h"

class Series1 : public Series {

	public:
		
		Series1(Series fParam, Series gParam);
		~Series1();
	
	private:
		Series f, g;
};
#endif // SERIES1_H
