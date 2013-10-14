#ifndef  _IIR_FILTERS_H 
#define _IIR_FILTERS_H

#include "types.h"

OUTPUT_TYPE passThrough(INPUT_TYPE input, COEFF_TYPE * coeffs, DELAY_TYPE * delays);
OUTPUT_TYPE f_direct1_float_postK(INPUT_TYPE input, COEFF_TYPE * coeffs, DELAY_TYPE * delays);

#endif
