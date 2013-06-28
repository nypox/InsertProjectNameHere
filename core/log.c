#include <stdio.h>
#include <string.h>
#include <time.h>
#include "log.h"

FILE * log_f_info;
FILE * log_f_error;
// log_f_warn?

// Must be set at runtime because FILE *, not fd
void log_init (void) {
	log_f_info		= stdout;
	log_f_error		= stderr;
}

// DRY
FILE * type2file (type)
{
	switch (type)
	{
		case LOG_INFO:
			return log_f_info;
		case LOG_ERROR:
			return log_f_error;
	}

	return NULL;
}

uint8_t log_open (uint8_t type, char * fn)
{
	FILE * f = type2file(type);

	f = fopen(fn, "a");
	if (f == NULL)
	{
		return 0;
	}

	return 1;
}

// In case you need it, could be merged to log_open too
//uint8_t log_open_sock (uint8_t type, int sock)
//{
//	FILE * f = type2file(type);
//
//	f = fdopen(sock, "a"); // "w" will do, too
//	if (f == NULL)
//	{
//		return 0;
//	}
//
//	return 1;
//}

// Add ANSI escapes to stderr messages?
void log_print (char * msg, uint8_t type)
{
	FILE * f = type2file(type);

	time_t t = time(NULL);
	struct tm * tm = localtime(&t);

	fprintf(f, "[%2i:%2i:%2i %2i.%2i.%2i] %s\n", tm->tm_hour, tm->tm_min, tm->tm_sec, tm->tm_mday, tm->tm_mon, tm->tm_year, msg);
}

void log_exit (void)
{
	if (log_f_info != stdout && log_f_info != stderr)
	{
		fclose(log_f_info);
	}
	if (log_f_error != stdout && log_f_error != stderr)
	{
		fclose(log_f_error);
	}
}
