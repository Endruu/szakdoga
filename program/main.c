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

void main(void)
{

#ifdef _COMPILE_WITH_BLACKFIN
	resetFilter();
	Init_All();
	CYCLES_INIT(sOneTime);

	while(1) {
#endif	
		if( changeFilterRequest ) {
			changeFilterRequest = 0;
			/*decodeInput(uart_buffer);*/
			//CYCLES_START(sOneTime);
			pwf = getPrewarpFreq(w0, 1/F_SAMPLING);
			printf("%d - %g\n", sizeof(pzkContainer), pwf);
			//pz[0] = createChebyshev2(5, 10.0/6.0, 0.01778);
			pz[0] = createChebyshev1(5, 0.1);
			//pz[0] = createButterworth(5, 1);
			pz[1] = t2lp(pz[0] , w0);
			pz[2] = t2hp(pz[0] , w0);
			pz[3] = t2bp(pz[0] , w0, dw);
			pz[4] = t2bs(pz[0] , w0, dw);
			//CYCLES_STOP(sOneTime);
			
			
			for(i = 5; i < 9; i++) {
				pz[i] = bilinear(pz[i-4],pwf);
			}
			for(i = 1; i < 5; i++) {
				//printPzkContainer(pz[i]);
				print4Matlab(pz[i]);
				print4Matlab(pz[i+4]);
			}
			//CYCLES_PRINT(sOneTime);
		}
#ifdef _COMPILE_WITH_BLACKFIN
	}
#endif
}

/*
#include "LabFrameFIR.h"

#define N 20		// number of filter coefficients
float coefs[N] = {	// Filter coeffs
#include "mova_float.dat"
};



float input[N];
int output[N];
int i = 0;		

//--------------------------------------------------------------------------//
// Function:	Process_Data()												//
//																			//
// Description: This function is called from inside the SPORT0 ISR every 	//
//				time a complete audio frame has been received. The new 		//
//				input samples can be found in the variables iChannel0LeftIn,//
//				iChannel0RightIn, iChannel1LeftIn and iChannel1RightIn 		//
//				respectively. The processed	data should be stored in 		//
//				iChannel0LeftOut, iChannel0RightOut, iChannel1LeftOut,		//
//				iChannel1RightOut, iChannel2LeftOut and	iChannel2RightOut	//
//				respectively.												//
//--------------------------------------------------------------------------//
// The function below is capable to execute an FIR filter up to 10 (!)      //
// coefficents only.                                                        //
//--------------------------------------------------------------------------//

void Process_Data(void)
{
	
	iChannel0LeftOut = iChannel0RightIn;	
		
	iChannel0RightIn = iChannel0RightIn <<8;
	input[i] = (float) iChannel0RightIn;	

				
	int j;
	int ix = i;
	float fout = 0;
	int out;		
	
	// Filter loop
	for (j=0;j<N;j++){
		fout += input[ix++%N] * coefs[j]; 
	}
	
	out = (int) fout;				
	output[i++] = out;	
	
	if (i==N)
	{
		i = 0;
	}
	
	iChannel0RightOut = out >> 8;	
	
}
*/
