#include <stdlib.h>
#include <stdio.h>
#include "global.h"

pzkContainer * createPzkContainer(uint np, uint nz) {
	pzkContainer * pzk;
	
	pzk = (pzkContainer *) malloc(sizeof(pzkContainer));
	
	if( nz != 0 ) {
		pzk->zeros = (complex *) malloc(nz * sizeof(complex));	//hibakezelés jó lenne
	} else {
		pzk->zeros = NULL;
	}
	
	if( np != 0 ) {
		pzk->poles = (complex *) malloc(np * sizeof(complex));	//ide is
	} else {
		pzk->poles = NULL;
	}
	
	pzk->amp			= 1.0;
	pzk->wz			= 0.0;
	pzk->pwf			= 0.0;
	pzk->zSize		= nz;
	pzk->pSize		= np;
	pzk->nextPole	= 0;
	pzk->nextZero	= 0;
	pzk->no_wz		= 0;
	
	return pzk;
}

uint shrinkPzkContainer(pzkContainer * pzk, uint min) {
	void * tmp;
	uint freed = 0;
	
	if( pzk->pSize > pzk->nextPole + min ) {
		tmp = realloc( pzk->poles, pzk->nextPole * sizeof(complex) );	//TODO!
		pzk->poles = (complex *) tmp;
		freed += pzk->pSize - pzk->nextPole;
		pzk->pSize = pzk->nextPole;
	}
	
	if( pzk->zSize > pzk->nextZero + min ) {
		tmp = realloc( pzk->zeros, pzk->nextZero * sizeof(complex) );	//TODO!
		pzk->zeros = (complex *) tmp;
		freed += pzk->zSize - pzk->nextZero;
		pzk->zSize = pzk->nextZero;
	}
	
	return freed;
}

void deletePzkContainer(pzkContainer * pzk) {
	free(pzk->poles);
	free(pzk->zeros);
	free(pzk);
	pzk = NULL;
}

void printPzkContainer(pzkContainer * pzk) {
	unsigned int i;

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

	printf("---------------------------------------------------------\n");
}

void print4Matlab(pzkContainer * pzk) {
	unsigned int i;
	int tmp;
	complex ctmp1, ctmp2;

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
		if(pzk->wz == 0 ) {
			tmp = (pzk->pwf) ? 1 : 0;
			for(i=0; i<pzk->no_wz; i++) {
				printf("%d ", tmp);
			}
		} else {
			if( pzk->pwf ) {
				ctmp1.re = pzk->pwf;
				ctmp1.im = pzk->wz;
				ctmp2.re = pzk->pwf;
				ctmp2.im = -pzk->wz;
				ctmp1 = cdiv(ctmp1, ctmp2);
				for(i=0; i<pzk->no_wz; i++) {
					printf("%.10g%+.10gi %.10g%+.10gi ", ctmp1.re, ctmp1.im, ctmp1.re, -ctmp1.im);
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
	if(pzk->no_wz < 0) {
		if(pzk->wz == 0 ) {
			tmp = (pzk->pwf) ? -1 : 0;
			for(i=0; i<-pzk->no_wz; i++) {
				printf("%d ", tmp);
			}
		} else {
			if( pzk->pwf ) {
				ctmp1.re = pzk->pwf;
				ctmp1.im = pzk->wz;
				ctmp2.re = pzk->pwf;
				ctmp2.im = -pzk->wz;
				ctmp1 = cdiv(ctmp1, ctmp2);
				for(i=0; i<-pzk->no_wz; i++) {
					printf("%.10g%+.10gi %.10g%+.10gi ", ctmp1.re, ctmp1.im, ctmp1.re, -ctmp1.im);
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

uint addPole(pzkContainer * pzk, complex pole) {
	if( cisnull(pole) && pzk->wz == 0.0 ) {
		pzk->no_wz--;
		return 1;
	}
	else if( pzk->pSize > pzk->nextPole ) {
		pzk->poles[pzk->nextPole++] = pole;
		return 1;
	}
	else {
		return 0;
	}
}

uint addZero(pzkContainer * pzk, complex zero) {
	if( cisnull(zero) && pzk->wz == 0.0 ) {
		pzk->no_wz++;
		return 1;
	}
	else if( pzk->zSize > pzk->nextZero ) {
		pzk->zeros[pzk->nextZero++] = zero;
		return 1;
	}
	else {
		return 0;
	}
}

uint countPoles(pzkContainer * pzk) {
	uint np = pzk->nextPole;	// number of poles	
	int i;
	
	for( i = 0; i < pzk->nextPole; i++ ) {
		if( !cisreal(pzk->poles[i]) ) {
			np++;						// complex means two poles
		}
	}
	
	if( pzk->no_wz < 0 ) {
		np += -pzk->no_wz;
		if( pzk->wz != 0.0 ) {
			np += -pzk->no_wz;	// not null means complex => 2 poles
		}
	}
	return np;
}

uint countZeros(pzkContainer * pzk) {
	uint nz = pzk->nextZero;	// number of zeros
	int i;
	
	for( i = 0; i < pzk->nextZero; i++ ) {
		if( !cisreal(pzk->zeros[i]) ) {
			nz++;						// complex means two zeros
		}
	}
	
	if( pzk->no_wz > 0 ) {
		nz += pzk->no_wz;
		if( pzk->wz != 0 ) {
			nz += pzk->no_wz;	// not null means complex => 2 zeros
		}
	}
	return nz;
}

uint countBiquads(pzkContainer * pzk) {
	uint np, nz;
	
	np = countPoles(pzk);
	nz = countZeros(pzk);
	
	if( np > nz ) { 
		nz = np;		// nz = max{np,nz}
	}
	
	return (nz + 1)/2;
	
}
