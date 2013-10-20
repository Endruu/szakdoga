#include "../headers/variables.h"
#include "../headers/diagnostics.h"
#include "../headers/pzk_container.h"
#include "../headers/emath.h"

clock_t tickCounter, tickDelay;


/*

void print4Matlab(pzkContainer * pzk) {
	unsigned int i;

	if(pzk == NULL) {
		return;
	}

	printf("k = %10g;\n", pzk->amp);

	printf("z = [ ");
	if(pzk->nextZero) {
		for(i=0; i<pzk->nextZero;i++) {
			printf("%.10g%+.10gi ", pzk->zeros[i].re, pzk->zeros[i].im);
			if(pzk->zeros[i].im != 0) {
				printf("%.10g%+.10gi ", pzk->zeros[i].re, -pzk->zeros[i].im);
			}
		}
	}
	if(pzk->no_wz > 0) {
		if( pzk->wz != DIGITAL_FILTER ) {
			if(pzk->wz == 0 ) {
				for(i=0; i<pzk->no_wz; i++) {
					printf("0 ");
				}
			} else {
				for(i=0; i<pzk->no_wz; i++) {
					printf("%.10gi %.10gi ", pzk->wz, -pzk->wz);
				}
			}
		}
	}
	printf("];\n");

	printf("p = [ ");
	if(pzk->nextPole) {
		for(i=0; i<pzk->nextPole;i++) {
			printf("%.10g%+.10gi ", pzk->poles[i].re, pzk->poles[i].im);
			if(pzk->poles[i].im != 0) {
				printf("%.10g%+.10gi ", pzk->poles[i].re, -pzk->poles[i].im);
			}
		}
	}
	if( pzk->wz != DIGITAL_FILTER ) {
		if(pzk->no_wz < 0) {
			if(pzk->wz == 0 ) {
				for(i=0; i<-pzk->no_wz; i++) {
					printf("0 ");
				}
			} else {
				for(i=0; i<-pzk->no_wz; i++) {
					printf("%.10gi %.10gi ", pzk->wz, -pzk->wz);
				}
			}
		}
	}
	printf("];\n\n");
}
*/
//--------------------------------------------------------------------------------------------------------
// Time
//--------------------------------------------------------------------------------------------------------

void calibrateClock() {
	unsigned int sum =0, i;
	tickDelay = 0;
	
	for( i = 0; i < CLOCK_CALIBRATION_LENGTH; i++ ) {
		startClock();
		stopClock();
		sum += tickCounter;
	}
	
	tickDelay = sum / CLOCK_CALIBRATION_LENGTH;
}

void startClock() {
	tickCounter = clock();
}
void stopClock() {
	tickCounter =  clock() - tickCounter - tickDelay;
}

void setTick() {
	filterInfo * fi = &filterBank[actualFilter];
	fi->ticks = ( fi->ticks + tickCounter ) / 2;
}

void printCpuUsage() {
	filterInfo * fi = &filterBank[actualFilter];	
	
	sprintf( inputBuffer, "CPU usage: %d ticks - %.2f%%%%\n", fi->ticks, (float)(fi->ticks * 100)*F_SAMPLING/(float)(CPU_FREQ) );
	out( inputBuffer );
}

//--------------------------------------------------------------------------------------------------------
// Memory usage
//--------------------------------------------------------------------------------------------------------

void setMem( int mem_delay, int mem_coeff ) {
	filterInfo * fi = &filterBank[actualFilter];
	fi->mem_delay = mem_delay;
	fi->mem_coeff = mem_coeff;
}

void printMemoryUsage() {
	filterInfo * fi = &filterBank[actualFilter];
	
	sprintf( inputBuffer, "Memory usage:\n");
	out(inputBuffer);
	sprintf( inputBuffer, "Delay: %d bytes - %.2f%%%%\n", fi->mem_delay, (float)(fi->mem_delay * 100)/(float)(DELAY_SIZE));
	out(inputBuffer);
	sprintf( inputBuffer, "Coeff: %d bytes - %.2f%%%%\n", fi->mem_coeff, (float)(fi->mem_coeff * 100)/(float)(COEFF_SIZE));
	out(inputBuffer);
}

//--------------------------------------------------------------------------------------------------------
// Other
//--------------------------------------------------------------------------------------------------------

/*
void printActualChannel() {
	char buffer[25];
	
	sprintf( buffer, "Actual channel: %d\n", channelSelect);
	out(buffer);
}
*/

int printCodeWord() {
	const filterInfo fi = filterBank[actualFilter];
	real ws = fi.iirP.ws;

	if( fi.iirP.fixWs ) {
		ws = 1 / ws;
	}

	out(">GI:");
	
	// Referent ------------------------------------------------
	out("RF:");
	switch( fi.subtype ) {
		case butterworth : 
			out("BW:");
			break;
		case chebyshev1 : 
			out("C1:");
			break;
		case chebyshev2 : 
			out("C2:");
			break;
	}

	if( fi.iirP.ac && fi.iirP.as ) {
		sprintf(inputBuffer, "C%.5g,S%.5g,W%.5g", fi.iirP.ac, fi.iirP.as, ws);
	} else if( fi.iirP.ac && fi.iirP.n ) {
		sprintf(inputBuffer, "N%dC%.5g", fi.iirP.n, fi.iirP.ac);
	} else if( fi.iirP.as && fi.iirP.n ) {
		sprintf(inputBuffer, "N%d,S%.5g,W%.5g", fi.iirP.n, fi.iirP.as, ws);
	} else {
		sprintf(inputBuffer, "N%d", fi.iirP.n);
	}
	out(inputBuffer);

	if( fi.iirP.inDb == 0 ) {
		out(",L");
	}

	// Transform -----------------------------------------------
	out("*TP:");
	switch( fi.type ) {
		case lowpass :
			out("LP");
			break;
		case highpass :
			out("HP");
			break;
		case bandpass :
			out("BP");
			break;
		case bandstop :
			out("BS");
			break;
	}
	sprintf(inputBuffer, ":W,C%.3g", fi.transformP.w0);
	out(inputBuffer);
	if( fi.type == bandpass || fi.type == bandstop ) {
		sprintf(inputBuffer, ",D%.3g", fi.transformP.w0);
		out(inputBuffer);
	}

	// Digitalization ------------------------------------------
	out("*DP:");
	if( fi.warping == WARP_FACTOR ) {
		out("B");
	} else if( fi.warping == WARP_FREQUENCY ) {
		out("P");
	} else if( fi.warping == WARP_AUTO_FIX ) {
		out("C");
	} else {
		sprintf(inputBuffer, "W%.3g", fi.warping);
		out(inputBuffer);
	}

	// Implementation ------------------------------------------
	out("*IP:S");
	if( fi.implementP.sort == SORT_BY_QFACTOR ) {
		out("Q");
	} else {
		out("F");
	}

	out(",O");
	if( fi.implementP.order == ORDER_UP ) {
		out("U");
	} else {
		out("D");
	}

	out(",P");
	if( fi.implementP.pair == PAIR_POLES_TO_ZEROS ) {
		out("Z");
	} else {
		out("P");
	}

	out(",F");
	sprintf(inputBuffer, "%d\n", fi.implementP.filter);
	out(inputBuffer);

	return 1;
}