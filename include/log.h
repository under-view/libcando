#ifndef HANDY_LOG_H
#define HANDY_LOG_H

#include <stdio.h>
#include <libgen.h>

/*
 * enum handy_log_level_type (Handy Log Level Type)
 *
 * Sets which messages of a given type to print and is used to
 * help determine which ANSI Escape Codes to utilize.
 *
 * @macros
 * HANDY_LOG_NONE    - Term color
 * HANDY_LOG_SUCCESS - Green
 * HANDY_LOG_DANGER  - Red
 * HANDY_LOG_INFO    - Light purple
 * HANDY_LOG_WARNING - Yellow
 * HANDY_LOG_RESET   - Term color
 * HANDY_LOG_ALL     - Term color
 */
enum handy_log_level_type {
	HANDY_LOG_NONE     = 0x00000000,
	HANDY_LOG_SUCCESS  = 0x00000001,
	HANDY_LOG_DANGER   = 0x00000002,
	HANDY_LOG_INFO     = 0x00000004,
	HANDY_LOG_WARNING  = 0x00000008,
	HANDY_LOG_RESET    = 0x00000010,
	HANDY_LOG_ALL      = 0xFFFFFFFF
};


/*
 * handy_log_level_set: Sets which type of messages that are allowed
 *                      to be printed to an open file stream.
 *
 * parameters:
 * @level - 32-bit integer representing the type of log to print to
 *          an open file stream. Each log type has a different color.
 */
void
handy_log_level_set (enum handy_log_level_type level);


/*
 * handy_log: Provides applications/library way to write to @stream
 *            with colors to display different error message.
 *
 * paramaters:
 * @type   - The type of color to use with log
 * @stream - Pointer to open file stream to print messages to
 * @fmt    - Format of the log passed to va_args
 * @...    - Variable list arguments
 */
void
handy_log (enum handy_log_level_type type, FILE *stream, const char *fmt, ...);


/*
 * Macros defined to structure the message
 * timestamp - [file:function:line] message
 */
#define handy_logme(logType, fmt, ...) \
	handy_log(logType, stdout, "[%s:%s:%d] " fmt, basename(__FILE__), __func__, __LINE__, ##__VA_ARGS__)

#endif /* HANDY_LOG_H */
