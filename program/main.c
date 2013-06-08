/*****************************************************************************
**																			**
**	 Name: 	BF537 C LabFrameFIR I2S										    **	
**																			**
******************************************************************************

File Name:	Main.c

Date Modified:	12/14/04	GO		Rev 1.0
				01/11/05	TL		Rev 1.1 Updated to work on Rev 1.1 of the 
				                    ADSP-BF537 EZ-Kit
				04/07/06			Modified for LabFrame				                    

Software:       VisualDSP++4.0

Hardware:	ADSP-BF537 EZ-KIT Board Rev 1.1
			Users of ADSP-BF537 EZ-KIT Board Rev 1.0 must change the SPORT to 
			generate internal clock and frame sync

Purpose:	This program sets up the ADSP-BF537 to reset the ADC and DAC.  
			The data to/from the DAC/ADC are transfered over SPORT0 in I2S mode.		                                          
				                                                                          
Program Parameters:                                                                                       
                                                                                                          

******************************************************************************/   
#include "global.h"
#include <stdio.h>
#include <string.h>


//--------------------------------------------------------------------------//
// Function:	main														//
//																			//
// Description:	After calling a few initalization routines, main() just 	//
//				waits in a loop forever.  The code to process the incoming  //
//				data can be placed in the function Process_Data() in the 	//
//				file "Process_Data.c".										//
//--------------------------------------------------------------------------//

int i;

void main(void)
{
	filterR = newFilterInfo();
#ifdef _COMPILE_WITH_BLACKFIN
	Init_Device();

	while(1) {
#endif
		if( uartRequest ) {
			strcpy(uart_buffer, "GI:BW:N3A3*T:LP:A1000*D:M*");	//GI:BW:A3w2B40*T:BP:A1000B5000*D:M*	
			/*decodeInput(uart_buffer);*/
			changeState(uart_buffer);
			//printPzkContainer(filterR.tFilter);
			printErrors(uart_buffer, UART_BUF_SIZE);
			printf("%s\n", uart_buffer);
			//printFilterInfo(&filterR);
			/*CLI();
			printf("HALT!\n");
			STI();*/
			/*printErrors(uart_buffer, UART_BUF_SIZE);
			printf("Errors:\n");
			printf(uart_buffer);*/
			
			for(i=0; i<100; i++) {
					printf("%d\n", (*filterR.filter)(32768, coeffLineR, delayLineR));
					//printf("%g\n", (float)coeffLineR[i]/32768.0);
			}
			uartRequest = 0;
		}
#ifdef _COMPILE_WITH_BLACKFIN
	}
#endif
}
