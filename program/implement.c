#include "global.h"

#ifdef _COMPILE_WITH_BLACKFIN

void iDirect1(pzkContainer * pzk)
{
	stages = countBiquads(pzk);

	
	// allocate memory
	delayLine = (fract16 *) malloc(stages * sizeof(fract16));
	
	
}

#endif
