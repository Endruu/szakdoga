#include "../headers/iir_functions.h"	// function prototypes
#include "../headers/filterinfo.h"		// filterinfo constructors
#include "../headers/debug.h"		// diagnostic functions
#include <stdio.h>
#include <stdlib.h>


int createIirFilter( filterInfo * fi, unsigned char level ) {
	pzkContainer *pzk1 = NULL, *pzk2 = NULL;
	biquad * bList;	// biquad list
	uint insert;

	if( level >= P_REFERENT ) {
		if( pzk1 = createReferentFilter( fi ) ) {

#ifdef DEBUG_FUNCTIONS_ENABLED  //--------------------------------------------------------------------------++
			if( level & (P_REFERENT | P_PRINT) ) {
				out("\n-- Referent filter: ---------------------------\n\n");
				printPzkContainer( pzk1 );
			}
#endif //---------------------------------------------------------------------------------------------------//

			if( level >= P_TRANSFORMED ) {
				if( pzk2 = transformFilter( fi, pzk1 ) ) {
					pzk1 = deletePzkContainer( pzk1 );

					insert = sortPzkContainer( pzk2, SORT_BY_QFACTOR, ORDER_UP );

#ifdef DEBUG_FUNCTIONS_ENABLED  //--------------------------------------------------------------------------++
					if( level & (P_TRANSFORMED | P_PRINT) ) {
						out("\n-- Transformed filter: -----------------------\n\n");
						printPzkContainer( pzk2 );
					}
#endif //---------------------------------------------------------------------------------------------------//

					if( level >= P_BIQUAD ) {
						if ( bList = pairPZ( pzk2, insert, PAIR_ZEROS_TO_POLES ) ) {

#ifdef DEBUG_FUNCTIONS_ENABLED  //--------------------------------------------------------------------------++
							if( level & (P_TRANSFORMED | P_PRINT | P_BIQUAD) ) {
								out("\n-- Analog biquad list: -----------------------\n\n");
								printBiquadList( bList, pzk2 );
							}
#endif //---------------------------------------------------------------------------------------------------//

							if( level > P_DIGITALIZED ) {
								if( pzk1 = digitalizeFilter( fi, pzk2 ) ) {
									pzk2 = deletePzkContainer( pzk2 );

#ifdef DEBUG_FUNCTIONS_ENABLED  //--------------------------------------------------------------------------++
									if( level & (P_DIGITALIZED | P_PRINT) ) {
										out("\n-- Digital filter: ---------------------------\n\n");
										printPzkContainer( pzk1 );
										if( level & P_BIQUAD ) {
											out("\n-- Digital biquad list: ----------------------\n\n");
											printBiquadList( bList, pzk1 );
										}
#endif //---------------------------------------------------------------------------------------------------//

										if( !implementFilter(pzk1, bList) ) {
											free(bList);
											deletePzkContainer( pzk1 );
											error(98);
										}
									}

								} else {
									free(bList);
									deletePzkContainer( pzk2 );
									error(75);
								}
							}

						} else {
							deletePzkContainer( pzk2 );
							error(78);
						}
					}
				} else {
					deletePzkContainer( pzk1 );
					error(74);
				}
			}

		} else {
			error(73);
		}
	} else {
		error(72);
	}

	if( pzk1 != NULL ) deletePzkContainer(pzk1);
	if( pzk2 != NULL ) deletePzkContainer(pzk2);
	if( bList != NULL ) free(bList);

	return 1;
}

int parseIirDigitalizationParameters( char * s, int l, filterInfo * fi ) {
	float tmp;
	fi->warping = 0;
	
	switch( s[0] ) {
		case 'B' :
			fi->warping = WARP_FACTOR;
			break;
		case 'P' :
			fi->warping = WARP_FREQUENCY;
			break;
		case 'C' :
			fi->warping = WARP_AUTO_FIX;
			break;
		case 'F' :
			if ( sscanf( s + 1, "%g", &tmp ) == 1 ) {
				fi->warping = tmp;
			} else {
				error(45);
			}
			fi->warping *= 2.0 * PI;
			if( fi->warping < 0 ) {
				fi->warping *= -1.0;
				//warn
			}
			break;
		case 'W' :
			if ( sscanf( s + 1, "%g", &tmp ) == 1 ) {
				fi->warping = tmp;
			} else {
				error(46);
			}
			if( fi->warping < 0 ) {
				fi->warping *= -1.0;
				//warn
			}
			break;
		default:
			error(47);
	}

	return 1;
}

int parseIirReferentParameters( char * s, int l, filterInfo * fi ) {
	int nextLetter, prevLetter;
	iirParameters ip = defaultIirParameters();
	float tmp;

	if( l < 2 ) {
		error(48);
	}
	
	if			(s[0] == 'B' && s[1] == 'W') {
		fi->subtype = butterworth;
	} else if	(s[0] == 'C' && s[1] == '1') {
		fi->subtype = chebyshev1;
	} else if	(s[0] == 'C' && s[1] == '2') {
		fi->subtype = chebyshev2;
	} else {
		error(19);
	}
	
	if( l > 2 && s[2] != ':' ) {
		error(49);
	}
	
	nextLetter = 2;
	while( ++nextLetter < l ) {
		prevLetter = nextLetter++;
		for( ; nextLetter < l && s[nextLetter] != ','; nextLetter++ );
		
		switch( s[prevLetter] ) {
			case 'N' :
				if ( sscanf( s + prevLetter + 1, "%d", &(ip.n) ) != 1 ) {
					error(50);
				}
				break;
			case 'C' :
				if ( sscanf( s + prevLetter + 1, "%g", &tmp ) == 1 ) {
					ip.ac = tmp;
				} else {
					error(51);
				}
				break;
			case 'S' :
				if ( sscanf( s + prevLetter + 1, "%g", &tmp ) == 1 ) {
					ip.as = tmp;
				} else {
					error(52);
				}
				break;
			case 'W' :
				if ( sscanf( s + prevLetter + 1, "%g", &tmp ) == 1 ) {
					ip.ws = tmp;
				} else {
					error(53);
				}
				break;
			case 'L' :
				ip.inDb = 0; break;
			default:
				error(54);
		}
	}
	
	if( !normalizeIirParameters( &ip ) ) {
		error(22);
	}
	
	fi->iirP = ip;
	
	return 1;

}

int parseIirTransformParameters( char * s, int l, filterInfo * fi ) {
	int nextLetter, prevLetter;
	int type1 = 0, type2 = 0;
	transformParameters tp = defaultTransformParameters();
	float tmp = 0;
	if( l < 2 ) {
		error(55);
	}
	
	if			(s[0] == 'L' && s[1] == 'P') {
		fi->type = lowpass;
	} else if	(s[0] == 'H' && s[1] == 'P') {
		fi->type = highpass;
	} else if	(s[0] == 'B' && s[1] == 'P') {
		fi->type = bandpass;
	} else if	(s[0] == 'B' && s[1] == 'S') {
		fi->type = bandstop;
	} else {
		error(15);
	}
	
	if( l > 2 && s[2] != ':' ) {
		error(56);
	}

	nextLetter = 2;
	while( ++nextLetter < l ) {
		prevLetter = nextLetter++;
		for( ; nextLetter < l && s[nextLetter] != ','; nextLetter++ );
		
		if( fi->type == lowpass || fi->type == highpass ) {
			switch( s[prevLetter] ) {
				case 'C' :
					if ( sscanf( s + prevLetter + 1, "%g", &tmp ) == 1 ) {
						tp.w0 = tmp;
					} else {
						error(57);
					}
					break;
				case 'W' :
					tp.inHz = 0; break;
				default:
					error(20);
			}
		}
		else {
			switch( s[prevLetter] ) {
				case 'C' :
					if ( sscanf( s + prevLetter + 1, "%g", &tmp ) == 1 ) {
						tp.w0 = tmp;
					} else {
						error(58);
					}
					type1 = 1;
					break;
				case 'D' :
					if ( sscanf( s + prevLetter + 1, "%g", &tmp ) == 1 ) {
						tp.w1 = tmp;
					} else {
						error(59);
					}
					type1 = 1;
					break;
				case 'L' :
					if ( sscanf( s + prevLetter + 1, "%g", &tmp ) == 1 ) {
						tp.w0 = tmp;
					} else {
						error(60);
					}
					type2 = 1;
					break;
				case 'U' :
					if ( sscanf( s + prevLetter + 1, "%g", &tmp ) == 1 ) {
						tp.w1 = tmp;
					} else {
						error(61);
					}
					type2 = 1;
					break;
				case 'W' :
					tp.inHz = 0; break;
				default:
					error(62);
			}
		}
	}
	

	if( ( type1 || type2 ) && ( tp.w0 == 0 || tp.w1 == 0 ) ) {
		error(71);
	}

	if( type1 ) {
		if( type2 ) {
			error(63);
		} else {
			tp.isDw = 1;
		}
	}
	
	if( !normalizeTransformParameters( &tp ) ) {
		error(18);
	}
	
	fi->transformP = tp;
	
	return 1;
	
}

int parseIirImplementParameters( char * s, int l, filterInfo * fi ) {
	int nextLetter, prevLetter;
	implementParameters ip = defaultImplementParameters();
	char tmp;
	
	nextLetter = -1;
	while( ++nextLetter < l ) {
		prevLetter = nextLetter++;
		for( ; nextLetter < l && s[nextLetter] != ','; nextLetter++ );
		
		switch( s[prevLetter] ) {
			case 'S' :
				if ( sscanf( s + prevLetter + 1, "%c", &tmp ) == 1 ) {
					if( tmp == 'q' || tmp == 'Q' ) {
						ip.sort = SORT_BY_QFACTOR;
					} else if( tmp == 'f' || tmp == 'F' ) {
						ip.sort = SORT_BY_MAGNITUDE;
					} else {
						error(206);
					}
				} else {
					error(201);
				}
				break;
			case 'P' :
				if ( sscanf( s + prevLetter + 1, "%c", &tmp ) == 1 ) {
					if( tmp == 'p' || tmp == 'P' ) {
						ip.pair = PAIR_ZEROS_TO_POLES;
					} else if( tmp == 'z' || tmp == 'Z' ) {
						ip.pair = PAIR_POLES_TO_ZEROS;
					} else {
						error(207);
					}
				} else {
					error(202);
				}
				break;
			case 'O' :
				if ( sscanf( s + prevLetter + 1, "%c", &tmp ) == 1 ) {
					if( tmp == 'u' || tmp == 'U' ) {
						ip.order = ORDER_UP;
					} else if( tmp == 'd' || tmp == 'D' ) {
						ip.order = ORDER_DOWN;
					} else {
						error(208);
					}
				} else {
					error(203);
				}
				break;
			case 'F' :
				if ( sscanf( s + prevLetter + 1, "%d", &tmp ) == 1 ) {
					if( tmp < 0 ) {
						// warn
						tmp = -tmp;
					}
					if( tmp >= FILTER_NUM ) {
						error(209);
					}
					ip.filter = tmp;
				} else {
					error(204);
				}
				break;
			default:
				error(205);
		}
	}
	
	fi->implementP = ip;
	
	return 1;

}

int cmdGenerateIir( char * s, int l, int modify ) {
	int nextDelimiter = -1;
	int sectionStart;
	
	filterInfo * newFilter = &filterBank[tmpFilter];
	
	if( modify ) {
		filterBank[tmpFilter] = copyFilterInfo( &filterBank[actualFilter] );
	} else {
		filterBank[tmpFilter] = defaultIirFilterInfo();
	}
	
	while( ++nextDelimiter < l ) {
		sectionStart = nextDelimiter++;
		for( ; nextDelimiter < l && s[nextDelimiter] != CODE_DELIMITER; nextDelimiter++ );
		
		if		( s[sectionStart] == 'R' && s[sectionStart + 1] == 'F' ) {
			if( !parseIirReferentParameters( s + sectionStart + 3, nextDelimiter - sectionStart - 3, newFilter ) ) {
				error(64);
			}
		}
		else if	( s[sectionStart] == 'T' && s[sectionStart + 1] == 'P' ) {
			if( !parseIirTransformParameters( s + sectionStart + 3, nextDelimiter - sectionStart - 3, newFilter ) ) {
				error(65);
			}
		}
		else if	( s[sectionStart] == 'D' && s[sectionStart + 1] == 'P' ) {
			if( !parseIirDigitalizationParameters( s + sectionStart + 3, nextDelimiter - sectionStart - 3, newFilter ) ) {
				error(66);
			}
		}
		else if	( s[sectionStart] == 'I' && s[sectionStart + 1] == 'P' ) {
			if( !parseIirImplementParameters( s + sectionStart + 3, nextDelimiter - sectionStart - 3, newFilter ) ) {
				error(200);
			}
		}
		else {
			error(67);
		}
	}
	
	if( createIirFilter( newFilter, P_ALL | P_PRINT ) ) {
		
	} else {
		error(80);
	}
	
	// set the new filter in an atomic way
	//CLI();
	//disableAudio();
	
	nextDelimiter = actualFilter;	// nextDelimiter as temp var for swap
	actualFilter = tmpFilter;
	tmpFilter = nextDelimiter;
	
	for(nextDelimiter = 0; nextDelimiter < DELAY_SIZE; nextDelimiter++) {
			delayLine[nextDelimiter] = 0;
	}
	
	//enableAudio();
	//STI();

	return 1;
	
}
