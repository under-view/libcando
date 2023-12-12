#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>
#define HAVE_POSIX_TIMER
#include <time.h>
#ifdef CLOCK_MONOTONIC
# define CLOCKID CLOCK_MONOTONIC
#else
# define CLOCKID CLOCK_REALTIME
#endif

#include "log.h"
#include "macros.h"

static enum handy_log_level_type logLevel;


void
handy_log_init (enum handy_log_level_type level)
{
	logLevel = level;
}


/* ANSI Escape Codes */
static const char *term_colors[] = {
	[HANDY_LOG_NONE]    = "",
	[HANDY_LOG_SUCCESS] = "\e[32;1m",
	[HANDY_LOG_DANGER]  = "\e[31;1m",
	[HANDY_LOG_INFO]    = "\e[35;1m",
	[HANDY_LOG_WARNING] = "\e[33;1m",
	[HANDY_LOG_RESET]   = "\x1b[0m"
};


void
handy_log (enum handy_log_level_type type, FILE *stream, const char *fmt, ...)
{
	va_list args;
	time_t rawtime;
	char buffer[26];

	if (!(type & logLevel))
		return;

	/* create message time stamp */
	rawtime = time(NULL);

	/* generate time */
	strftime(buffer, sizeof(buffer), "%F %T - ", localtime_r(&rawtime, &(struct tm){}));
	fprintf(stream, "%s", buffer);

	/* Set terminal color */
	fprintf(stream, "%s", term_colors[type]);

	va_start(args, fmt);
	vfprintf(stream, fmt, args);
	va_end(args);

	/* Reset terminal colors */
	fprintf(stream, "%s", term_colors[HANDY_LOG_RESET]);

	/* Flush buffer */
	fprintf(stream, "\n");
}
