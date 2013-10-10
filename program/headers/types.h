#ifndef  _TYPES_H
#define _TYPES_H

#include "constants.h"

#ifdef USE_CUSTOM_MATH

typedef struct {
	double re, im;
} complex_double;

#else 

#include <complex_typedef.h>

#endif

typedef complex_double	_complex;
typedef complex_double	complex;
typedef double			real;
typedef unsigned int	uint;

typedef enum {
	empty,

	iir,
	fir,

	lowpass,
	highpass,
	bandpass,
	bandstop,

	butterworth,
	chebyshev1,
	chebyshev2,
	custom_iir

} filterType;

typedef struct {
	real	amp,
			wz;
	complex * poles;
	complex * zeros;
	uint	pSize,
			zSize,
			nextPole,
			nextZero;
	int	no_wz;
	filterType type;
} pzkContainer;

typedef enum {
	sStart,
	sReferent,
	sTransform,
	sDigitalize,
	sImplement,
	sRunning
} filterState;

typedef struct {
	double	ac,		// max attenuation at corner freq
			as,		// min attenuation at stop freq
			ws,		// stop freq
			e0;		// inner parameter
	uint	n,		// filter order
			inDb,	// 0 => abs; 1 => Db
			fixWs;	// 0 => transforms relative to the corner freq; 1 => transforms relative to the stop freq
} iirParameters;

typedef struct {
	double	w0,		// corner freq (LP, HP) / lower freq (BP, BS, isDw = 0) / center freq (BP, BS, isDw = 1)
			w1;		// upper freq (BP, BS, isDw = 0) / bandwidth (BP, BS, isDw = 1)
	uint	inHz,	// 0 => rad/s; 1 => Hz
			isDw;	// 0 => w0/w1 = upper/lower cutoff freq; 1 => w0 = center freq, w1 = bandwidth
} transformParameters;

typedef struct {
	char	pair,
			sort,
			order,
			infinity;
} implementParameters;

typedef struct {
	iirParameters		iirP;
	transformParameters	transformP;
	implementParameters implementP;

	fract32 (*filter)(fract16 input, fract16 * coeffs, fract16 * delays);

	real		warping;

	filterType	type,			// LP, HP, BP, BS
				subtype,		// butterworth, cheby, hanning, kaiser....
				supertype;		// iir, fir

	filterState	fState;

	uint 		options,
				stages,
				mem_delay,		// bytes used from delay buffer
				mem_coeff,		// bytes used from coeff buffer
				ticks;			// no. of clock ticks while filtering
	
} filterInfo;

typedef struct {
	char p1, p2, z1, z2;
} biquad;
#endif
