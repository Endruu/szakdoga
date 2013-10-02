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
	pzkContainer *pzk1 = NULL, *pzk2;
	int level = 0, i;
	filterInfo * fi = &filterBank[actualFilter];

	for( i = 0; i < l; i++ ) {
		switch( s[i] ) {
			case 'R' :
				level |= 0x001;
				break;
			case 'T' :
				level |= 0x010;
				break;
			case 'D' :
				level |= 0x100;
				break;
			case 'A' :
				level |= 0x111;
				break;
			default:
				;//warn
		}
	}

	if( level > 0x001 ) {
		if( pzk1 = createReferentFilter( fi ) ) {
			if( level & 0x001 ) {
				out("\n-- Referent filter: ---------------------------\n\n");
				printPzkContainer( pzk1 );
			}

			if( level > 0x010 ) {
				if( pzk2 = transformFilter( fi, pzk1 ) ) {
					sortPzkContainer( pzk2, SORT_BY_MAGNITUDE, ORDER_UP );
					if( level & 0x010 ) {
						out("\n-- Transformed filter: -----------------------\n\n");
						printPzkContainer( pzk2 );
					}

					if( level > 0x010 ) {
						deletePzkContainer( pzk1 );
						if( pzk1 = digitalizeFilter( fi, pzk2 ) ) {
							out("\n-- Digital filter: ---------------------------\n\n");
							printPzkContainer( pzk1 );
						} else {
							deletePzkContainer( pzk2 );
							error(75);
						}
					}

					deletePzkContainer( pzk2 );
				} else {
					deletePzkContainer( pzk1 );
					error(74);
				}
			}

			deletePzkContainer( pzk1 );
		} else {
			error(73);
		}
	} else {
		error(72);
	}

	return 1;

}