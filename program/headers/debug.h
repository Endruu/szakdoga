#ifndef _DEBUG_H
#define _DEBUG_H

#define COPY_COMMENTS		2		// 0 - dont, 1 - mark, 2 - copy
#define	TC_CALL_MAX_DEPTH	3

int runTestcase( char * tcname );
int debugFilterInfo(char * s, int l );

simulationParameters defaultPulseSimulationParameters();

simulationParameters defaultSinusSimulationParameters();

int simulatePSRespose( char * s, int l, char sinus );
#endif
