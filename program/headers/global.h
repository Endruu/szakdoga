#ifndef  _GLOBAL_H 
#define _GLOBAL_H 

//--------------------------------------------------------------------------------------------------------
// Header files	and macros																	
//--------------------------------------------------------------------------------------------------------
#include "variables.h"		// global variables
#include "error.h"			// error handling
#include "diagnostics.h"
#include "communication.h"


#ifdef _COMPILE_WITH_BLACKFIN
#include "device_blackfin.h"
#endif

// FILTERS.C	//----------------------------------------------------------------------------------------

void fPassThrough(void);
void fDirect1(void);

// UART.C	//--------------------------------------------------------------------------------------------
uint toggleEcho(void);
uint sendChar(char c);
uint receiveString(void);

real implementFilter(filterInfo * fi);

OUTPUT_TYPE direct1(INPUT_TYPE input, COEFF_TYPE * coeffs, DELAY_TYPE * delays);
OUTPUT_TYPE passThrough(INPUT_TYPE input, COEFF_TYPE * coeffs, DELAY_TYPE * delays);

#endif

