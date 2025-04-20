#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <libgen.h>
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

static int writefd = STDOUT_FILENO;
static enum cando_log_level_type logLevel = CANDO_LOG_NONE;

/* ANSI Escape Codes, terminal colors */
static const char *tcolors[] =
{
	[CANDO_LOG_NONE]    = "",
	[CANDO_LOG_SUCCESS] = "\e[32;1m",
	[CANDO_LOG_ERROR]   = "\e[31;1m",
	[CANDO_LOG_INFO]    = "\e[35;1m",
	[CANDO_LOG_WARNING] = "\e[33;1m",
	[CANDO_LOG_RESET]   = "\x1b[0m"
};


static const char *
_get_error (const unsigned int code)
{
	switch (code) {
		case CANDO_LOG_ERR_INCORRECT_DATA:
			return "Incorrect data passed";
		default:
			return NULL;
	}

	return NULL;
}


void
cando_log_set_level (enum cando_log_level_type level)
{
	logLevel = level;
}


void
cando_log_set_write_fd (const int fd)
{
	writefd = fd;
}


const char *
cando_log_get_error (const void *context)
{
	if (!context)
		return NULL;

	return ((struct cando_log_error_struct*)context)->buffer;
}


unsigned int
cando_log_get_error_code (const void *context)
{
	if (!context)
		return UINT32_MAX;

	return ((struct cando_log_error_struct*)context)->code;
}


void
cando_log_set_error_struct (void *context,
                            const unsigned int code,
                            const char *fmt,
                            ...)
{
	va_list args;

	int offset = 0;

	const char *string = NULL;

	struct cando_log_error_struct *error = context;

	if (!error)
		return;

	CANDO_PAGE_SET_WRITE(error, sizeof(struct cando_log_error_struct));

	error->code = code;

	va_start(args, fmt);
	vsnprintf(error->buffer, sizeof(error->buffer), fmt, args);
	va_end(args);

	string = _get_error(error->code);
	if (string) {
		offset = strnlen(error->buffer, sizeof(error->buffer));
		strncpy(error->buffer+offset, string, sizeof(error->buffer)-offset);
	}

	CANDO_PAGE_SET_READ(error, sizeof(struct cando_log_error_struct));
}


void
cando_log_time (enum cando_log_level_type type,
		const char *fmt,
		...)
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
	dprintf(writefd, "%s", buffer);

	/* Set terminal color */
	dprintf(writefd, "%s", tcolors[type]);

	va_start(args, fmt);
	vdprintf(writefd, fmt, args);
	va_end(args);

	/* Reset terminal colors */
	dprintf(writefd, "%s", tcolors[CANDO_LOG_RESET]);
	fsync(writefd);
}


void
cando_log_notime (enum cando_log_level_type type,
		  const char *fmt,
		  ...)
{
	va_list args;

	if (!(type & logLevel))
		return;

	/* Set terminal color */
	dprintf(writefd, "%s", tcolors[type]);

	va_start(args, fmt);
	vdprintf(writefd, fmt, args);
	va_end(args);

	/* Reset terminal colors */
	dprintf(writefd, "%s", tcolors[CANDO_LOG_RESET]);
	fsync(writefd);
}
