#include "../headers/variables.h"
#include "../headers/communication.h"
#include "../headers/debug.h"

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