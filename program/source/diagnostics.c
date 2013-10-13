#include "../headers/variables.h"
#include "../headers/diagnostics.h"
#include "../headers/pzk_container.h"
#include "../headers/emath.h"

clock_t tickCounter, tickDelay;

void printBiquadList(biquad * bList, pzkContainer * pzk) {
	const int limit = countBiquads(pzk);
	int i;
	char bfr[50];
	char pz;

	for( i=0; i<limit; i++ ) {
		out("\n-----------------------------\n");

		pz = bList[i].p1;
		out("p1: ");
		if( pz == EMPTY_PAIR ) {
			sprintf(bfr, "INF\n");
		} else if( pz == WZ_PAIR ) {
			if( pzk->wz < 0 ) {
				sprintf(bfr, "%10g + %gi\n", cos(pzk->wz), -sin(pzk->wz));
			} else {
				sprintf(bfr, "%10g + %gi\n", 0.0, pzk->wz);
			}
		} else {
			sprintf(bfr, "%10g + %gi\n", pzk->poles[pz].re, pzk->poles[pz].im);
		}
		out(bfr);

		pz = bList[i].p2;
		out("p2: ");
		if( pz == EMPTY_PAIR ) {
			sprintf(bfr, "INF\n");
		} else if( pz == WZ_PAIR ) {
			if( pzk->wz < 0 ) {
				sprintf(bfr, "%10g - %gi\n", cos(pzk->wz), -sin(pzk->wz));
			} else {
				sprintf(bfr, "%10g - %gi\n", 0.0, pzk->wz);
			}
		} else {
			sprintf(bfr, "%10g - %gi\n", pzk->poles[pz].re, pzk->poles[pz].im);
		}
		out(bfr);

		pz = bList[i].z1;
		out("z1: ");
		if( pz == EMPTY_PAIR ) {
			sprintf(bfr, "INF\n");
		} else if( pz == WZ_PAIR ) {
			if( pzk->wz < 0 ) {
				sprintf(bfr, "%10g + %gi\n", cos(pzk->wz), -sin(pzk->wz));
			} else {
				sprintf(bfr, "%10g + %gi\n", 0.0, pzk->wz);
			}
		} else {
			sprintf(bfr, "%10g + %gi\n", pzk->zeros[pz].re, pzk->zeros[pz].im);
		}
		out(bfr);

		pz = bList[i].z2;
		out("z2: ");
		if( pz == EMPTY_PAIR ) {
			sprintf(bfr, "INF\n");
		} else if( pz == WZ_PAIR ) {
			if( pzk->wz < 0 ) {
				sprintf(bfr, "%10g - %gi\n", cos(pzk->wz), -sin(pzk->wz));
			} else {
				sprintf(bfr, "%10g - %gi\n", 0.0, pzk->wz);
			}
		} else {
			sprintf(bfr, "%10g - %gi\n", pzk->zeros[pz].re, pzk->zeros[pz].im);
		}
		out(bfr);
	}
	
	out("\n----------------------------------------------\n");
}

void printPzkContainer(pzkContainer * pzk) {
	unsigned int i;
	char bfr[60];

	if(pzk == NULL) {
		return;
	}

	sprintf(bfr, "Amplification: %g\n", pzk->amp);
	out(bfr);

	if(pzk->nextZero) {
		sprintf(bfr, "Zeros: (%u)\n", countZeros(pzk));
		out(bfr);
		for(i=0; i<pzk->nextZero;i++) {
			sprintf(bfr, "  %10g + %gi\n", pzk->zeros[i].re, pzk->zeros[i].im);
			out(bfr);
			if( pzk->wz >= 0 ) {
				if( pzk->zeros[i].re != 0 ) {
					sprintf(bfr, "\t( abs: %3g - Q: %3g )\n", cabs(pzk->zeros[i]), 0.5 * cabs(pzk->zeros[i]) / fabs(pzk->zeros[i].re));
					out(bfr);
				}
			}
		}
	}

	if(pzk->nextPole) {
		sprintf(bfr, "Poles: (%u)\n", countPoles(pzk));
		out(bfr);
		for(i=0; i<pzk->nextPole;i++) {
			sprintf(bfr, "  %10g + %gi\n", pzk->poles[i].re, pzk->poles[i].im);
			out(bfr);
			if( pzk->wz >= 0 ) {
				if( pzk->poles[i].re != 0 ) {
					sprintf(bfr, "\t( abs: %3g - Q: %3g )\n", cabs(pzk->poles[i]), 0.5 * cabs(pzk->poles[i]) / fabs(pzk->poles[i].re));
					out(bfr);
				}
			}
		}
	}
	
	sprintf(bfr, "Biquads: %u\n", countBiquads(pzk));
	out(bfr);

	if( pzk->wz != 0 ) {
		out("Number of ");
		if(pzk->wz == 0.0 || pzk->wz == DIGITAL_ZERO ) {
			if(pzk->no_wz > 0) {
				sprintf(bfr, "differentiators: %d\n", pzk->no_wz);
			}
			else if(pzk->no_wz < 0) {
				sprintf(bfr, "integrators: %d\n", -pzk->no_wz);
			}
		} else {
			if( pzk->wz < 0 ) {			// DIGITAL
				if(pzk->no_wz > 0) {
					sprintf(bfr, "e^j%g conjugate zero pairs: %d\n", -pzk->wz, pzk->no_wz);
				} else {
					sprintf(bfr, "e^j%g conjugate pole pairs: %d\n", -pzk->wz, pzk->no_wz);
				}
			} else {
				if(pzk->no_wz > 0) {
					sprintf(bfr, "j%g conjugate zero pairs: %d\n", pzk->wz, pzk->no_wz);
				} else {
					sprintf(bfr, "j%g conjugate pole pairs: %d\n", pzk->wz, pzk->no_wz);
				}
			}
		}
		out(bfr);
	}

	out("\n----------------------------------------------\n");
}


/*
void simulateFilter(const filterInfo * fi, const char * simfile ) {

}

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