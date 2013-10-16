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
	char buffer[50];
	filterInfo * fi = &filterBank[actualFilter];	
	
	sprintf( buffer, "CPU usage: %d ticks - %.2f%%%%\n", fi->ticks, (float)(fi->ticks * 100)*F_SAMPLING/(float)(CPU_FREQ) );
	out( buffer );
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
	char buffer[50];
	filterInfo * fi = &filterBank[actualFilter];
	
	sprintf( buffer, "Memory usage:\n");
	out(buffer);
	sprintf( buffer, "Delay: %d bytes - %.2f%%%%\n", fi->mem_delay, (float)(fi->mem_delay * 100)/(float)(DELAY_SIZE));
	out(buffer);
	sprintf( buffer, "Coeff: %d bytes - %.2f%%%%\n", fi->mem_coeff, (float)(fi->mem_coeff * 100)/(float)(COEFF_SIZE));
	out(buffer);
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
	return 0;
}