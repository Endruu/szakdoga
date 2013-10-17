#ifdef __VISUALDSPVERSION__
#define _COMPILE_WITH_BLACKFIN	// uncomment to compile with other compiler, some functions will be unavailable
#endif

#ifndef  _CONSTANTS_H
#define _CONSTANTS_H

// UART buffersize
#define INPUT_BUF_SIZE			100

// parameters for preallocated data/coefficient arrays
#define COEFF_SIZE				100
#define DELAY_SIZE				100

#define COEFF_TYPE				fract16
#define DELAY_TYPE				fract16
#define INPUT_TYPE				fract16
#define OUTPUT_TYPE				fract32

#define INPUT_MAX				0x7FFF
#define OUTPUT_MAX				0x7FFFFFFF


#define DEFAULT_BW_E0			1.0			// 1/sqrt(2) (~3dB) attenuation at corner freq
#define DEFAULT_C1_E0			0.5			// ~1dB passband ripple
#define DEFAULT_C2_E0			1.0			// 1/sqrt(2) (~3dB) attenuation at corner freq

#define MAX_DB_INPUT			240
#define MIN_LIN_INPUT			1e-12
#define MAX_WS_INPUT			1200.0		// usable freq range: 20-24000Hz

#define MIN_BANDWIDTH			100			// ~16Hz hasraütve
#define MIN_FREQUENCY			0.001	//125			// ~20Hz
#define MAX_FREQUENCY			150000		// ~24kHz


#define BAUD_RATE				57600

#define CPU_FREQ				600000000

//--------------------------------------------------------------------------------------------------------
// Header files
//--------------------------------------------------------------------------------------------------------
#ifdef _COMPILE_WITH_BLACKFIN

#include <fract.h>			// fract typedefs and functions
#include <complex.h>		// complex typedefs and functions

#else

#include <stddef.h>			// NULL pointer
#include <stdint.h>			// fixed width int
#define fract16 int16_t		// instead of fract.h
#define fract32 int32_t		// instead of fract.h
#define USE_CUSTOM_MATH		// (in emath.h) instead of complex.h

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

// audio buffersize
#define AUDIO_BUF_SIZE			2				// 2 bytes for 24bit ADC/DAC

// sampling rate of ADC
#define F_SAMPLING				48000.0


// math constants
#define PI						3.14159265358979323846
#define PIP2					1.57079632679489661923		// pi/2

#define CODE_DELIMITER			'*'

#define WARP_AUTO_FIX			-1.0
#define WARP_FACTOR				-2.0
#define WARP_FREQUENCY			-3.0

#define DIGITAL_ZERO			-6.283	// ~ -2pi

#define CLKIN					25000000

#define LOGGING_ENABLED


#define SORT_BY_MAGNITUDE		0
#define SORT_BY_QFACTOR			1

#define ORDER_UP				2
#define ORDER_DOWN				1

#define PAIR_POLES_TO_ZEROS		0
#define PAIR_ZEROS_TO_POLES		1
#define EMPTY_PAIR				-1
#define WZ_PAIR					-2

#define INFINITY_IN_SEQUENCE	0
#define INFINITY_FIRST			1
#define INFINITY_LAST			2

#define P_PRINT					0x01
#define P_REFERENT				0x02
#define P_TRANSFORMED			0x04
#define P_BIQUAD				0x08
#define P_DIGITALIZED			0x10
#define P_ALL					0xFE

#define FILTER_NUM				1

#define TESTCASE_DIR			"../simulation/testcase/"
#define SIM_INPUT_DIR			"../simulation/input/"
#define SIM_OUTPUT_DIR			"../simulation/output/"

#define	DEBUG_FUNCTIONS_ENABLED

#endif
