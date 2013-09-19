#ifndef _DIAGNOSTICS_H
#define _DIAGNOSTICS_H

//--------------------------------------------------------------------------------------------------------
// Time
//--------------------------------------------------------------------------------------------------------

#define CLOCK_CALIBRATION_LENGTH	16

#include <time.h>

void calibrateClock();
void startClock();
void stopClock();

void setTick();
void printCpuUsage();

//--------------------------------------------------------------------------------------------------------
// Memory usage
//--------------------------------------------------------------------------------------------------------

void setMem( int mem_delay, int mem_coeff );
void printMemoryUsage();

//--------------------------------------------------------------------------------------------------------
// Other
//--------------------------------------------------------------------------------------------------------

//void printActualFilter();
//printRoutingInfo();
int printCodeWord();


/*
void printPzkContainer(pzkContainer * pzk);
void print4Matlab(pzkContainer * pzk);
void simulateFilter(const filterInfo * fi, const char * simfile);*/

int debugFilterInfo();

#endif
