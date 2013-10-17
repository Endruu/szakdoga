#include <stdlib.h>

#include "../headers/pzk_container.h"
#include "../headers/error.h"
#include "../headers/emath.h"

pzkContainer * createPzkContainer(uint np, uint nz) {
	pzkContainer * pzk;
	
	pzk = (pzkContainer *) malloc(sizeof(pzkContainer));
	if( pzk == NULL ) {
		errorR(37, NULL);
	}
	
	if( nz != 0 ) {
		pzk->zeros = (complex *) malloc(nz * sizeof(complex));
		if( pzk->zeros == NULL ) {
			free(pzk);
			errorR(38, NULL);
		}
	} else {
		pzk->zeros = NULL;
	}
	
	if( np != 0 ) {
		pzk->poles = (complex *) malloc(np * sizeof(complex));
		if( pzk->poles == NULL ) {
			free(pzk->zeros);
			free(pzk);
			errorR(39, NULL);
		}
	} else {
		pzk->poles = NULL;
	}
	
	pzk->amp		= 1.0;
	pzk->wz			= 0.0;
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

pzkContainer * deletePzkContainer(pzkContainer * pzk) {
	if(pzk == NULL) {
		return NULL;
	}
	free(pzk->poles);
	free(pzk->zeros);
	free(pzk);
	return NULL;
}


uint addPole(pzkContainer * pzk, complex pole) {
	if( cisnull(pole) && pzk->wz == 0.0 ) {
		pzk->no_wz--;
		return 1;
	}
	else if( pzk->pSize > pzk->nextPole ) {
		if( pole.im < 0 ) {
			pole.im = -pole.im;
		}
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
		if( zero.im < 0 ) {
			zero.im = -zero.im;
		}
		pzk->zeros[pzk->nextZero++] = zero;
		return 1;
	}
	else {
		return 0;
	}
}


uint countPoles(pzkContainer * pzk) {
	uint np = pzk->nextPole;				// number of poles	
	int i;
	
	for( i = 0; i < pzk->nextPole; i++ ) {
		if( !cisreal(pzk->poles[i]) ) {
			np++;							// complex means two poles
		}
	}
	
	if( pzk->no_wz < 0 ) {
		np -= pzk->no_wz;
		if( pzk->wz != 0.0 ) {
			np -= pzk->no_wz;				// not null means complex => 2 poles
		}
	}
	return np;
}

uint countZeros(pzkContainer * pzk) {
	uint nz = pzk->nextZero;				// number of zeros
	int i;
	
	for( i = 0; i < pzk->nextZero; i++ ) {
		if( !cisreal(pzk->zeros[i]) ) {
			nz++;							// complex means two zeros
		}
	}
	
	if( pzk->no_wz > 0 ) {
		nz += pzk->no_wz;
		if( pzk->wz != 0 ) {
			nz += pzk->no_wz;				// not null means complex => 2 zeros
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


int compareMagnitude( complex c1, complex c2 ) {
	// return	1 if c1 is greater (abs)
	//			0 if equal
	//			-1 if c1 is less

	if( cabs(c1) > cabs(c2) ) {
		return 1;
	}
	if( cabs(c1) < cabs(c2) ) {
		return -1;
	}
	return 0;
}

int compareQFactor( complex c1, complex c2 ) {
	// return	1 if c1 is greater (Q)
	//			0 if equal
	//			-1 if c1 is less

	real q1, q2;

	if( cisnull(c1) ) return -1;
	if( cisnull(c2) ) return 1;

	q1 = cabs(c1) / fabs(c1.re);
	q2 = cabs(c2) / fabs(c2.re);

	if( q1 > q2 ) {
		return 1;
	}
	if( q1 < q2 ) {
		return -1;
	}
	return 0;
}

void reverseComplexList(complex * list, const uint num) {
	complex tmp;
	int i;

	for( i = 0; i < num/2; i++ ) {
		tmp = list[i];
		list[i] = list[num-i-1];
		list[num-i-1] = tmp;
	}
}

uint findInsertionPoint(real im, complex * list, uint num, char method) {
	uint i, pos = num;

	if( im == 0 || num == 0) { return 0; }

	if( method == SORT_BY_QFACTOR ) {
		for( i = num-1; i >= 0; i-- ) {
			if( list[i].re == 0  &&  list[i].im > im ) {
				pos--;
			} else {
				return pos;
			}
		}
	}
	else if( method == SORT_BY_MAGNITUDE ) {
		for( i = num-1; i >= 0; i-- ) {
			if( cabs( list[i] ) > im ) {
				pos--;
			} else {
				return pos;
			}
		}
	}
	return pos;
}

void sortComplexList(complex * list, uint num, char method) {
	// order: up
    uint	pos = 1,
			last = 0;
	complex tmp;
	int (*primary)(complex c1, complex c2);
	int (*secondary)(complex c1, complex c2);

	if( method == SORT_BY_MAGNITUDE ) {
		primary = &compareMagnitude;
		secondary = &compareQFactor;
	} else if( method == SORT_BY_QFACTOR ) {
		primary = &compareQFactor;
		secondary = &compareMagnitude;
	}

    while( pos < num ) {
        if ( primary(list[pos], list[pos-1]) == 1 ) {
            if (last != 0) {
                pos = last;
                last = 0;
            }
            pos++;
		}
		
		else if ( primary(list[pos], list[pos-1]) == 0 ) {
			if ( secondary(list[pos], list[pos-1]) >= 0 ) {
				if (last != 0) {
					pos = last;
					last = 0;
				}
				pos++;
			}
			else {
				tmp = list[pos];
				list[pos] = list[pos-1];
				list[pos-1] = tmp;
				if (pos > 1) {
					if (last == 0) {
						last = pos;
					}
					pos--;
				}
				else {
					pos++;
				}
			}
		}
		
        else {
            tmp = list[pos];
			list[pos] = list[pos-1];
			list[pos-1] = tmp;
            if (pos > 1) {
                if (last == 0) {
                    last = pos;
                }
                pos--;
			}
            else {
                pos++;
            }
        }
    }
}

uint sortPzkContainer(pzkContainer * pzk, char sort, char order) {
	uint ip = 0;

	sortComplexList(pzk->zeros, pzk->nextZero, sort);
	sortComplexList(pzk->poles, pzk->nextPole, sort);

	if( pzk->no_wz > 0 ) {
		ip = findInsertionPoint( pzk->wz, pzk->zeros, pzk->nextZero, sort );
	} else if( pzk->no_wz < 0 ) {
		ip = findInsertionPoint( pzk->wz, pzk->poles, pzk->nextPole, sort );
	}

	if( order == ORDER_DOWN ) {
		reverseComplexList( pzk->zeros, pzk->nextZero );
		reverseComplexList( pzk->poles, pzk->nextPole );
		if( pzk->no_wz > 0 ) {
			ip = pzk->nextZero - ip;
		} else if( pzk->no_wz < 0 ) {
			ip = pzk->nextPole - ip;
		}
	}
	return ip;
}

