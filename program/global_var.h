#ifdef __VISUALDSPVERSION__
#define _COMPILE_WITH_BLACKFIN	// uncomment to compile with other compiler, some functions will be unavailable
#endif

#ifndef  _GLOBAL_VAR_H
#define _GLOBAL_VAR_H


//--------------------------------------------------------------------------------------------------------
// Header files
//--------------------------------------------------------------------------------------------------------
#ifdef _COMPILE_WITH_BLACKFIN

#include <fract.h>		// fract typedefs and functions
#include <complex.h>	// complex typedefs and functions
#include <cycles.h>		// statistics

//#define ETSI_SOURCE
//#define __SET_ETSI_FLAGS 1

#else

#include <stddef.h>
#define fract16 int
#define _COMPLEX_DEFINED
#include "other_complex.h"

#endif

#include "error.h"


//--------------------------------------------------------------------------------------------------------
// Symbolic constants
//--------------------------------------------------------------------------------------------------------
// names for registers in AD1854/AD187 converters
#define INTERNAL_ADC_L0			0
#define INTERNAL_ADC_R0			1
#define INTERNAL_DAC_L0			0
#define INTERNAL_DAC_R0			1

#define AUDIO_RESET_DELAY		0xf00

// SPORT0 word length
#define SLEN_24					0x0017

// DMA flow mode
#define FLOW_1					0x1000

// UART buffersize
#define UART_BUF_SIZE			100

// audio buffersize
#define AUDIO_BUF_SIZE			2				// 2 bytes for 24bit ADC/DAC

// sampling rate of ADC
#define F_SAMPLING				48000.0

// parameters for preallocated data/coefficient arrays
#define COEFF_SIZE				100
#define DELAY_SIZE				100

#define COEFF_TYPE				fract16
#define DELAY_TYPE				fract16

// math constants
#define PI						3.14159265358979323846
#define PIP2					1.57079632679489661923		// pi/2

#define DEFAULT_BW_E0			1.0			// 1/sqrt(2) (~3dB) attenuation at corner freq
#define DEFAULT_C1_E0			0.5			// ~1dB passband ripple
#define DEFAULT_C2_E0			1.0			// 1/sqrt(2) (~3dB) attenuation at corner freq

#define MAX_DB_INPUT			240
#define MIN_LIN_INPUT			1e-12
#define MAX_WS_INPUT			1200.0		// usable freq range: 20-24000Hz

#define MIN_BANDWIDTH			100			// ~16Hz hasraütve
#define MIN_FREQUENCY			125			// ~20Hz
#define MAX_FREQUENCY			150000		// ~24kHz

#define CODE_DELIMITER			'*'

#define WARP_AUTO				-1.0
#define WARP_NORMAL				-2.0

#define DIGITAL_FILTER			-1

#define MAX_REAL				9999999


//--------------------------------------------------------------------------------------------------------
// Typedefs and enums
//--------------------------------------------------------------------------------------------------------

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
	iirParameters		iirP;
	transformParameters	transformP;
	pzkContainer		*iFilter,
						*tFilter,
						*dFilter;
	void (*filter)(void);
	real		warping;
	filterType	type,			// LP, HP, BP, BS
				subtype,		// butterworth, cheby, hanning, kaiser....
				supertype;		// iir, fir
	filterState	fState;
	uint options;
	
} filterInfo;


//--------------------------------------------------------------------------------------------------------
// Global variables
//--------------------------------------------------------------------------------------------------------

extern int aLeftIn;
extern int aRightIn;
extern int aLeftOut;
extern int aRightOut;

// DMA buffers for audio
extern int aRxBuffer[];
extern int aTxBuffer[];
extern int channelDelay;

extern pzkContainer pzkList[];

extern char uart_buffer[];

// request to set the filter
extern uint changeFilterRequest;

// delay line, and coeff array
extern DELAY_TYPE delayLineR[];
extern COEFF_TYPE * coeffLineR;
extern COEFF_TYPE * coeffLineTemp;
extern filterInfo filterR;

#endif

