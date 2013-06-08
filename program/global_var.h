#define _COMPILE_WITH_BLACKFIN	// uncomment to compile with other compiler, some functions will be unavailable

#ifndef  _GLOBAL_VAR_H
#define _GLOBAL_VAR_H


//--------------------------------------------------------------------------------------------------------
// Header files
//--------------------------------------------------------------------------------------------------------
#ifdef _COMPILE_WITH_BLACKFIN

#include <fract.h>		// fract typedefs and functions
#include <complex.h>	// complex typedefs and functions
#include <cycles.h>		// statistics

#else

#include <stddef.h>
#define _COMPLEX_DEFINED
#include "other_complex.h"

#endif


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

// math constants
#define PI						3.14159265358979323846
#define PIP2					1.57079632679489661923		// pi/2


//--------------------------------------------------------------------------------------------------------
// Typedefs and enums
//--------------------------------------------------------------------------------------------------------

typedef complex_double	complex;
typedef double			real;
typedef unsigned int	uint;

typedef enum {
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
			wz,
			pwf;
	complex * poles;
	complex * zeros;
	uint	pSize,
			zSize,
			nextPole,
			nextZero;
	int	no_wz;
	filterType type;
} pzkContainer;


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

extern void (*filterFunction)(void);
extern void *delayLine;
extern void *coefLine;
extern uint stages;

extern char uart_buffer[];

// request to set the filter
extern uint changeFilterRequest;

#ifdef _COMPILE_WITH_BLACKFIN
// variables for statistics
extern cycle_stats_t sOneTime;
#endif

#endif

