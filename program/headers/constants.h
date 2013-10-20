#ifdef __VISUALDSPVERSION__
#ifndef _COMPILE_WITH_BLACKFIN
#define _COMPILE_WITH_BLACKFIN	// uncomment to compile with other compiler, some functions will be unavailable
#endif
#endif

#ifndef  _CONSTANTS_H
#define _CONSTANTS_H

//--------------------------------------------------------------------------------------------------------
// Config constants
//--------------------------------------------------------------------------------------------------------

// UART buffersize
#define INPUT_BUF_SIZE				1000

// Placement in memory
#define DELAY_SECTION
#define COEFF_SECTION

// parameters for preallocated data/coefficient arrays
#define COEFF_SIZE					100
#define DELAY_SIZE					100
#define COEFF_TYPE					fract16
#define DELAY_TYPE					fract16

// filter input/output types
#define INPUT_TYPE					fract16
#define OUTPUT_TYPE					fract32
#define INPUT_MAX					0x7FFF

// sampling rate
#define F_SAMPLING					48000.0

// Number of filters
#define FILTER_NUM					1

// Debugging
#define	DEBUG_FUNCTIONS_ENABLED
// Testcase
#define TESTCASE_DIR				"../simulation/testcase/"
#define	TC_CALL_MAX_DEPTH			3
#define COPY_COMMENTS				2			// 0 - dont, 1 - mark, 2 - copy
// Simulation
#define SIM_INPUT_DIR				"../simulation/input/"
#define SIM_OUTPUT_DIR				"../simulation/output/"

// Logging
#define LOGGING_DIR					"../simulation/log/"
#define LOGGING_ENABLED
#define LOGGING_MODE				"w"			// "a" - append, "w" - write

// Error
#define ERROR_BUFFER_SIZE			10

// For CPU usage
#define CLOCK_CALIBRATION_LENGTH	16

// Default e0 values for referent filters
#define DEFAULT_BW_E0				1.0			// 1/sqrt(2) (~3dB) attenuation at corner freq
#define DEFAULT_C1_E0				0.5			// ~1dB passband ripple
#define DEFAULT_C2_E0				1.0			// 1/sqrt(2) (~3dB) attenuation at corner freq

// Limits for the filters
#define MAX_DB_INPUT				240
#define MIN_LIN_INPUT				1e-12
#define MAX_WS_INPUT				1200.0		// usable freq range: 20-24000Hz
#define MIN_BANDWIDTH				100			// ~16Hz hasraütve
#define MIN_FREQUENCY				0.001		// ~20Hz
#define MAX_FREQUENCY				150000		// ~24kHz

//--------------------------------------------------------------------------------------------------------
// Header files
//--------------------------------------------------------------------------------------------------------

#ifdef _COMPILE_WITH_BLACKFIN

#include <fract.h>				// fract typedefs and functions
#include <complex.h>			// complex typedefs and functions
#include "device/bf_const.h"	// additional constants for blackfin

#else

#include <stddef.h>				// NULL pointer
#include <stdint.h>				// fixed width int
#define fract16 int16_t			// instead of fract.h
#define fract32 int32_t			// instead of fract.h
#define USE_CUSTOM_MATH			// (in emath.h) instead of complex.h

#endif

//--------------------------------------------------------------------------------------------------------
// Symbolic constants
//--------------------------------------------------------------------------------------------------------

// Math constants
#define PI						3.14159265358979323846
#define PIP2					1.57079632679489661923		// pi/2
#define DIGITAL_ZERO			-6.283						// ~ -2pi

// Input code delimiters
#define CODE_DELIMITER			'*'

// Digitalization
#define WARP_AUTO_FIX			-1.0
#define WARP_FACTOR				-2.0
#define WARP_FREQUENCY			-3.0

// Pole-zero ordering
#define SORT_BY_MAGNITUDE		0
#define SORT_BY_QFACTOR			1
#define ORDER_UP				2
#define ORDER_DOWN				1

// Pole-zero pairing
#define PAIR_POLES_TO_ZEROS		0
#define PAIR_ZEROS_TO_POLES		1
#define EMPTY_PAIR				-1
#define WZ_PAIR					-2

// Biquad ordering - not yet implemented
#define INFINITY_IN_SEQUENCE	0
#define INFINITY_FIRST			1
#define INFINITY_LAST			2

// IIR filter creation and debugging
#define P_PRINT					0x01
#define P_REFERENT				0x02
#define P_TRANSFORMED			0x04
#define P_BIQUAD				0x08
#define P_DIGITALIZED			0x10
#define P_ALL					0xFE

#endif
