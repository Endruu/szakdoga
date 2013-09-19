#ifndef  _IIR_FILTERS_H 
#define _IIR_FILTERS_H

#include "types.h"

fract32 passThrough(fract16 input, fract16 * coeffs, fract16 * delays);

fract32 direct1_float(fract16 input, fract16 * coeffs, fract16 * delays);

#ifndef _COMPILE_WITH_BLACKFIN
fract32 direct1(fract16 input, fract16 * coeffs, fract16 * delays);
#endif

#endif
