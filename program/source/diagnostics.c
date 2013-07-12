#include "../headers/global.h"

void simulateFilter(const filterInfo * fi, const char * simfile ) {

}

void printPzkContainer(pzkContainer * pzk) {
	unsigned int i;

	if(pzk == NULL) {
		return;
	}

	printf("Amplification: %g\n", pzk->amp);

	if(pzk->nextZero) {
		printf("Zeros: (%u)\n", countZeros(pzk));
		for(i=0; i<pzk->nextZero;i++) {
			printf("  %10g + %gi\n", pzk->zeros[i].re, pzk->zeros[i].im);
		}
	}

	if(pzk->nextPole) {
		printf("Poles: (%u)\n", countPoles(pzk));
		for(i=0; i<pzk->nextPole;i++) {
			printf("  %10g + %gi\n", pzk->poles[i].re, pzk->poles[i].im);
		}
	}
	
	printf("Biquads: %u\n", countBiquads(pzk));

	if( pzk->wz == DIGITAL_FILTER ) {
		printf("DIGITAL\n");
	} else {
		if(pzk->wz == 0.0) {
			if(pzk->no_wz > 0)
				printf("Number of differentiators: %d\n", pzk->no_wz);
			else if(pzk->no_wz < 0) {
				printf("Number of integrators: %d\n", -pzk->no_wz);
			}
		}
		else {
			if(pzk->no_wz > 0) {
				printf("Number of j%g conjugate zeros: %d\n", pzk->wz, pzk->no_wz);
			} else if(pzk->no_wz < 0) {
				printf("Number of j%g conjugate poles: %d\n", pzk->wz, pzk->no_wz);
			}
		}
	}

	printf("---------------------------------------------------------\n");
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