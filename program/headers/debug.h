#ifdef DEBUG_FUNCTIONS_ENABLED

#ifndef _DEBUG_H
#define _DEBUG_H

int runTestcase( char * tcname );


int debugFilterInfo(char * s, int l );
void printPzkContainer(pzkContainer * pzk);
void printBiquadList(biquad * bList, pzkContainer * pzk);


void getTimeStamp( char * s );

simulationParameters defaultPulseSimulationParameters();
simulationParameters defaultSinusSimulationParameters();

int simulatePSRespose( char * s, int l, char sinus );
int simulatePulse( const char * filename, simulationParameters sp );
int simulateSinus( const char * filename, simulationParameters sp );

#endif

#endif

