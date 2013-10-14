#include "headers/variables.h"
#include "headers/communication.h"
#include "headers/diagnostics.h"

#ifndef _COMPILE_WITH_BLACKFIN

char buff[256];	// = ">gi:Rf:BW:N4*DP:B*TP:LP:C1000";
int i;

void main( int argc, char *argv[] )
{

	calibrateClock();

	for( i=1; i<argc; i++ ) {
		parseInput(argv[i], 256);
	}

	while(1) {
		scanf("%s", buff);
		parseInput(buff, 256);
	}

}

#endif
