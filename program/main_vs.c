#include "headers/global.h"

#ifndef _COMPILE_WITH_BLACKFIN

//#include <string.h>
#include <stdio.h>
clock_t st;

void main(void)
{
	filterR = newFilterInfo();
	

	/*while(1) {
		scanf("%s", uart_buffer);
		if( uart_buffer[1] == ':') {
			if ( uart_buffer[0] == 'f') {
				changeState( uart_buffer+2 );
				if( getErrors() ) {
					printErrors(uart_buffer, UART_BUF_SIZE);
					printf("%s\n", uart_buffer);
					clearErrors();
				} else {
					printf("OK\n");
				}
			}
			else if ( uart_buffer[0] == 's') {
				printf("ERROR: Not implemented yet!\n");
			}
			else if ( uart_buffer[0] == 'x') {
				break;
			}
			else {
				printf("ERROR: Input should start with 's:', 'f:', or 'x:'!\n");
			}
		} else {
			printf("ERROR: Input should start with 's:', 'f:', or 'x:'!\n");
		}
	}*/

	calibrateClock();
	startClock();
	st = clock();
	/*while( clock() - st < 5000 ) {
		printf(".");
	}*/
	stopClock();
	printf("\n%d\n", clock() - st);
	setTick(&filterR);
	printTick(&filterR);
}

#endif
