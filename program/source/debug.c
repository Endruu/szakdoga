#include "../headers/variables.h"

#ifdef DEBUG_FUNCTIONS_ENABLED

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
	char buff[100];

	if( tcDepth >= TC_CALL_MAX_DEPTH ) {
		error(70);
	}

	sprintf(buff, TESTCASE_DIR "%s", tcname);

	tc = tcBuffer[tcDepth] = fopen( buff, "r");
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
				level |= P_ALL;
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

void getTimeStamp( char * s ) {
	time_t rawtime;
	struct tm * timeinfo;

	time (&rawtime);
	timeinfo = localtime (&rawtime);

	strftime (s, 13, "%y%m%d%H%M%H", timeinfo);
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
	int tmp;

	if( sinus == 1 ) {
		sp = defaultSinusSimulationParameters();
	} else {
		sp = defaultPulseSimulationParameters();
	}

	/*if( l == 0 ) {
		error(85);
	}*/

	nextLetter = 0;
	for( ; nextLetter < l && s[nextLetter] != CODE_DELIMITER; nextLetter++ );
	if( s[nextLetter] == CODE_DELIMITER ) {
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
				if ( sscanf( s + prevLetter + 1, "%d", &tmp ) == 1 ) {
					if( tmp < 0 ) {
						tmp *= -1;
						// warn
					}
					sp.delay = tmp;
				} else {
					error(86);
				}
				break;
			case 'P' :
				if ( sscanf( s + prevLetter + 1, "%d", &tmp ) == 1 ) {
					if( tmp < 0 ) {
						tmp *= -1;
						// warn
					}
					sp.pulse = tmp;
				} else {
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
		if( !simulateSinus( s, sp ) ) {
			error(97);
		}
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
	OUTPUT_TYPE output;
	char buff[100];
	char ts[13];

	getTimeStamp(ts);

	if( filename[0] == '\0' ) {
		sprintf(buff, SIM_OUTPUT_DIR "P%s.txt", ts);
	} else {
		sprintf(buff, SIM_OUTPUT_DIR "%s", filename);
	}

	sf = fopen( buff, "w");
	if( sf != NULL ) {

		fprintf(sf, "%% -- PULSE SIMULATION ------------------------------\n");
		fprintf(sf, "%%\tSimulation ID:     %12s\n", ts);
		fprintf(sf, "%%\tDelay:             %12d (%.5gms)\n", sp.delay, (float)(sp.delay*1000)/F_SAMPLING);
		fprintf(sf, "%%\tPulse length:      %12d (%.5gms)\n", sp.pulse, (float)(sp.pulse*1000)/F_SAMPLING);
		fprintf(sf, "%%\tPeriod length:     %12d (%.5gms)\n", sp.length, (float)(sp.length*1000)/F_SAMPLING);
		fprintf(sf, "%%\tNumber of periods: %12d\n", sp.repetitions);
		fprintf(sf, "%%\tSimulation length: %12d (%.5gms)\n", sp.repetitions*sp.length+sp.delay, (float)(sp.repetitions*sp.length+sp.delay)/F_SAMPLING);
		fprintf(sf, "%%\tPulse amplitude:   %12d (%.3g%%)\n", sp.amplitude, (float)sp.amplitude/(float)INPUT_MAX*100.0);
		fprintf(sf, "%%\tOffset:            %12d (%.3g%%)\n", sp.offset, (float)sp.offset/(float)INPUT_MAX*100.0);
		fprintf(sf, "%% --------------------------------------------------\n");

		in = sp.amplitude+sp.offset;

		for( j=0; j<sp.delay; j++ ) {
			startClock();
			output = filterBank[actualFilter].filter(0, coeffLines[actualFilter], delayLine);
			stopClock();
			setTick();
			fprintf(sf, "0 %d\n", output);
		}

		for( r=0; r<sp.repetitions; r++ ) {
			for( j=0; j<sp.pulse; j++ ) {
				startClock();
				output = filterBank[actualFilter].filter(in, coeffLines[actualFilter], delayLine);
				stopClock();
				setTick();
				fprintf(sf, "%g %d\n", (float)in, output);
			}
			for( ; j<sp.length; j++ ) {
				startClock();
				output = filterBank[actualFilter].filter(sp.offset, coeffLines[actualFilter], delayLine);
				stopClock();
				setTick();
				fprintf(sf, "%g %d\n", (float)sp.offset, output);
			}
		}

		out("Simulation complete!\nOutput file: ");
		out(buff);
		sprintf(buff, "\nID: %s\n", ts);
		out(buff);
		fclose(sf);
		return 1;

	} else {
		error(84);
	}
}

int simulateSinus( const char * filename, simulationParameters sp ) {
	uint r, j;
	FILE * sf;
	INPUT_TYPE in;
	OUTPUT_TYPE output;
	char buff[100];
	char ts[13];

	getTimeStamp(ts);

	if( filename[0] == '\0' ) {
		sprintf(buff, SIM_OUTPUT_DIR "S%s.txt", ts);
	} else {
		sprintf(buff, SIM_OUTPUT_DIR "%s", filename);
	}

	sf = fopen( buff, "w");
	if( sf != NULL ) {

		fprintf(sf, "%% -- SINUS WAVE SIMULATION -------------------------\n");
		fprintf(sf, "%%\tSimulation ID:     %12s\n", ts);
		fprintf(sf, "%%\tDelay:             %12d (%.5gms)\n", sp.delay, (float)(sp.delay*1000)/F_SAMPLING);
		fprintf(sf, "%%\tPhase delay:       %12d (%.5g°)\n", sp.pulse, -(float)sp.pulse/(float)sp.length*360);
		fprintf(sf, "%%\tPeriod length:     %12d (%.5gHz)\n", sp.length, F_SAMPLING/(float)sp.length);
		fprintf(sf, "%%\tNumber of periods: %12d\n", sp.repetitions);
		fprintf(sf, "%%\tSimulation length: %12d (%.5gms)\n", sp.repetitions*sp.length+sp.delay, (float)(sp.repetitions*sp.length+sp.delay)/F_SAMPLING);
		fprintf(sf, "%%\tSinus amplitude:   %12d (%.3g%%)\n", sp.amplitude, (float)sp.amplitude/(float)INPUT_MAX*100.0);
		fprintf(sf, "%%\tOffset:            %12d (%.3g%%)\n", sp.offset, (float)sp.offset/(float)INPUT_MAX*100.0);
		fprintf(sf, "%% --------------------------------------------------\n");

		for( j=0; j<sp.delay; j++ ) {
			startClock();
			output = filterBank[actualFilter].filter(0, coeffLines[actualFilter], delayLine);
			stopClock();
			setTick();
			fprintf(sf, "0 %d\n", output);
		}

		for( r=0; r<sp.repetitions; r++ ) {

			for( j=0; j<sp.length; j++ ) {
				in = (INPUT_TYPE)( (float)sp.amplitude*sin(2.0*PI*(float)(j+sp.pulse)/(float)sp.length)+sp.offset );
				startClock();
				output = filterBank[actualFilter].filter(in, coeffLines[actualFilter], delayLine);
				stopClock();
				setTick();
				fprintf(sf, "%d %d\n", in, output);
			}
		}

		out("Simulation complete!\nOutput file: ");
		out(buff);
		sprintf(buff, "\nID: %s\n", ts);
		out(buff);
		fclose(sf);
		return 1;

	} else {
		error(95);
	}
}

#endif
