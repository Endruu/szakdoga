#include "../headers/iir_functions.h"	// function prototypes
#include "../headers/filterinfo.h"		// filterinfo constructors
#include <stdio.h>

int parseIirDigitalizationParameters( char * s, int l, filterInfo * fi ) {
	float tmp;
	fi->warping = 0;
	
	switch( s[0] ) {
		case 'B' :
			fi->warping = WARP_NORMAL;
			break;
		case 'C' :
			fi->warping = WARP_AUTO;
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

int cmdGenerateIir( char * s, int l, int modify ) {
	int nextDelimiter = -1;
	int sectionStart;
	
	pzkContainer *pzk1, *pzk2;
	
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
		else {
			error(67);
		}
	}
	
	if( pzk1 = createReferentFilter( newFilter ) ) {
		if( pzk2 = transformFilter( newFilter, pzk1 ) ) {
			deletePzkContainer( pzk1 );
			if( pzk1 = digitalizeFilter( newFilter, pzk2 ) ) {
				deletePzkContainer( pzk2 );
				if( implementFilter( newFilter, pzk1 ) ) {
					deletePzkContainer( pzk1 );
				} else {
					deletePzkContainer( pzk1 );
					error(35);
				}
			} else {
				deletePzkContainer( pzk2 );
				error(33);
			}
		} else {
			deletePzkContainer( pzk1 );
			error(31);
		}
	} else {
		error(29);
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
