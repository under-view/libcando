#ifndef CANDO_LOG_H
#define CANDO_LOG_H

#include <stdlib.h>
#include <stdio.h>
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

#include "macros.h"


/*
 * enum cando_log_level_type (Handy Log Level Type)
 *
 * Sets which messages of a given type to print and is used to
 * help determine which ANSI Escape Codes to utilize.
 *
 * @macros
 * CANDO_LOG_NONE    - Term color
 * CANDO_LOG_SUCCESS - Green
 * CANDO_LOG_DANGER  - Red
 * CANDO_LOG_INFO    - Light purple
 * CANDO_LOG_WARNING - Yellow
 * CANDO_LOG_RESET   - Term color
 * CANDO_LOG_ALL     - Term color
 */
enum cando_log_level_type {
	CANDO_LOG_NONE     = 0x00000000,
	CANDO_LOG_SUCCESS  = 0x00000001,
	CANDO_LOG_DANGER   = 0x00000002,
	CANDO_LOG_INFO     = 0x00000004,
	CANDO_LOG_WARNING  = 0x00000008,
	CANDO_LOG_RESET    = 0x00000010,
	CANDO_LOG_ALL      = 0xFFFFFFFF
};


enum cando_log_level_type logLevel = CANDO_LOG_ALL;


/*
 * cando_log_level_set: Sets which type of messages that are allowed
 *                      to be printed to an open file stream.
 *
 * parameters:
 * @level - 32-bit integer representing the type of log to print to
 *          an open file stream. Each log type has a different color.
 */
CANDO_STATIC_INLINE void
cando_log_level_set (enum cando_log_level_type level)
{
	logLevel = level;
}


/* ANSI Escape Codes */
static const char *tcolors[] = {
	[CANDO_LOG_NONE]    = "",
	[CANDO_LOG_SUCCESS] = "\e[32;1m",
	[CANDO_LOG_DANGER]  = "\e[31;1m",
	[CANDO_LOG_INFO]    = "\e[35;1m",
	[CANDO_LOG_WARNING] = "\e[33;1m",
	[CANDO_LOG_RESET]   = "\x1b[0m"
};


/*
 * cando_log_time: Provides applications/library way to write to @stream
 *                 with a time stamp and ansi color codes to colorize
 *                 different message.
 *
 * paramaters:
 * @type   - The type of color to use with log
 * @stream - Pointer to open file stream to print messages to
 * @fmt    - Format of the log passed to va_args
 * @...    - Variable list arguments
 */
void
cando_log_time (enum cando_log_level_type type, FILE *stream, const char *fmt, ...)
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
	fprintf(stream, "%s", tcolors[type]);

	va_start(args, fmt);
	vfprintf(stream, fmt, args);
	va_end(args);

	/* Reset terminal colors */
	fprintf(stream, "%s", tcolors[CANDO_LOG_RESET]);
}


/*
 * cando_log_notime: Provides applications/library way to write to @stream
 *                   without time stamp with ansi color codes to colorize
 *                   different message.
 *
 * paramaters:
 * @type   - The type of color to use with log
 * @stream - Pointer to open file stream to print messages to
 * @fmt    - Format of the log passed to va_args
 * @...    - Variable list arguments
 */
void
cando_log_notime (enum cando_log_level_type type, FILE *stream, const char *fmt, ...)
{
	va_list args;

	if (!(type & logLevel))
		return;

	/* Set terminal color */
	fprintf(stream, "%s", tcolors[type]);

	va_start(args, fmt);
	vfprintf(stream, fmt, args);
	va_end(args);

	/* Reset terminal colors */
	fprintf(stream, "%s", tcolors[CANDO_LOG_RESET]);
}


/*
 * Macros defined to structure the message
 * timestamp - [file:function:line] message
 */
#define cando_log(logType, fmt, ...) \
	cando_log_time(logType, stdout, "[%s:%s:%d] " fmt, basename(__FILE__), __func__, __LINE__, ##__VA_ARGS__)

#define cando_log_err(fmt, ...) \
	cando_log_time(CANDO_LOG_DANGER, stderr, "[%s:%s:%d] " fmt, basename(__FILE__), __func__, __LINE__, ##__VA_ARGS__)

#define cando_log_print(logType, fmt, ...) \
	cando_log_notime(logType, stdout, fmt, ##__VA_ARGS__)

#endif /* CANDO_LOG_H */
