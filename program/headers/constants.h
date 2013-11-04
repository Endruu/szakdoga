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

#ifdef _COMPILE_WITH_BLACKFIN
#include "config_dsp.h"
#else
#include "config_vs.h"
#endif

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
#define P_IMPLEMENTED			0x20
#define P_ALL					0xFE

#endif
