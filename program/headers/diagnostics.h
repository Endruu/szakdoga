#ifndef _DIAGNOSTICS_H
#define _DIAGNOSTICS_H

#include "global_var.h"

//--------------------------------------------------------------------------------------------------------
// Time
//--------------------------------------------------------------------------------------------------------

#define CLOCK_CALIBRATION_LENGTH	16

#include <time.h>

clock_t tickCounter, tickDelay;

void calibrateClock();
void startClock();
void stopClock();

void setTick( filterInfo * fi );
void printTick( filterInfo * fi );

//--------------------------------------------------------------------------------------------------------
// Memory usage
//--------------------------------------------------------------------------------------------------------

void setMem( filterInfo * fi, int mem_delay, int mem_coeff );
void printMem( filterInfo * fi );

//--------------------------------------------------------------------------------------------------------
// Global variables
//--------------------------------------------------------------------------------------------------------

void printPzkContainer(pzkContainer * pzk);
void print4Matlab(pzkContainer * pzk);
void simulateFilter(const filterInfo * fi, const char * simfile);

#endif