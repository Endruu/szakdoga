#include "global.h"
#include <stdio.h>

filterInfo newFilterInfo() {
	filterInfo r;
	r.iirP			= newIirParameters();
	r.transformP	= newTransformParameters();
	r.iFilter		= NULL;
	r.tFilter		= NULL;
	r.dFilter		= NULL;
	r.filter		= NULL;
	r.type			= empty;
	r.subtype		= empty;
	r.supertype		= empty;
	r.fState		= sStart;
	return r;
}

filterInfo copyFilterInfo( filterInfo * fi ) {
	filterInfo r;
	r.iirP			= fi->iirP;
	r.transformP	= fi->transformP;
	r.iFilter		= NULL;
	r.tFilter		= NULL;
	r.dFilter		= NULL;
	r.filter		= NULL;
	r.type			= fi->type;
	r.subtype		= fi->subtype;
	r.supertype		= fi->supertype;
	r.fState		= fi->fState;
	return r;
}

void deleteFilterInfo( filterInfo * fi ) {
	if(fi->iFilter) deletePzkContainer(fi->iFilter);
	if(fi->tFilter) deletePzkContainer(fi->tFilter);
	if(fi->dFilter) deletePzkContainer(fi->dFilter);
	fi->fState = sStart;
}

void printFilterInfo( filterInfo * fi ) {
	printPzkContainer(fi->iFilter);
	printPzkContainer(fi->tFilter);
	printPzkContainer(fi->dFilter);
	print4Matlab(fi->iFilter);
	print4Matlab(fi->tFilter);
	printf("Hc = zpk(z, p, k)\n\n");
	print4Matlab(fi->dFilter);
}

int changeState(char code[]) {
	int	tmp,
		chars_read = 0;
	
	filterInfo newFilter = copyFilterInfo( &filterR );
	
	while( code[chars_read] != '\0' ) {
		switch( code[chars_read] ) {
			case 'G':
				if( code[chars_read+1] == 'I' ) {
					if( tmp = decodeIirInput( code+chars_read+3, &newFilter ) ) {
						chars_read += tmp + 3;
						newFilter.fState = sReferent;
						newFilter.supertype = iir;
					} else {
						error(0);
					}
				} else if( code[chars_read+1] == 'F' ) {
					// FIR
				} else {
					error(0);
				}
				break;
				
			case 'T':
				if( tmp = decodeTransformInput( code+chars_read+2, &newFilter ) ) {
					chars_read += tmp + 2;
					if( newFilter.fState > sTransform ) {
						newFilter.fState = sTransform;
					}
				} else {
					error(0);
				}
				break;
				
			case 'D':
				if( tmp = decodeDigitalizationInput( code+chars_read+2, &newFilter ) ) {
					chars_read += tmp + 2;
					if( newFilter.fState > sDigitalize ) {
						newFilter.fState = sDigitalize;
					}
				} else {
					error(0);
				}
				break;
				
			default:
				error(0);
		}
	}
	
	if( newFilter.supertype == iir ) {
		switch( newFilter.fState ) {
			case sReferent:
				if( !createReferentFilter( &newFilter ) ) {
					error(0);
				}
			case sTransform:
				if( newFilter.iFilter == NULL ) {	
					error(0);
				}
				if( !transformFilter( &newFilter ) ) {
					deleteFilterInfo( &newFilter );
					error(0);
				}
			case sDigitalize:
				if( newFilter.tFilter == NULL ) {
					deleteFilterInfo( &newFilter );
					error(0); 
				}
				if( !digitalizeFilter( &newFilter ) ) {
					deleteFilterInfo( &newFilter );
					error(0);
				}
			case sImplement:
				if( newFilter.dFilter == NULL ) {
					deleteFilterInfo( &newFilter );
					error(0); 
				}
				/*if( !implementFilter( &newFilter ) ) {
					deleteFilterInfo( &newFilter );
					error(0); 
				}*/
			break;
		}
		
		switch( newFilter.fState ) {
			case sTransform:
				newFilter.iFilter = filterR.iFilter;
				filterR.iFilter = NULL;
			case sDigitalize:
				newFilter.tFilter = filterR.tFilter;
				filterR.tFilter = NULL;
			case sImplement:
				newFilter.dFilter = filterR.dFilter;
				filterR.dFilter = NULL;
			break;
		}
		deleteFilterInfo( &filterR );
		filterR = newFilter;
		return chars_read;
		
	} else if( newFilter.supertype == fir ) {
	} else {
		error(0);
	}
	
	
	return chars_read;
}

int decodeDigitalizationInput(char code[], filterInfo * fi) {
	char tmp_char;
	float tmp_float = -1;
	int chars_scanned = 0, chars_read = 0;

	if( sscanf(code, "%c", &tmp_char) ) {
		switch(tmp_char) {
			case 'b' :
			case 'B' :
				if( tmp_float ) { warn(0); }
				tmp_float = WARP_NORMAL;
				chars_read++;
				break;
			case 'm' :
			case 'M' :
				chars_read++;
				chars_scanned = 1;
				if( sscanf(code+chars_read, "%g%n", &tmp_float, &chars_scanned) ) {
					if( tmp_float < 0 ) {
						warn(0);
						tmp_float *= -1;
					}
					chars_read += chars_scanned;
					chars_scanned = 1;
					if( sscanf(code+chars_read, "%c", &tmp_char) ) {
						if( tmp_char == 'w' || tmp_char == 'W' ) {
							chars_read ++;
							tmp_float /= 2.0*PI;
						}
					}
				} else {
					tmp_float = WARP_AUTO;
				}
				break;
			default:
				error(0);
		}
		if( code[chars_read] == CODE_DELIMITER ) {
			fi->warping = tmp_float*2.0*PI;
			return chars_read+1;
		} else {
			error(0);
		}
	} else {
		error(0);
	}
}

int decodeTransformInput(char code[], filterInfo * fi) {
	char tmp_char;
	float tmp_float;
	int i, chars_scanned = 0, chars_read;
	int for_end;
	filterType filter;
	transformParameters tp = newTransformParameters();
	
	for_end = 3;
	if(code[0] == 'L' && code[1] == 'P') {
		filter = lowpass;
	} else if(code[0] == 'H' && code[1] == 'P') {
		filter = highpass;
	} else if(code[0] == 'B' && code[1] == 'P') {
		filter = bandpass;
		for_end++;
	} else if(code[0] == 'B' && code[1] == 'S') {
		filter = bandstop;
		for_end++;
	} else {
		error(0);
	}
	
	chars_read = 3;
	chars_scanned = 1;
	for(i=0; i < for_end; i++) {
		if( sscanf(code + chars_read, "%c%g%n", &tmp_char, &tmp_float, &chars_scanned) == 2 ) {
			switch(tmp_char) {
				case 'a' :
				case 'A' :
					tp.w0 = tmp_float; break;
				case 'b' :
				case 'B' :
					if( filter == lowpass || filter == highpass ) {
						error(0);
					}
					tp.w1 = tmp_float; tp.isDw = 0; break;
				case 'c' :
				case 'C' :
					if( filter == lowpass || filter == highpass ) {
						error(0);
					}
					tp.w1 = tmp_float; tp.isDw = 1; break;
				case 'w' :
				case 'W' :
					tp.inHz = 0; break;
				default:
					error(0);
			}
			chars_read += chars_scanned;
			chars_scanned = 1;
		} else if( tmp_char == CODE_DELIMITER ) {
			chars_read += chars_scanned;
			break;
		} else {
			error(0);
		}
	}
	
	if( !normalizeTransformParameters( &tp ) ) {
		error(0);
	}
	
	fi->transformP = tp;
	fi->type = filter;
	
	return chars_read;
}

int decodeIirInput(char code[], filterInfo * fi) {
	char tmp_char;
	float tmp_float;
	int i, chars_scanned = 0, chars_read;
	filterType filter;
	iirParameters ip = newIirParameters();
	
	if(code[0] == 'B' && code[1] == 'W') {
		filter = butterworth;
	} else if(code[0] == 'C' && code[1] == '1') {
		filter = chebyshev1;
	} else if(code[0] == 'C' && code[1] == '2') {
		filter = chebyshev2;
	} else {
		error(0);
	}
	
	chars_read = 3;
	chars_scanned = 1;
	for(i=0; i < 6; i++) {
		if( sscanf(code + chars_read, "%c%g%n", &tmp_char, &tmp_float, &chars_scanned) == 2 ) {
			switch(tmp_char) {
				case 'n' :
				case 'N' :
					ip.n = (uint)tmp_float; break;
				case 'a' :
				case 'A' :
					ip.ac = tmp_float; break;
				case 'b' :
				case 'B' :
					ip.as = tmp_float; break;
				case 'w' :
				case 'W' :
					ip.ws = tmp_float; break;
				case 'l' :
				case 'L' :
					ip.inDb = 0; break;
				default:
					error(0);
			}
			chars_read += chars_scanned;
			chars_scanned = 1;	// if only %c was read
		} else if( tmp_char == CODE_DELIMITER ) {
			chars_read += chars_scanned;
			break;
		} else {
			error(0);
		}
	}
	
	if( !normalizeIirParameters( &ip ) ) {
		error(0);
	}

	fi->iirP = ip;
	fi->subtype = filter;
	
	return chars_read;
}