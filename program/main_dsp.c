#include "headers/variables.h"

#ifdef _COMPILE_WITH_BLACKFIN

#include "headers/communication.h"
#include "headers/diagnostics.h"
#include "headers/device/device.h"

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
			
			if( getErrors() ) {
				printErrors();
				clearErrors();
			}
			
		}
	}
}

#endif
