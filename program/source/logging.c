#include "../headers/logging.h"

#ifdef LOGGING_ENABLED

#include "../headers/out.h"
#include <time.h>

FILE * logfile = NULL;

int msgLog( const char * msg ) {
	if( logfile != NULL ) {
		if( fprintf(logfile, "%s", msg) < 0 ) {
			perror("Error! Can't write to logfile!\n");
			return 0;
		}
	}
	return 1;
}

void startLog( const char * filename ) {
	char bfr[42];
	time_t rawtime;
	struct tm * t;

	time(&rawtime);
	t = localtime( &rawtime );

	if( logfile != NULL ) {
		sprintf(bfr, "\n-- Force close log at: %4d-%02d-%02d %02d:%02d\n", t->tm_year+1900, t->tm_mon, t->tm_mday, t->tm_hour, t->tm_min);
		out(bfr);

		if ( fclose( logfile ) == EOF ) {
			sprintf(bfr, "Error! Can't close previous logfile!\n");
		} else {
			sprintf(bfr, "Warn! Previous logfile force closed!\n");
		}

		logfile = NULL;
		out(bfr);

	}

	logfile = fopen( filename, LOGGING_MODE );
	if( logfile != NULL ) {
		sprintf(bfr, "\n-- Log started at: %4d-%02d-%02d %02d:%02d\n\n", t->tm_year+1900, t->tm_mon, t->tm_mday, t->tm_hour, t->tm_min);
	} else {
		sprintf(bfr, "Error! Can't open logfile!\n");
	}

	out(bfr);

}

void closeLog() {
	char bfr[42];
	time_t rawtime;
	struct tm * t;

	time(&rawtime);
	t = localtime( &rawtime );

	if( logfile != NULL ) {
		sprintf(bfr, "\n-- Closing log at: %4d-%02d-%02d %02d:%02d\n", t->tm_year+1900, t->tm_mon, t->tm_mday, t->tm_hour, t->tm_min);
		out(bfr);

		if ( fclose( logfile ) == EOF ) {
			sprintf(bfr, "Error! Can't close logfile!\n");
			out(bfr);
		} else {
			logfile = NULL;
		}

	} else {
		sprintf(bfr, "Warn! Can't find open logfile!\n");
		out(bfr);
	}
}

#else

#include "../headers/error.h"

int msgLog( const char * msg ) {
	//warn
}

void startLog( const char * filename ) {
	//warn
}

void closeLog() {
	//warn
}

#endif
