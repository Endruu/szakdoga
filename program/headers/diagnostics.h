#ifndef _DIAGNOSTICS_H
#define _DIAGNOSTICS_H

#ifdef DISABLE_DIAGNOSTICS

#ifdef ENABLE_DIAG_CU
#undef ENABLE_DIAG_CU
#endif

#ifdef ENABLE_DIAG_MU
#undef ENABLE_DIAG_MU
#endif

#ifdef ENABLE_DIAG_CW
#undef ENABLE_DIAG_CW
#endif

#endif
//--------------------------------------------------------------------------------------------------------
// Time
//--------------------------------------------------------------------------------------------------------
#ifdef ENABLE_DIAG_CU

//#include <time.h>

void calibrateClock();
void startClock();
void stopClock();

void setTick();
void printCpuUsage();

#else

#define calibrateClock()	diagUnavailable()
#define startClock()		diagUnavailable()
#define stopClock()			diagUnavailable()
#define setTick()			diagUnavailable()
#define printCpuUsage()		diagUnavailable()

#ifndef ENABLE_DIAG_UNAV_FUNC
#define ENABLE_DIAG_UNAV_FUNC
#endif

#endif
//--------------------------------------------------------------------------------------------------------
// Memory usage
//--------------------------------------------------------------------------------------------------------
#ifdef ENABLE_DIAG_MU

void setMem( int mem_delay, int mem_coeff );
void printMemoryUsage();

#else

#define setMem(p,q)			diagUnavailable()
#define printMemoryUsage()	diagUnavailable()

#ifndef ENABLE_DIAG_UNAV_FUNC
#define ENABLE_DIAG_UNAV_FUNC
#endif

#endif
//--------------------------------------------------------------------------------------------------------
// Other
//--------------------------------------------------------------------------------------------------------
#ifdef ENABLE_DIAG_CW
//void printActualFilter();
//printRoutingInfo();
int printCodeWord();

#else

#define printCodeWord()		diagUnavailable()

#ifndef ENABLE_DIAG_UNAV_FUNC
#define ENABLE_DIAG_UNAV_FUNC
#endif

#endif

#ifdef ENABLE_DIAG_UNAV_FUNC
int diagUnavailable();
#endif

#endif
