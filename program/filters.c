#include "global.h"
#include <libetsi.h>

void resetFilter()
{
	if( delayLine != NULL ) {
		free(delayLine);
		delayLine = NULL;
	}
	
	if( coefLine != NULL ) {
		free(coefLine);
		coefLine = NULL;
	}
	
	filterFunction = &fPassThrough;
	stages = 0;
}

void fPassThrough() {
	aRightOut	= aRightIn;
	aLeftOut	= aRightIn;
}

void fDirect1() {
	fract32 acc = 0;
}
