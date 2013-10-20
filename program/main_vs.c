#include "headers/variables.h"

#ifndef _COMPILE_WITH_BLACKFIN

#include "headers/communication.h"
#include "headers/diagnostics.h"

int i;

void main( int argc, char *argv[] )
{

	calibrateClock();

	for( i=1; i<argc; i++ ) {
		parseInput(argv[i], INPUT_BUF_SIZE);
		if( getErrors() ) {
			printErrors();
			clearErrors();
		}
	}

	while(1) {
		scanf("%s", inputBuffer);
		parseInput(inputBuffer, INPUT_BUF_SIZE);
		if( getErrors() ) {
			printErrors();
			clearErrors();
		}
	}

}

#endif
