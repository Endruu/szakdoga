#include "global.h"

#ifdef _COMPILE_WITH_BLACKFIN

void main(void)
{
	filterR = newFilterInfo();
	Init_Device();

	while(1) {
		if( uartRequest == 1 ) {
			changeState(uart_buffer);
			if( getErrors() ) {
				printErrors(uart_buffer, UART_BUF_SIZE);
				//printf("%s\n", uart_buffer);
				clearErrors();
			} else {
				//printf("OK\n");
			}
			uartRequest = 0;
		}
	}
}

#endif
