#include "headers/variables.h"
#include "headers/communication.h"
#include "headers/diagnostics.h"

#ifdef _COMPILE_WITH_BLACKFIN

void main(void)
{
	calibrateClock();
	initializeDevice();

	while(1) {
		if( pendingCommand ) {
		
			if( pendingCommand == LONG_COMMAND ) {
				// error
			} else {
				parseInput(inputBuffer, INPUT_BUF_SIZE);
			}
			
			
			
		}
	}
}

#endif
