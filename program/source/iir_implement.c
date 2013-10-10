#include "../headers/pzk_container.h"		// pzk functions
#include "../headers/diagnostics.h"			// for setting memory usage
#include "../headers/iir_filters.h"			// filtering functions
#include "../headers/iir_functions.h"		// function prototypes
#include "../headers/emath.h"				// math functions
#include "../headers/type_converters.h"		// as name says

/*
#ifndef _COMPILE_WITH_BLACKFIN
#define cabs cabs_custom
#endif
*/
#define MAX_FR16	0x7FFF
#define m1pln2		-1.442695041

real getInfNormScale(complex * z1, complex * z2, complex * p1, complex * p2) {
	real op1, op2, oz1, oz2, tmp, K = 1;
	
	if( p2 == NULL ) {
		op1 = op2 = cabs(cdiv(csub2(1,*p1),csub2(-1,*p1)));
		K = (1-2*p1->re+cabs2(*p1))/4;
	} else {
		op1 = ( p1->re - 1 )/( p1->re + 1 );
		op2 = ( p2->re - 1 )/( p2->re + 1 );
		if( op1 > op2 ) {
			tmp = op1;
			op1 = op2;
			op2 = tmp;
		}
		if( fabs(p1->re) != 1.0 ) {
			K *=( p1->re - 1 )/2;
		}
		if( fabs(p2->re) != 1.0 ) {
			K *=( p2->re - 1 )/2;
		}
	}
	
	if( z2 == NULL ) {
		oz1 = oz2 = cabs(cdiv(csub2(1,*z1),csub2(-1,*z1)));
		K /= 1-2*z1->re+cabs2(*z1)/4;
	} else {
		oz1 = ( z1->re - 1 )/( z1->re + 1 );
		oz2 = ( z2->re - 1 )/( z2->re + 1 );
		if( oz1 > oz2 ) {
			tmp = oz1;
			oz1 = oz2;
			oz2 = tmp;
		}
		if( fabs(z1->re) != 1.0 ) {
			K /=( z1->re - 1)/2;
		}
		if( fabs(z2->re) != 1.0 ) {
			K /=( z2->re - 1)/2;
		}
	}
	
	if( z2 == NULL ) {
		tmp = K*cabs2(csub2(-1, *z1));
	} else {
		tmp = K;
		if( z1->re != -1 ) tmp *= (1+z1->re);
		if( z2->re != -1 ) tmp *= (1+z2->re);
	}
	
	if( p2 == NULL ) {
		tmp /= cabs2(csub2(-1, *p1));
		
		if( tmp > 1.0 ) {
			
			return K/tmp;
		} else {
			
			return K;
		}
			
	} else {
		if( (op1<=oz1 && oz2<=op2) || (oz2<=op1) || (oz1<=op1 && op1<=oz2 && oz2<=op2) ) {
			
			if( p1->re != -1 ) tmp /= (1+p1->re);
			if( p2->re != -1 ) tmp /= (1+p2->re);
			
			if( tmp > 1.0 ) {
				return K/tmp;
			} else {
				return K;
			}
		} else if( oz1<=op1 && op2<=oz2 ) {
			//kell!
		}
	}
	
}
#include<stdlib.h>
#include<float.h>



biquad * pairPZ( pzkContainer * pzk, uint insert, char options ) {
	biquad * bList;
	complex cwz;
	int ip, iz, ib, b = 0;
	int wz_used = 0, remainingPoles;
	real dist, min;
	uint bmax = countBiquads(pzk);

	remainingPoles = pzk->nextPole;
	if( pzk->no_wz < 0 ) remainingPoles -= pzk->no_wz;

	if( countPoles(pzk) < countZeros(pzk) ) {
		errorR(75, NULL);		// there should not be less poles than zeros
	}

	bList = (biquad*)malloc( bmax * sizeof(biquad) );
	if( bList == NULL ) {
		errorR(76, NULL);
	}

	cwz.re = 0;
	cwz.im = pzk->wz;

	if( options | PAIR_ZEROS_TO_POLES ) {

		for( iz=0; (iz < pzk->nextZero) || (insert == 0 && (pzk->no_wz - wz_used) > 0); iz++ ) {

			if( b == bmax ) {
				free(bList);
				errorR(77, NULL);
			}

			bList[b].z1 = iz;				// default value: loop counter
			bList[b].z2 = EMPTY_PAIR;		// zero1 is empty by default
			bList[b].p1 = EMPTY_PAIR;		// pole1 is empty by default
			bList[b].p2 = EMPTY_PAIR;		// pole2 is empty by default
			min = DBL_MAX;					// min distance, deault: infinity

			if( iz == insert && pzk->no_wz > 0 ) {		// if wz insert point is reached and no_wz indicates zeros 
				if( wz_used < pzk->no_wz ) {			// and there are unused ones
					bList[b].z1 = WZ_PAIR;				// use wz as zero
					if( pzk->wz != 0 ) {				// if conjugate pair
						bList[b].z2 = WZ_PAIR;			// use wz for z2 too
					}
					wz_used++;							// count used wz
					iz--;								// keep index fixed
				}
			}

			if( bList[b].z2 == EMPTY_PAIR ) {						// if z2 is still empty
				if( bList[b].z1 != WZ_PAIR) {						// and z1 is not wz
					if( !cisreal(pzk->zeros[bList[b].z1]) ) {		// but it's complex
						bList[b].z2 = bList[b].z1;					// then z2 is complex too (its conjugate pair)
					}
				}
			}
		

			if( bList[b].z2 == EMPTY_PAIR ) {							// if z1 is real

				if( bList[b].z1 != WZ_PAIR ) {
					// check if pzk->zeros[iz] is already taken
					for( ib=0; ib < b; ib++ ) {								// loop through biquads
						if( bList[ib].z1 == iz || bList[ib].z2 == iz ) {	// already taken
							ib = -1;										// signal that zero already taken
							break;
						}
					}
					if( ib == -1 ) {										// if zero is already taken
						continue;											// continue with next zero
					}
				}

				// search for next zero that is not taken and is real
				for( ip = iz; ip < pzk->nextZero; ip++ ) {		// loop through remaining zeros
					if( ip == insert && pzk->no_wz > 0 ) {		// if wz insert point is reached and no_wz indicates zeros 
						if( wz_used < pzk->no_wz ) {			// and there are unused ones
							if( pzk->wz == 0 ) {				// and if real
								bList[b].z2 = WZ_PAIR;			// use wz for z2
								wz_used++;						// count used wz
								break;							// exit loop
							}
						}
					}

					// if z2 is still empty (wz was not paired) 
					if( !cisreal(pzk->zeros[ip]) ) {						// search for real zeros
						continue;											// ignore complex
					}
					for( ib=0; ib < b; ib++ ) {								// loop through previous biquads
						if( bList[ib].z1 == ip || bList[ib].z2 == ip ) {	// already taken
							ib = -1;										// signal that zero already taken
							break;
						}
					}
					if( ip == bList[ib].z1 ) continue;						// actual biquad
					if( ib != -1 ) {										// if zero is not taken
						bList[b].z2 = ip;									// use this zero
						break;												// exit loop
					}
				}

				// search for nearest pole
				for( ip=0; ip < pzk->nextPole; ip++ ) {						// loop through poles
					for( ib=0; ib < b; ib++ ) {								// loop through biquads
						if( bList[ib].p1 == ip || bList[ib].p2 == ip ) {	// already taken
							ib = -1;										// signal that pole already taken
							break;
						}
					}
					if( ib != -1 ) {												// if pole is not yet taken
						if( bList[b].z1 == WZ_PAIR ) {										// if z1 is wz
							dist = cabs( csub(cwz, pzk->poles[ip]) );						// then calculate distance from wz
						} else {
							dist = cabs( csub(pzk->zeros[bList[b].z1], pzk->poles[ip]) );	// else: calculate distance from z1
						}
						if( dist < min ) {									// if new distance is smaller than the actual minimal
							min = dist;										// new distance becomes the actual minimal
							bList[b].p1 = ip;								// use this pole
						
						} else if( dist == min && !cisreal(pzk->poles[ip]) ) {								// if new distance is equal to the actual minimal, and pole is real
							if( ( ip > 0 && bList[b].z2 != EMPTY_PAIR ) ) {									// if there was a previous pole and the second zero is not empty
								if( bList[b].z2 == WZ_PAIR ) {												// if z2 is wz
									if(	cabs( csub( cwz, pzk->poles[ip])  )									// if the distance between wz and the actual pole
										<																	// is less than
										cabs( csub( cwz, pzk->poles[bList[b].p1])  )						// the distance between wz and the pole (with minimal dist. to z1)
									) {
										bList[b].p1 = ip;													// then use this pole
									}
								} else {
									if(	cabs( csub(pzk->zeros[bList[b].z2], pzk->poles[ip])  )				// if the distance between z2 and the actual pole
										<																	// is less than
										cabs( csub(pzk->zeros[bList[b].z2], pzk->poles[bList[b].p1])  )		// the distance between z2 and the pole (with minimal dist. to z1)
									) {
										bList[b].p1 = ip;													// then use this pole
									}
								}
							}
						}
					}
				}	// ip loop

				if( bList[b].p1 != EMPTY_PAIR ) {				// if p1 is not empty
					if( !cisreal(pzk->poles[bList[b].p1]) ) {	// and is complex
						bList[b].p2 = bList[b].p1;				// p2 becomes the same
					}
				}

				// check if wz can be used and nearer than the previously found
				if( -pzk->no_wz > wz_used ) {										// if there are unused wz poles
					dist = cabs( csub(pzk->zeros[bList[b].z1], cwz) );				// distance between z1 and j*wz
					if( dist < min ) {												// if new distance is smaller than the actual minimal
						bList[b].p1 = WZ_PAIR;
						if( pzk->wz != 0 ) {										// if conjugate pair
							bList[b].p2 = WZ_PAIR;									// p2 becomes the same
						}
						wz_used++;													// count used wz poles
					}
					else if( dist == min && pzk->wz != 0 ) {								// if new distance is equal to the actual minimal, and wz means conjugate pair
						if( ( ip > 0 && bList[b].z2 != EMPTY_PAIR ) ) {								// if there was a previous pole and the second zero is not empty
							if(	cabs( csub(pzk->zeros[bList[b].z2], cwz)  )							// and if the distance between z2 and wz
								<																	// is less than
								cabs( csub(pzk->zeros[bList[b].z2], pzk->poles[bList[b].p1])  )		// the distance between z2 and the pole (with minimal dist. to z1)
							) {
								bList[b].p1 = bList[b].p1 = WZ_PAIR;								// then use this pole
								wz_used++;															// count used wz poles
							}
						}
					}
				}

				// if p1 is not empty but p2 is, then try to find another real pole
				min = DBL_MAX;
				if( bList[b].p1 != EMPTY_PAIR && bList[b].p2 == EMPTY_PAIR ) {
					for( ip=0; ip < pzk->nextPole; ip++ ) {						// loop through poles
						if( !cisreal(pzk->poles[ip]) ) {						// if pole is complex
							continue;											// then continue with next pole
						}
						for( ib=0; ib < b; ib++ ) {								// loop through biquads
							if( bList[ib].p1 == ip || bList[ib].p2 == ip ) {	// already taken
								ib = -1;										// signal that pole already taken
								break;
							}
						}
						if( bList[ib].p1 == ip ) continue;						// actual biquad (not included in for loop)
						if( ib != -1 ) {														// if pole is not yet taken
							if( bList[b].z2 != EMPTY_PAIR ) {									// if there is z2
								dist = cabs( csub(pzk->zeros[bList[b].z2], pzk->poles[ip]) );	// use that to calculate distance
							} else {
								dist = cabs( csub(pzk->zeros[bList[b].z1], pzk->poles[ip]) );	// else: use z1 to calculate distance
							}
							if( dist < min ) {									// if new distance is smaller than the actual minimal
								min = dist;										// new distance becomes the actual minimal
								bList[b].p2 = ip;								// use this pole
							}
						}
					}	// ip loop
				}
				if( -pzk->no_wz > wz_used ) {										// if there are unused wz poles
					if( pzk->wz == 0 ) {											// if wz is 0 (thus real)
						if( bList[b].z2 != EMPTY_PAIR ) {							// if there is z2
							dist = cabs( pzk->zeros[bList[b].z2] );					// use that to calculate distance
						} else {
							dist = cabs( pzk->zeros[bList[b].z1] );					// else: use z1 to calculate distance
						}
						if( dist < min ) {											// if new distance is smaller than the actual minimal
							bList[b].p2 = WZ_PAIR;
							wz_used++;												// count used wz poles
						}
					}
				}
			} // z1 is real
			else {

				// search for nearest pole
				for( ip=0; ip < pzk->nextPole; ip++ ) {						// loop through poles
					for( ib=0; ib < b; ib++ ) {								// loop through biquads
						if( bList[ib].p1 == ip || bList[ib].p2 == ip ) {	// already taken
							ib = -1;										// signal that pole already taken
							break;
						}
					}
					if( ib != -1 ) {												// if pole is not yet taken
						if( bList[b].z1 == WZ_PAIR ) {										// if z1 is wz
							dist = cabs( csub(cwz, pzk->poles[ip]) );						// then calculate distance from wz
						} else {
							dist = cabs( csub(pzk->zeros[bList[b].z1], pzk->poles[ip]) );	// else: calculate distance from z1
						}
						if( dist < min ) {									// if new distance is smaller than the actual minimal
							min = dist;										// new distance becomes the actual minimal
							bList[b].p1 = ip;								// use this pole
						
						}
					}
				}	// ip loop

				if( !cisreal(pzk->poles[bList[b].p1]) ) {	// if pole is complex
					bList[b].p2 = bList[b].p1;				// p2 becomes the same
				}

				// check if wz can be used and nearer than the previously found
				if( -pzk->no_wz > wz_used ) {									// if there are unused wz poles
					dist = cabs( csub(pzk->zeros[bList[b].z1], cwz) );			// distance between z1 and j*wz
					if( dist < min ) {											// if new distance is smaller than the actual minimal
						bList[b].p1 = WZ_PAIR;
						if( pzk->wz != 0 ) {									// if conjugate pair
							bList[b].p2 = WZ_PAIR;								// p2 becomes the same
						}
						wz_used++;												// count used wz poles
					}
				}

				// if p1 is not empty but p2 is, then try to find another real pole
				min = DBL_MAX;
				if( bList[b].p1 != EMPTY_PAIR && bList[b].p2 == EMPTY_PAIR ) {
					for( ip=0; ip < pzk->nextPole; ip++ ) {						// loop through poles
						if( !cisreal(pzk->poles[ip]) ) {						// if pole is complex
							continue;											// then continue with next pole
						}
						for( ib=0; ib < b; ib++ ) {								// loop through biquads
							if( bList[ib].p1 == ip || bList[ib].p2 == ip ) {	// already taken
								ib = -1;										// signal that pole already taken
								break;
							}
						}
						if( bList[ib].p1 == ip ) continue;						// actual biquad (not included in for loop)
						if( ib != -1 ) {													// if pole is not yet taken
							if( bList[b].z1 == WZ_PAIR ) {										// if z1 is wz
								dist = cabs( csub(cwz, pzk->poles[ip]) );						// then calculate distance from wz
							} else {
								dist = cabs( csub(pzk->zeros[bList[b].z1], pzk->poles[ip]) );	// else: calculate distance from z1
							}
							if( dist < min ) {												// if new distance is smaller than the actual minimal
								min = dist;													// new distance becomes the actual minimal
								bList[b].p2 = ip;											// use this pole
							}
						}
					}	// ip loop
				}
				if( -pzk->no_wz > wz_used ) {						// if there are unused wz poles
					if( pzk->wz == 0 ) {							// if wz is 0 (thus real)
						dist = cabs( pzk->zeros[bList[b].z1] );		// calculate distance
						if( dist < min ) {							// if new distance is smaller than the actual minimal
							bList[b].p2 = WZ_PAIR;
							wz_used++;								// count used wz poles
						}
					}
				}

			} // z1 is complex

			if( bList[b].p1 != EMPTY_PAIR ) {
				remainingPoles--;
				if( bList[b].p2 != EMPTY_PAIR && bList[b].p2 != bList[b].p1 ) {
					// if p2 is not empty and is different from p1 (meaning not complex)
					remainingPoles--;		// then two poles were used
				}
			}
			b++;	// next biquad

		} // iz loop
	}

	// pair poles
	// if there are reamaining poles they will be assigned here
	// if the zeros were not paired prior, then all poles remain
	for( ip=0; remainingPoles; ip++ ) {

		if( b == bmax ) {
			free(bList);
			errorR(78, NULL);
		}

		bList[b].p1 = ip;				// default value: loop counter
		bList[b].p2 = EMPTY_PAIR;		// pole2 is empty by default
		bList[b].z1 = EMPTY_PAIR;		// zero1 is empty by default
		bList[b].z2 = EMPTY_PAIR;		// zero2 is empty by default
		min = DBL_MAX;					// min distance, deault: infinity

		if( ip == insert && pzk->no_wz < 0 ) {		// if wz insert point is reached and no_wz indicates poles 
			if( wz_used < -pzk->no_wz ) {			// and there are unused ones
				bList[b].p1 = WZ_PAIR;				// use wz as pole
				if( pzk->wz != 0 ) {				// if conjugate pair
					bList[b].p2 = WZ_PAIR;			// use wz for p2 too
				}
				wz_used++;							// count used wz
				ip--;								// keep index fixed
			}
		}

		if( bList[b].p2 == EMPTY_PAIR ) {						// if p2 is still empty
			if( bList[b].p1 != WZ_PAIR) {						// and p1 is not wz
				if( !cisreal(pzk->poles[bList[b].p1]) ) {		// but it's complex
					bList[b].p2 = bList[b].p1;					// then p2 is complex too (its conjugate pair)
				}
			}
		}
		

		if( bList[b].p2 == EMPTY_PAIR ) {							// if p1 is real

			if( bList[b].p1 != WZ_PAIR ) {
				// check if pzk->poles[ip] is already taken
				for( ib=0; ib < b; ib++ ) {								// loop through biquads
					if( bList[ib].p1 == ip || bList[ib].p2 == ip ) {	// already taken
						ib = -1;										// signal that pole already taken
						break;
					}
				}
				if( ib == -1 ) {										// if pole is already taken
					continue;											// continue with next pole
				}
			}

			// search for next pole that is not taken and is real
			for( iz = ip; iz < pzk->nextPole; iz++ ) {		// loop through remaining poles
				if( iz == insert && pzk->no_wz < 0 ) {		// if wz insert point is reached and no_wz indicates poles 
					if( wz_used < -pzk->no_wz ) {			// and there are unused ones
						if( pzk->wz == 0 ) {				// if real
							bList[b].p2 = WZ_PAIR;			// use wz for p2
							wz_used++;						// count used wz
							break;							// exit loop
						}
					}
				}
				// if p2 is still empty (wz was not paired) 
				if( !cisreal(pzk->poles[iz]) ) {						// search for real poles
					continue;											// ignore complex
				}
				for( ib=0; ib < b; ib++ ) {								// loop through previous biquads
					if( bList[ib].p1 == iz || bList[ib].p2 == iz ) {	// already taken
						ib = -1;										// signal that pole already taken
						break;
					}
				}
				if( bList[ib].p1 == iz ) continue;						// actual biquad
				if( ib != -1 ) {										// if pole is not taken
					bList[b].p2 = iz;									// use this pole
					break;												// exit loop
				}
			}

			remainingPoles--;
			if( bList[b].p2 != EMPTY_PAIR && bList[b].p2 != bList[b].p1 ) {
				// if p2 is not empty and is different from p1 (meaning not complex)
				remainingPoles--;		// then two poles were used
			}

			if( options | PAIR_ZEROS_TO_POLES ) {			// if zeros were exhausted
				b++;										// increase biquad counter
				continue;									// skip rest of the loop (check next ip)
			}

			// search for nearest zero
			for( iz=0; iz < pzk->nextZero; iz++ ) {						// loop through zeros
				for( ib=0; ib < b; ib++ ) {								// loop through biquads
					if( bList[ib].z1 == iz || bList[ib].z2 == iz ) {	// already taken
						ib = -1;										// signal that zero already taken
						break;
					}
				}
				if( ib != -1 ) {										// if zero is not yet taken
					if( bList[b].p1 == WZ_PAIR ) {										// if p1 is wz
						dist = cabs( csub(cwz, pzk->zeros[iz]) );						// then calculate distance from wz
					} else {
						dist = cabs( csub(pzk->poles[bList[b].p1], pzk->zeros[iz]) );	// else: calculate distance from p1
					}
					if( dist < min ) {									// if new distance is smaller than the actual minimal
						min = dist;										// new distance becomes the actual minimal
						bList[b].z1 = iz;								// use this zero
						
					} else if( dist == min && !cisreal(pzk->zeros[iz]) ) {								// if new distance is equal to the actual minimal, and zero is real
						if( ( iz > 0 && bList[b].p2 != EMPTY_PAIR ) ) {									// if there was a previous zero and the second pole is not empty
							if( bList[b].p2 == WZ_PAIR ) {												// if p2 is wz
								if(	cabs( csub( cwz, pzk->zeros[iz])  )									// if the distance between wz and the actual zero
									<																	// is less than
									cabs( csub( cwz, pzk->zeros[bList[b].z1])  )						// the distance between wz and the zero (with minimal dist. to p1)
								) {
									bList[b].z1 = iz;													// then use this zero
								}
							} else {
								if(	cabs( csub(pzk->poles[bList[b].p2], pzk->zeros[iz])  )				// if the distance between p2 and the actual zero
									<																	// is less than
									cabs( csub(pzk->poles[bList[b].p2], pzk->zeros[bList[b].z1])  )		// the distance between p2 and the zero (with minimal dist. to p1)
								) {
									bList[b].z1 = iz;													// then use this zero
								}
							}
						}
					}
				}
			}	// iz loop

			if( bList[b].z1 != EMPTY_PAIR ) {					// if z1 is not empty
				if( !cisreal(pzk->zeros[bList[b].z1]) ) {		// and is complex
					bList[b].z2 = bList[b].z1;					// z2 becomes the same
				}
			}

			// check if wz can be used and nearer than the previously found
			if( pzk->no_wz > wz_used ) {										// if there are unused wz zeros
				dist = cabs( csub(pzk->poles[bList[b].p1], cwz) );				// distance between p1 and j*wz
				if( dist < min ) {												// if new distance is smaller than the actual minimal
					bList[b].z1 = WZ_PAIR;
					if( pzk->wz != 0 ) {										// if conjugate pair
						bList[b].z2 = WZ_PAIR;									// z2 becomes the same
					}
					wz_used++;													// count used wz zeros
				}
				else if( dist == min && pzk->wz != 0 ) {								// if new distance is equal to the actual minimal, and wz means conjugate pair
					if( ( iz > 0 && bList[b].p2 != EMPTY_PAIR ) ) {								// if there was a previous zero and the second pole is not empty
						if(	cabs( csub(pzk->poles[bList[b].p2], cwz)  )							// and if the distance between p2 and wz
							<																	// is less than
							cabs( csub(pzk->poles[bList[b].p2], pzk->zeros[bList[b].z1])  )		// the distance between p2 and the zero (with minimal dist. to p1)
						) {
							bList[b].z1 = bList[b].z1 = WZ_PAIR;								// then use this zero
							wz_used++;															// count used wz zeros
						}
					}
				}
			}

			// if z1 is not empty but z2 is, then try to find another real zero
			min = DBL_MAX;
			if( bList[b].z1 != EMPTY_PAIR && bList[b].z2 == EMPTY_PAIR ) {
				for( iz=0; iz < pzk->nextZero; iz++ ) {						// loop through zeros
					if( !cisreal(pzk->zeros[iz]) ) {						// if zero is complex
						continue;											// then continue with next zero
					}
					for( ib=0; ib < b; ib++ ) {								// loop through biquads
						if( bList[ib].z1 == iz || bList[ib].z2 == iz ) {	// already taken
							ib = -1;										// signal that zero already taken
							break;
						}
					}
					if( bList[ib].z1 == iz ) continue;						// actual biquad (not included in for loop)
					if( ib != -1 ) {														// if zero is not yet taken
						if( bList[b].p2 != EMPTY_PAIR ) {									// if there is p2
							dist = cabs( csub(pzk->poles[bList[b].p2], pzk->zeros[iz]) );	// use that to calculate distance
						} else {
							dist = cabs( csub(pzk->poles[bList[b].p1], pzk->zeros[iz]) );	// else: use p1 to calculate distance
						}
						if( dist < min ) {									// if new distance is smaller than the actual minimal
							min = dist;										// new distance becomes the actual minimal
							bList[b].z2 = iz;								// use this zero
						}
					}
				}	// iz loop
			}
			if( pzk->no_wz > wz_used ) {										// if there are unused wz zeros
				if( pzk->wz == 0 ) {											// if wz is 0 (thus real)
					if( bList[b].p2 != EMPTY_PAIR ) {							// if there is p2
						dist = cabs( pzk->poles[bList[b].p2] );					// use that to calculate distance
					} else {
						dist = cabs( pzk->poles[bList[b].p1] );					// else: use p1 to calculate distance
					}
					if( dist < min ) {											// if new distance is smaller than the actual minimal
						bList[b].z2 = WZ_PAIR;
						wz_used++;												// count used wz zeros
					}
				}
			}
		} // p1 is real
		else {

			if( options | PAIR_ZEROS_TO_POLES ) {			// if zeros were exhausted
				b++;										// increase biquad counter
				continue;									// skip rest of the loop (check next ip)
			}

			// search for nearest zero
			for( iz=0; iz < pzk->nextZero; iz++ ) {						// loop through zeros
				for( ib=0; ib < b; ib++ ) {								// loop through biquads
					if( bList[ib].z1 == iz || bList[ib].z2 == iz ) {	// already taken
						ib = -1;										// signal that zero already taken
						break;
					}
				}
				if( ib != -1 ) {										// if zero is not yet taken
					if( bList[b].p1 == WZ_PAIR ) {										// if p1 is wz
						dist = cabs( csub(cwz, pzk->zeros[iz]) );						// then calculate distance from wz
					} else {
						dist = cabs( csub(pzk->poles[bList[b].p1], pzk->zeros[iz]) );	// else: calculate distance from p1
					}
					if( dist < min ) {									// if new distance is smaller than the actual minimal
						min = dist;										// new distance becomes the actual minimal
						bList[b].z1 = iz;								// use this zero
						
					}
				}
			}	// iz loop

			if( !cisreal(pzk->zeros[bList[b].z1]) ) {	// if zero is complex
				bList[b].z2 = bList[b].z1;				// z2 becomes the same
			}

			// check if wz can be used and nearer than the previously found
			if( pzk->no_wz > wz_used ) {									// if there are unused wz zeros
				dist = cabs( csub(pzk->poles[bList[b].p1], cwz) );			// distance between p1 and j*wz
				if( dist < min ) {											// if new distance is smaller than the actual minimal
					bList[b].z1 = WZ_PAIR;
					if( pzk->wz != 0 ) {									// if conjugate pair
						bList[b].z2 = WZ_PAIR;								// z2 becomes the same
					}
					wz_used++;												// count used wz zeros
				}
			}

			// if z1 is not empty but z2 is, then try to find another real zero
			min = DBL_MAX;
			if( bList[b].z1 != EMPTY_PAIR && bList[b].z2 == EMPTY_PAIR ) {
				for( iz=0; iz < pzk->nextZero; iz++ ) {						// loop through zeros
					if( !cisreal(pzk->zeros[iz]) ) {						// if zero is complex
						continue;											// then continue with next zero
					}
					for( ib=0; ib < b; ib++ ) {								// loop through biquads
						if( bList[ib].z1 == iz || bList[ib].z2 == iz ) {	// already taken
							ib = -1;										// signal that zero already taken
							break;
						}
					}
					if( bList[ib].z1 == iz ) continue;						// actual biquad (not included in for loop)
					if( ib != -1 ) {													// if zero is not yet taken
						if( bList[b].p1 == WZ_PAIR ) {										// if p1 is wz
							dist = cabs( csub(cwz, pzk->zeros[iz]) );						// then calculate distance from wz
						} else {
							dist = cabs( csub(pzk->poles[bList[b].p1], pzk->zeros[iz]) );	// else: calculate distance from p1
						}
						if( dist < min ) {												// if new distance is smaller than the actual minimal
							min = dist;													// new distance becomes the actual minimal
							bList[b].z2 = iz;											// use this zero
						}
					}
				}	// iz loop
			}
			if( pzk->no_wz > wz_used ) {						// if there are unused wz zeros
				if( pzk->wz == 0 ) {							// if wz is 0 (thus real)
					dist = cabs( pzk->poles[bList[b].p1] );		// calculate distance
					if( dist < min ) {							// if new distance is smaller than the actual minimal
						bList[b].z2 = WZ_PAIR;
						wz_used++;								// count used wz zeros
					}
				}
			}

		} // p1 is complex

		b++;	// next biquad

	} // ip loop

	return bList;

}

real direct1_biquad( fract16 coeffs[], complex * z1, complex * z2, complex * p1, complex * p2, int scale_mod ) {
	real K = 1, tmp1, tmp2, tmp3, tmp4;
	int sign = 1;
	
	if( !cisreal(*p1) ) {
		if( p2 != p1 && p2 != NULL ) {
			error(1);									// if p1 is complex then p2 is assumed to be its conjugate
		}
		coeffs[4] = float_to_fr16( (float)( -cabs2(*p1) ) );
		coeffs[5] = float_to_fr16( (float)( -p1->re ) );
		
		K *= (1-2*p1->re+cabs2(*p1))/4;
		
	} else {
		tmp1 = -p1->re * p2->re;
		tmp2 = -(p1->re + p2->re)/2;
		
		if( fabs(p1->re) != 1.0 ) {
			K *=( p1->re - 1 )/2;
		}
		if( fabs(p2->re) != 1.0 ) {
			K *=( p2->re - 1 )/2;
		}
		
		if( tmp1 == 1 ) {								// if -p1*p2 = 1	: fract16 can only store [-1,1)
			warn(1);
			if( 1 )	{									// abs p1 and p2 both decreased minimally
				coeffs[4] = MAX_FR16;
				coeffs[5] = 0;
			} else {
				coeffs[4] = MAX_FR16 - 1;
				if( 1 ) {								// pole in 0 is decreased, pole in infinity is maintained
					coeffs[5] = 1;
				} else {								// pole in infinity is decreased, pole in 0 is maintained
					coeffs[5] = -1;
				}
			}
		} else {
			if( tmp2 == 1 ) {							// if p1 = p2 = -1	: fract16 can only store [-1,1)
				warn(2);
				coeffs[5] = MAX_FR16;
				if( 1 )	{
					coeffs[4] = MAX_FR16 - 1;			// p1 decreased, p2 maintained
				}										// else: abs(p1)=abs(p2) p1 = conj(p2)
			} else {
				coeffs[4] = float_to_fr16( (float)tmp1 );
				coeffs[5] = float_to_fr16( (float)tmp2 );
			}
		}
	}
	
	if( !cisreal(*z1) ) {
		if( z2 != z1 && z2 != NULL ) {
			error(2);									// if z1 is complex then z2 is assumed to be its conjugate
		}

		K /= 1-2*z1->re+cabs2(*z1)/4;

		if( K < 0 ) {
			K = -K;
			sign = -1;
		}

		tmp3 = -K*cabs2(*z1);
		tmp2 = K*z1->re;
	} else {
		if( fabs(z1->re) != 1.0 ) {
			K /=( z1->re - 1)/2;
		}
		if( fabs(z2->re) != 1.0 ) {
			K /=( z2->re - 1)/2;
		}

		if( K < 0 ) {
			K = -K;
			sign = -1;
		}
		
		tmp3 = -K * z1->re * z2->re;
		tmp2 = K *( z1->re + z2->re )/2;
	}
	
	tmp1 = -K;
	if( tmp2 < tmp1 || tmp1 == 0 ) tmp1 = tmp2;
	if( tmp3 < tmp1 || tmp1 == 0 ) tmp1 = tmp3;				// tmp1 = min
	tmp4 = tmp3;
	if( tmp2 > tmp4 || tmp4 == 0 ) tmp4 = tmp2;
	if(  -K  > tmp4 || tmp4 == 0 ) tmp4 =  -K ;				// tmp4 = max
	
	tmp1 = floor( log(fabs(tmp1)) * m1pln2 );
	tmp4 = ceil( log(fabs(tmp4)) * m1pln2 );
	if( tmp4 > tmp1 ) tmp4 = tmp1;							// min(tmp1, tmp4)
	tmp1 = pow(2, tmp4);
	tmp3 *= tmp1;
	tmp2 *= tmp1;
	tmp1 *= -K;
	
	coeffs[2] = float_to_fr16( (float)tmp1 );
	coeffs[1] = float_to_fr16( (float)tmp2 );
	coeffs[0] = float_to_fr16( (float)tmp3 );
	coeffs[3] = (int)(-tmp4);
	
	if( sign == 1 ) {
		return K;
	} else {
		return -K;
	}
}

real implementFilter( filterInfo * fi, pzkContainer * digitalized ) {
	int i = 0, j = 0, k = 1;
	complex *p1, *p2, *z1, *z2;
	complex zero;
	real tmp;
	real K = digitalized->amp;
	
	if(countZeros(digitalized) != countPoles(digitalized)) {
		error(3);
	}

	zero.re = 0;
	zero.im = 0;
	
	coeffLines[tmpFilter][0] = countBiquads(digitalized);
	
	sortDigitalPZ(digitalized);
	while( i < digitalized->nextPole ) {
		if( k+6 > COEFF_SIZE ) {
			error(4);
		}
		p1 = &digitalized->poles[i];
		if( cisreal( *p1 ) ) {
			if( i+1 < digitalized->nextPole ) {
				p2 = &digitalized->poles[i+1];
			} else {
				p2 = &zero;
			}
			i += 2;
		} else {
			p2 = NULL;
			i++;
		}
		z1 = &digitalized->zeros[j];
		if( cisreal( *z1 ) ) {
			if( j+1 < digitalized->nextZero ) {
				z2 = &digitalized->zeros[j+1];
			} else {
				z2 = &zero;
			}
			j += 2;
		} else {
			z2 = NULL;
			j++;
		}
		tmp = direct1_biquad(coeffLines[tmpFilter]+k, z1, z2, p1, p2, 0);
		if(tmp) {
			K /= tmp;
		} else {
			error(5);
		}
		k += 6;
	}
	
	if( k+1 > COEFF_SIZE ) {
		error(6);
	}
	
	coeffLines[tmpFilter][k] = float_to_fr16( (float)K );
	
	fi->filter = &direct1;
	
	//printf("fK = %g\n", K);
	return K;
}
