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

pzkContainer * pz[9];
real pwf, w0 = 1000, dw = 1000;
uint i;
complex tmp;

void main(void)
{
	filterR = newFilterInfo();
#ifdef _COMPILE_WITH_BLACKFIN
	//resetFilter();
	Init_Device();

	while(1) {
#endif
		if( changeFilterRequest ) {
			changeFilterRequest = 0;
			strcpy(uart_buffer, "GI:C2:N5W2B0.001L0*T:LP:A1000W0*D:M*");	//GI:BW:A3w2B40*T:BP:A1000B5000*D:M*	
			/*decodeInput(uart_buffer);*/
			changeState(uart_buffer);
			printErrors(uart_buffer, UART_BUF_SIZE);
			printf("Errors:\n");
			printf(uart_buffer);
			printFilterInfo(&filterR);
			for(i=0; i<COEFF_SIZE; i++) {
					printf("%d - %d\n", delayLineR[i], coeffLineR[i]);
			}
			//pwf = getPrewarpFreq(w0, 1/F_SAMPLING);
			//printf("%d - %g\n", sizeof(pzkContainer), pwf);
			//pz[0] = createChebyshev2(5, 10.0/6.0, 0.01778);
			//pz[0] = createChebyshev1(5, 0.1);
			//pz[0] = createButterworth(3, 1);
			/*pz[0] = createPzkContainer(2, 0);
			tmp.im = 0;
			tmp.re = 1;
			addPole(pz[0],tmp);
			tmp.re = 2;
			addPole(pz[0],tmp);
			tmp.re = 0;
			addZero(pz[0],tmp);
			pz[0]->amp = 2;*/

			/*pz[1] = t2lp(pz[0] , w0);
			pz[2] = t2hp(pz[0] , w0);
			pz[3] = t2bp(pz[0] , w0, dw);
			pz[4] = t2bs(pz[0] , w0, dw);
			
			
			for(i = 5; i < 9; i++) {
				sortPzkContainer(pz[i-4]);
				pz[i] = bilinear(pz[i-4],F_SAMPLING,w0);
			}
			for(i = 1; i < 5; i++) {
				printPzkContainer(pz[i]);
				print4Matlab(pz[i]);
				print4Matlab(pz[i+4]);
			}*/
		}
#ifdef _COMPILE_WITH_BLACKFIN
	}
#endif
}
