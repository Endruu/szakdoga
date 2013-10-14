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

//postSortComplexList(complex * list, uint num, char method, char order) {}

// returns 0 if c2 is closer to 1
// returns 1 if c1 is closer to 1
int compare1(complex c1, complex c2) {
	real a1, a2;								// magnitudes
	if( cisreal(c1) ) { a1 = fabs(c1.re); }
	else { a1 = cabs(c1); }
	if( cisreal(c2) ) { a2 = fabs(c2.re); }
	else { a2 = cabs(c2); }
	
	if( a1 < a2 ) return 1;						// if a1 is closer to 1
	return 0;
}

void sortDigitalPZ(pzkContainer * pzk) {
	int firstRealZero, firstRealPole, section1, section2;
	int i, j, k, l;
	real mind, tmp_mind;
	complex tmp;
	
	firstRealZero = preSortDigitalZeros(pzk);
	firstRealPole = sortDigitalPoles(pzk);
	section1 = min(firstRealZero, firstRealPole);
	section2 = max(firstRealZero, firstRealPole);
	
	for( i=0; i < section1; i++ ) {
		mind = cabs(csub(pzk->poles[i], pzk->zeros[i]));
		k = i;
		for( j=i+1; j < section1; j++ ) {
			tmp_mind = cabs(csub(pzk->poles[i], pzk->zeros[j]));
			if( mind > tmp_mind ) {
				mind = tmp_mind;
				k = j;
			}
		}
		tmp = pzk->zeros[i];
		pzk->zeros[i] = pzk->zeros[k];
		pzk->zeros[k] = tmp;
	}
	
	l = i;
	if( firstRealPole > firstRealZero ) {
		if( 1 ) {		// poles and zeros as close as they can
			for( ; i < section2; i++ ) {
				mind = MAX_REAL;
				for( j=l; j < pzk->nextZero; j++ ) {
					for( k=l; j < pzk->nextZero; k++ ) {
						tmp_mind = cabs(csub(pzk->poles[i], pzk->zeros[j])) + fabs( pzk->zeros[j].re - pzk->zeros[j].re);
						if( mind > tmp_mind && j != k ) {
							mind = tmp_mind;
							tmp.re = j;
							tmp.im = k;
						}
					}
				}
				j = (int)tmp.re;
				k = (int)tmp.im;
				tmp = pzk->zeros[l];
				pzk->zeros[l] = pzk->zeros[j];
				pzk->zeros[j] = tmp;
				tmp = pzk->zeros[l+1];
				pzk->zeros[l+1] = pzk->zeros[k];
				pzk->zeros[k] = tmp;
				l += 2;
			}
		}
		else {
			for( ; i < section2; i++ ) {
				mind = cabs(csub(pzk->poles[i], pzk->zeros[l]));
				k = l;
				for( j=l+1; j < pzk->nextZero; j++ ) {
					tmp_mind = cabs(csub(pzk->poles[i], pzk->zeros[j]));
					if( mind > tmp_mind ) {
						mind = tmp_mind;
						k = j;
					}
				}
				tmp = pzk->zeros[l];
				pzk->zeros[l] = pzk->zeros[k];
				pzk->zeros[k] = tmp;
				k = -1;
				if( 0 ) {	// keep negative pair
					for( j=l+1; j <  pzk->nextZero; j++ ) {
						if( pzk->zeros[j].re == -pzk->zeros[l].re ) {
							k = j;
							break ;
						}
					}
				}
				if( k == -1 ) {
					mind = cabs(csub(pzk->poles[i], pzk->zeros[l+1]));
					k = l+1;
					for( j=l+2; j < pzk->nextZero; j++ ) {
						tmp_mind = cabs(csub(pzk->poles[i+1], pzk->zeros[j]));
						if( mind > tmp_mind ) {
							mind = tmp_mind;
							k = j;
						}
					}
				}
				tmp = pzk->zeros[l];
				pzk->zeros[l] = pzk->zeros[k];
				pzk->zeros[k] = tmp;
				l += 2;
			}
		}
		
	} else if( firstRealPole < firstRealZero ) {
		for( ; i < 2*(section2-section1); i+=2 ) {
			mind = cabs(csub(pzk->poles[i], pzk->zeros[l])) + cabs(csub(pzk->poles[i+1], pzk->zeros[l]));
			k = l;
			for( j=l+1; j < pzk->nextZero; j++ ) {
				tmp_mind = cabs(csub(pzk->poles[i], pzk->zeros[j])) + cabs(csub(pzk->poles[i+1], pzk->zeros[j]));
				if( mind > tmp_mind ) {
					mind = tmp_mind;
					k = j;
				}
			}
			tmp = pzk->zeros[l];
			pzk->zeros[l] = pzk->zeros[k];
			pzk->zeros[k] = tmp;
			l++;
		}
	}
	
	for( ; i < pzk->nextPole; i++ ) {
		mind = fabs(pzk->poles[i].re - pzk->zeros[l].re);
		k = l;
		for( j=l+1; j < pzk->nextZero; j++ ) {
			tmp_mind = fabs(pzk->poles[i].re - pzk->zeros[l].re);
			if( mind > tmp_mind ) {
				mind = tmp_mind;
				k = j;
			}
		}
		tmp = pzk->zeros[l];
		pzk->zeros[l] = pzk->zeros[k];
		pzk->zeros[k] = tmp;
		l++;
	}
}

int preSortDigitalZeros(pzkContainer * pzk) {
	int gaps[] = { 132, 57, 23, 10, 4, 1 };
	int i, j, g;
	complex tmp;
	
	// Shell sort:
	for( g = 0; g < 6; g++ ) {
		// Do an insertion sort for each gap size.
		for( i = gaps[g]; i < pzk->nextZero; i++ ) {
			tmp = pzk->zeros[i];
			for( j = i; j >= gaps[g] && pzk->zeros[j - gaps[g]].im < tmp.im; j -= gaps[g]) {
				pzk->zeros[j] = pzk->zeros[j - gaps[g]];
			}
			pzk->zeros[j] = tmp;
		}
	}
	
	g = pzk->nextZero;
	for( i = 0; i < pzk->nextPole; i++ ) {
		if( pzk->poles[i].im == 0 ) {
			g = i;
			break;
		}
	}
	return g;
}
	
int sortDigitalPoles(pzkContainer * pzk) {
	int gaps[] = { 132, 57, 23, 10, 4, 1 };
	int i, j, g;
	complex tmp;
	
	// Shell sort:
	for( g = 0; g < 6; g++ ) {
		// Do an insertion sort for each gap size.
		for( i = gaps[g]; i < pzk->nextPole; i++ ) {
			tmp = pzk->poles[i];
			for( j = i; j >= gaps[g] && compare1(pzk->poles[j - gaps[g]], tmp); j -= gaps[g]) {
				pzk->poles[j] = pzk->poles[j - gaps[g]];
			}
			pzk->poles[j] = tmp;
		}
	}

	// Gnome sort:
	i = 1; j = 0;
	while( i < pzk->nextPole ) {
        if ( pzk->poles[i].im < pzk->poles[i-1].im || ( pzk->poles[i].im == pzk->poles[i-1].im && pzk->poles[i].re >= pzk->poles[i-1].re ) ) {	// descending order
            if (j != 0) {
                i = j;
                j = 0;
            }
            i++;
		}
        else {
            tmp = pzk->poles[i];
			pzk->poles[i] = pzk->poles[i-1];
			pzk->poles[i-1] = tmp;
            if (i > 1) {
                if (j == 0) {
                    j = i;
                }
                i--;
			}
            else {
                i++;
            }
        }
    }
	
	g = pzk->nextPole;
	for( i = 0; i < pzk->nextPole; i++ ) {
		if( pzk->poles[i].im == 0 ) {
			g = i;
			break;
		}
	}
	return g;
}
