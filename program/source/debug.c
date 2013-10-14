#include "../headers/variables.h"
#include "../headers/communication.h"
#include "../headers/debug.h"
#include "../headers/iir_functions.h"
#include "../headers/diagnostics.h"

FILE * tcBuffer[TC_CALL_MAX_DEPTH];
int tcDepth = 0;

int runTestcase( char * tcname ) {
	FILE * tc;
	int i;
	int commentLevel = 0;

	if( tcDepth >= TC_CALL_MAX_DEPTH ) {
		error(70);
	}

	tc = tcBuffer[tcDepth] = fopen( tcname, "r");
	tcDepth++;

	if( tc != NULL ) {
		do {
			if( fgets( inputBuffer, INPUT_BUF_SIZE, tc ) != NULL ) {
				switch( inputBuffer[0] ) {

					case '*' :
						switch( inputBuffer[1] ) {
							case '{' :
								commentLevel++;
								break;
							case '}' :
								if( commentLevel > 0 ) commentLevel--;
								break;
							case '*' :
								continue;
							default:
								out( inputBuffer );
						}
						break;

					case '>' :
					case '<' :
					case '?' :
					case '#' :
						if( !commentLevel ) {
							out(inputBuffer);
							for( i = 0; i < INPUT_BUF_SIZE; i++ ) {
								if( inputBuffer[i] == '\n' ) {
									inputBuffer[i] = '\0';
									break;
								}
							}

							parseInput( inputBuffer, INPUT_BUF_SIZE );
							if( getErrors() ) {
								printErrors();
								clearErrors();
							}
							out("\n");
						}
						break;

					default:
						if( !commentLevel ) {
							if( COPY_COMMENTS == 1 ) {
								out("-\n");
							} else if( COPY_COMMENTS == 2 ) {
								out("-");
								out( inputBuffer );
							}
							out("\n");
						}
				}
			} else {
				fclose(tc);
				tc = NULL;
				tcDepth--;
				error(69);
			}
		} while( !feof(tc) );

	} else {
		tcDepth--;
		error(68);
	}

	fclose(tc);
	tc = NULL;
	tcDepth--;
	return 1;

}

int debugFilterInfo(char * s, int l ) {
	int i;
	unsigned char level = 0;
	filterInfo * fi = &filterBank[actualFilter];
	
	for( i = 0; i < l; i++ ) {
		switch( s[i] ) {
			case 'R' :
				level |= P_REFERENT;
				break;
			case 'T' :
				level |= P_TRANSFORMED;
				break;
			case 'B' :
				level |= P_BIQUAD;
				break;
			case 'D' :
				level |= P_DIGITALIZED;
				break;
			case 'A' :
				level |= 0x111;
				break;
			default:
				;//warn
		}
	}

	if( createIirFilter( fi, level | P_PRINT ) ) {
		return 1;
	} else {
		error(79);
	}

}

simulationParameters defaultPulseSimulationParameters() {
	simulationParameters sp;

	sp.delay		= 0;
	sp.pulse		= 1;
	sp.length		= F_SAMPLING;		// 1 sec
	sp.repetitions	= 1;
	sp.amplitude	= INPUT_MAX / 2;
	sp.offset		= 0;

	return sp;
}

simulationParameters defaultSinusSimulationParameters() {
	simulationParameters sp;

	sp.delay		= 0;
	sp.pulse		= 0;
	sp.length		= F_SAMPLING / 1000;	// 1000Hz
	sp.repetitions	= 10;
	sp.amplitude	= INPUT_MAX / 2;
	sp.offset		= 0;

	return sp;
}

int simulatePSRespose( char * s, int l, char sinus ) {
	int nextLetter, prevLetter;
	simulationParameters sp;

	if( sinus == 1 ) {
		sp = defaultSinusSimulationParameters();
	} else {
		sp = defaultPulseSimulationParameters();
	}

	if( l == 0 ) {
		error(85);
	}

	nextLetter = 0;
	for( ; nextLetter < l && s[nextLetter] != ','; nextLetter++ );
	if( s[nextLetter] == ',' ) {
		s[nextLetter] = '\0';
	}
	
	while( ++nextLetter < l ) {
		prevLetter = nextLetter++;
		for( ; nextLetter < l && s[nextLetter] != ','; nextLetter++ );
		
		if( s[prevLetter] >= 'a' && s[prevLetter] <= 'z' ) {
			s[prevLetter] = s[prevLetter] - 32;
		}

		switch( s[prevLetter] ) {
			case 'D' :
				if ( sscanf( s + prevLetter + 1, "%d", &sp.delay ) != 1 ) {
					error(86);
				}
				break;
			case 'P' :
				if ( sscanf( s + prevLetter + 1, "%d", &sp.pulse ) != 1 ) {
					error(87);
				}
				break;
			case 'L' :
				if ( sscanf( s + prevLetter + 1, "%d", &sp.length ) != 1 ) {
					error(88);
				}
				break;
			case 'R' :
				if ( sscanf( s + prevLetter + 1, "%d", &sp.repetitions ) != 1 ) {
					error(89);
				}
				break;
			case 'A' :
				if ( sscanf( s + prevLetter + 1, "%d", &sp.amplitude ) != 1 ) {
					error(90);
				}
				break;
			case 'O' :
				if ( sscanf( s + prevLetter + 1, "%d", &sp.offset ) != 1 ) {
					error(91);
				}
				break;
			default:
				error(92);
		}
	}

	if( sinus == 1 ) {

	} else {
		if( !simulatePulse( s, sp ) ) {
			error(93);
		}
	}

	return 1;
}

int simulatePulse( const char * filename, simulationParameters sp ) {
	uint r, j;
	FILE * sf;
	INPUT_TYPE in;
	OUTPUT_TYPE out;

	in = sp.amplitude+sp.offset;

	sf = fopen( filename, "w");
	if( sf != NULL ) {
		fprintf(sf, "%% -- PULSE SIMULATION ---------------\n%%\n");
		fprintf(sf, "%%\tDelay:             %d\n", sp.delay);
		fprintf(sf, "%%\tPulse length:      %d\n", sp.pulse);
		fprintf(sf, "%%\tPeriod length:     %d\n", sp.length);
		fprintf(sf, "%%\tNumber of periods: %d\n", sp.repetitions);
		fprintf(sf, "%%\tSimulation length: %d\n", sp.repetitions*sp.length+sp.delay);
		fprintf(sf, "%%\tPulse amplitude:   %d\n", sp.amplitude);
		fprintf(sf, "%%\tOffset:            %d\n", sp.offset);
		fprintf(sf, "%% -----------------------------------\n");

		for( j=0; j<sp.delay; j++ ) {
			startClock();
			out = filterBank[actualFilter].filter(0, coeffLines[actualFilter], delayLine);
			stopClock();
			setTick();
			fprintf(sf, "0 %d\n", out);
		}
		for( r=0; r<sp.repetitions; r++ ) {
			for( j=0; j<sp.pulse; j++ ) {
				startClock();
				out = filterBank[actualFilter].filter(in, coeffLines[actualFilter], delayLine);
				stopClock();
				setTick();
				fprintf(sf, "%g %d\n", (float)in, out);
			}
			for( ; j<sp.length; j++ ) {
				startClock();
				out = filterBank[actualFilter].filter(sp.offset, coeffLines[actualFilter], delayLine);
				stopClock();
				setTick();
				fprintf(sf, "%g %d\n", (float)sp.offset, out);
			}
		}


		fclose(sf);
		return 1;

	} else {
		error(84);
	}
}

	