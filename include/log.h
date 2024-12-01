#ifndef CANDO_LOG_H
#define CANDO_LOG_H

#include "macros.h"


/*
 * @brief enum cando_log_level_type (Handy Log Level Type)
 *
 *        Sets which messages of a given type to print and is used to
 *        help determine which ANSI Escape Codes to utilize.
 *
 * @macro CANDO_LOG_NONE    - Term color
 * @macro CANDO_LOG_SUCCESS - Green
 * @macro CANDO_LOG_DANGER  - Red
 * @macro CANDO_LOG_INFO    - Light purple
 * @macro CANDO_LOG_WARNING - Yellow
 * @macro CANDO_LOG_RESET   - Term color
 * @macro CANDO_LOG_ALL     - All above colors
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
 * @brief Sets which type of messages that are allowed
 *        to be printed to an open file.
 *
 * @param level - 32-bit integer representing the type of log to print to
 *                an open file stream. Each log type has a different color.
 */
CANDO_API
void
cando_log_level_set (enum cando_log_level_type level);


/*
 * @brief Sets the internal global write file descriptor
 *        to caller define file descriptor.
 *
 *        Default is set to STDOUT_FILENO.
 *
 * @param fd - File descriptor to an open file
 */
CANDO_API
void
cando_log_write_fd_set (int fd);


/*
 * @brief Returns a string with the error defined given
 *        a context with first members of the context
 *        being
 *        	* int code;
 *        	* char error[4096];
 *
 * @param context - Pointer to an arbitrary context
 *
 * @returns
 * 	on success: Error string
 * 	on failure: NULL
 */
CANDO_API
const char *
cando_log_get_error (void *context);


/*
 * @brief Returns error code of a given context
 *
 * @param context - Pointer to an arbitrary context
 *
 * @returns
 * 	on success: error code or errno
 * 	on failure: 0
 */
CANDO_API
unsigned int
cando_log_get_error_code (void *context);


/*
 * @brief Provides applications/library way to write to an open file
 *        with a time stamp and ansi color codes to colorize
 *        different message.
 *
 * @param type - The type of color to use with log
 * @param fmt  - Format of the log passed to va_args
 * @param ...  - Variable list arguments
 */
CANDO_API
void
cando_log_time (enum cando_log_level_type type,
		const char *fmt,
		...);


/*
 * @brief Provides applications/library way to write to an open file
 *        without time stamp with ansi color codes to colorize
 *        different message.
 *
 * @param type - The type of color to use with log
 * @param fmt  - Format of the log passed to va_args
 * @param ...  - Variable list arguments
 */
CANDO_API
void
cando_log_notime (enum cando_log_level_type type,
		  const char *fmt,
		  ...);


/*
 * Macros defined to structure the message
 * timestamp - [file:function:line] message
 */
#define cando_log(logType, fmt, ...) \
	cando_log_time(logType, "[%s:%s:%d] " fmt, __FILE_NAME__, __func__, __LINE__, ##__VA_ARGS__)

#define cando_log_err(fmt, ...) \
	cando_log_time(CANDO_LOG_DANGER, "[%s:%s:%d] " fmt, __FILE_NAME__, __func__, __LINE__, ##__VA_ARGS__)

#define cando_log_print(logType, fmt, ...) \
	cando_log_notime(logType, fmt, ##__VA_ARGS__)

#endif /* CANDO_LOG_H */
