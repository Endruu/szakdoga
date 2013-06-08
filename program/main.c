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
FILE * f;

void main(void)
{
	filterR = newFilterInfo();
#ifdef _COMPILE_WITH_BLACKFIN
	Init_Device();

	while(1) {
#endif
		if( uartRequest ) {
			//strcpy(uart_buffer, "GI:bw:N3*T:bs:A1000B20000*D:M*");	//GI:BW:A3w2B40*T:BP:A1000B5000*D:M*	
			/*decodeInput(uart_buffer);*/
			changeState(uart_buffer);
			//printPzkContainer(filterR.tFilter);
			if( getErrors() ) {
				printErrors(uart_buffer, UART_BUF_SIZE);
				printf("%s\n", uart_buffer);
				clearErrors();
			} else {
				printf("OK\n");
			}
			//printFilterInfo(&filterR);
			/*CLI();
			printf("HALT!\n");
			STI();*/
			/*printErrors(uart_buffer, UART_BUF_SIZE);
			printf("Errors:\n");
			printf(uart_buffer);*/
/*
			f = fopen("pulse.txt","w");
			fprintf(f, "%d\n", (*filterR.filter)(0x7FFF, coeffLineR, delayLineR) >> 8);
			for(i=0; i<2047; i++) {
				fprintf(f, "%d\n", (*filterR.filter)(0, coeffLineR, delayLineR) >> 8);
			}
			fclose(f);
			for(i=0; i<30; i++) {
				printf("%g\n", (float)coeffLineR[i]/32768.0);
			}*/
			uartRequest = 0;
		}
#ifdef _COMPILE_WITH_BLACKFIN
	}
#endif
}
