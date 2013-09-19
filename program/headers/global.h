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

fract32 direct1(fract16 input, fract16 * coeffs, fract16 * delays);
fract32 passThrough(fract16 input, fract16 * coeffs, fract16 * delays);

#endif

