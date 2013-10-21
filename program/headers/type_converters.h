#ifndef _TYPE_CONVERTERS_H
#define _TYPE_CONVERTERS_H

#include "types.h"

#ifdef _COMPILE_WITH_BLACKFIN

#include <fract2float_conv.h>

#else

#define float_to_fr16(p)	(int)(32768.0*(p))
#define float_to_fr32(p)	(int)(32768.0*65536.0*(p))
#define fr16_to_float(p)	((float)(p))/32768.0
#define fr16_to_fr32(p)		(int)((p)*65536)

#endif

// according to INPUT_TYPE and OUTPUT_TYPE definitions in constants.h
#define input_to_output(p)	fr16_to_fr32(p)
#define input_to_float(p)	fr16_to_float(p)
#define float_to_output(p)	float_to_fr32(p)

#endif
