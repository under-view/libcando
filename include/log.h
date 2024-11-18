#ifndef CANDO_LOG_H
#define CANDO_LOG_H

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
	CANDO_LOG_NONE    = 0x00000000,
	CANDO_LOG_SUCCESS = 0x00000001,
	CANDO_LOG_DANGER  = 0x00000002,
	CANDO_LOG_INFO    = 0x00000004,
	CANDO_LOG_WARNING = 0x00000008,
	CANDO_LOG_RESET   = 0x00000010,
	CANDO_LOG_ALL     = 0xFFFFFFFF
};


/*
 * cando_log_level_set: Sets which type of messages that are allowed
 *                      to be printed to an open file stream.
 *
 * parameters:
 * @level - 32-bit integer representing the type of log to print to
 *          an open file stream. Each log type has a different color.
 */
CANDO_EXTERNC void
cando_log_level_set (enum cando_log_level_type level);


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
CANDO_EXTERNC void
cando_log_time (enum cando_log_level_type type,
                void *stream,
		const char *fmt,
		...);


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
CANDO_EXTERNC void
cando_log_notime (enum cando_log_level_type type,
                  void *stream,
		  const char *fmt,
		  ...);

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
