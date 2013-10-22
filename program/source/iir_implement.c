#include "../headers/pzk_container.h"		// pzk functions
#include "../headers/diagnostics.h"			// for setting memory usage
#include "../headers/iir_filters.h"			// filtering functions
#include "../headers/iir_functions.h"		// function prototypes
#include "../headers/emath.h"				// math functions
#include "../headers/type_converters.h"		// as name says
#include <stdlib.h>							// memory allocation
#include <float.h>							// float limits

const filterList fList[FILTER_NUM] = {
	{ &passThrough, &i_no_implement },
	{ &f_direct1_float_postK, &i_direct1_float_K }
};

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
			errorR(77, NULL);
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
				remainingPoles--;							// one pole used
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

int i_no_implement( pzkContainer * pzk, biquad * bList ) {
	return 1;
}

int i_direct1_float_K( pzkContainer * pzk, biquad * bList ) {
	float *c;
	const int bNum = countBiquads(pzk);
	int ib, ic = 0;
	float tmp;

	c = (float*)coeffLines[tmpFilter];

	filterBank[tmpFilter].mem_coeff = (2+bNum*4)*sizeof(float);
	if( filterBank[tmpFilter].mem_coeff > COEFF_SIZE * sizeof(COEFF_TYPE) ) {
		error(81);
	}
	filterBank[tmpFilter].mem_delay = (4+bNum*2)*sizeof(float);
	if( filterBank[tmpFilter].mem_delay > DELAY_SIZE * sizeof(DELAY_TYPE) ) {
		error(82);
	}

	c[0] = (float)bNum;
	c[1] = pzk->amp;
	c += 2;

	for( ib = 0; ib < bNum; ib++ ) {
		if( bList[ib].z1 == bList[ib].z2 ) {		// complex or both digital zero
			if( bList[ib].z1 == EMPTY_PAIR ) {
				if( bList[ib].p2 == EMPTY_PAIR ) {	// p2 and z2 kill each other
					c[ic++] = 0;		// b2
					c[ic++] = 1.0;		// b1
				} else {
					c[ic++] = 1.0;		// b2
					c[ic++] = 2.0;		// b1
				}

			} else if( bList[ib].z1 == WZ_PAIR ) {
				if( pzk->wz == DIGITAL_ZERO ) {
					c[ic++] = 1.0;		// b2
					c[ic++] = -2.0;		// b1
				} else {
					c[ic++] = 1.0;					// b2
					c[ic++] = -2.0*cos(pzk->wz);	// b1 = -2*Re{z}
				}
			} else {
				c[ic++] = cabs2(pzk->zeros[bList[ib].z1]);		// b2 = |z|^2
				c[ic++] = -2.0*pzk->zeros[bList[ib].z1].re;		// b1 = -2*Re{z}
			}
		}
		else {
			if( bList[ib].z1 == WZ_PAIR ) {				// also means digital zero
				if( bList[ib].z2 == EMPTY_PAIR ) {
					if( bList[ib].p2 == EMPTY_PAIR ) {	// p2 and z2 kill each other
						c[ic++] = 0;		// b2
						c[ic++] = -1.0;		// b1
					} else {
						c[ic++] = -1.0;		// b2
						c[ic++] = 0.0;		// b1
					}
				} else {
					tmp = pzk->zeros[bList[ib].z2].re;
					c[ic++] = tmp;		// b2 = |z| = Re{z}
					c[ic++] = -1.0-tmp;	// b1 = -(1+Re{z})
				}
			} else {
				if( bList[ib].z2 == EMPTY_PAIR ) {
					tmp = -pzk->zeros[bList[ib].z1].re;
					if( bList[ib].p2 == EMPTY_PAIR ) {	// p2 and z2 kill each other
						c[ic++] = 0;		// b2
						c[ic++] = tmp;		// b1
					} else {
						c[ic++] = tmp;		// b2 = -|z| = -Re{z}
						c[ic++] = 1.0+tmp;	// b1 = 1-Re{z}
					}
				} else if( bList[ib].z2 == WZ_PAIR ) {
					tmp = pzk->zeros[bList[ib].z1].re;
					c[ic++] = tmp;		// b2 = |z| = Re{z}
					c[ic++] = -1.0-tmp;	// b1 = -(1+Re{z})
				} else {
					tmp = pzk->zeros[bList[ib].z1].re;
					c[ic++] = pzk->zeros[bList[ib].z1].re * pzk->zeros[bList[ib].z2].re;		// b2 = |z1|*|z2| = Re{z1}*Re{z2}
					c[ic++] = -pzk->zeros[bList[ib].z1].re - pzk->zeros[bList[ib].z2].re;		// b1 = -|z1|-|z2| = -Re{z1}-Re{z2}
				}
			}
		}


		if( bList[ib].p1 == bList[ib].p2 ) {		// complex or both digital one or empty
			if( bList[ib].p1 == EMPTY_PAIR ) {
				if( bList[ib].z2 == EMPTY_PAIR ) {
					c[ic++] = 0;		// a2
					c[ic++] = 1.0;		// a1
				} else {
					c[ic++] = 1.0;		// a2
					c[ic++] = 2.0;		// a1
				}
			} else if( bList[ib].p1 == WZ_PAIR ) {
				if( pzk->wz == DIGITAL_ZERO ) {
					c[ic++] = 1.0;		// a2
					c[ic++] = -2.0;		// a1
				} else {
					c[ic++] = 1.0;					// a2
					c[ic++] = -2.0*cos(pzk->wz);	// a1 = -2*Re{p}
				}
			} else {
				c[ic++] = cabs2(pzk->poles[bList[ib].p1]);		// a2 = |p|^2
				c[ic++] = -2.0*pzk->poles[bList[ib].p1].re;		// a1 = -2*Re{p}
			}
		}
		else {
			if( bList[ib].p1 == WZ_PAIR ) {		// also means digital zero
				if( bList[ib].p2 == EMPTY_PAIR ) {
					if( bList[ib].z2 == EMPTY_PAIR ) {	// p2 and z2 kill each other
						c[ic++] = 0;		// a2
						c[ic++] = -1;		// a1
					} else {
						c[ic++] = -1.0;		// a2
						c[ic++] = 0.0;		// a1
					}
				} else {
					tmp = pzk->poles[bList[ib].p2].re;
					c[ic++] = tmp;		// a2 = |p| = Re{p}
					c[ic++] = -1.0-tmp;	// a1 = -(1+Re{p})
				}
			} else {
				if( bList[ib].p2 == EMPTY_PAIR ) {
					tmp = -pzk->poles[bList[ib].p1].re;
					if( bList[ib].p2 == EMPTY_PAIR ) {	// p2 and z2 kill each other
						c[ic++] = 0;		// a2
						c[ic++] = tmp;		// a1
					} else {
						c[ic++] = tmp;		// a2 = -|p| = -Re{p}
						c[ic++] = 1.0+tmp;	// a1 = 1-Re{p}
					}
				} else if( bList[ib].p2 == WZ_PAIR ) {
					tmp = pzk->poles[bList[ib].p1].re;
					c[ic++] = tmp;		// a2 = |z| = Re{p}
					c[ic++] = -1.0-tmp;	// a1 = -(1+Re{p})
				} else {
					tmp = pzk->poles[bList[ib].p1].re;
					c[ic++] = pzk->poles[bList[ib].p1].re * pzk->poles[bList[ib].p2].re;		// a2 = |p1|*|p2| = Re{p1}*Re{p2}
					c[ic++] = -pzk->poles[bList[ib].p1].re - pzk->poles[bList[ib].p2].re;		// a1 = -|p1|-|p2| = -Re{p1}-Re{p2}
				}
			}
		}
	}

	return 1;
}

int implementFilter( pzkContainer * digitalized, biquad * bList ) {
	filterInfo * fi = &filterBank[tmpFilter];
	char fnum = fi->implementP.filter;

	if( fList[fnum].implementer( digitalized, bList ) ) {
		fi->filter = *fList[fnum].filter;
		return 1;
	} else {
		error(83);
	}
}

void swapToNewFilter() {
	int i;

	// disable actual filter
	filterBank[actualFilter].filter = &passThrough;

	// clear delay line
	for( i = 0; i < DELAY_SIZE; i++ ) {
		delayLine[i] = 0;
	}

	// swap old and new filters
	i = actualFilter;
	actualFilter = tmpFilter;
	tmpFilter = i;

}