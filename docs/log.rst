.. default-domain:: C

log
===

Header: handy/log.h

Table of contents (click to go)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

======
Macros
======

1. :c:macro:`handy_log`
#. :c:macro:`handy_log_err`
#. :c:macro:`handy_log_print`

=====
Enums
=====

1. :c:enum:`handy_log_level_type`

======
Unions
======

=======
Structs
=======

=========
Functions
=========

1. :c:func:`handy_log_level_set`
#. :c:func:`handy_log_time`
#. :c:func:`handy_log_notime`

API Documentation
~~~~~~~~~~~~~~~~~

====================
handy_log_level_type
====================

.. c:enum:: handy_log_level_type

	.. c:macro::
		HANDY_LOG_NONE
		HANDY_LOG_SUCCESS
		HANDY_LOG_DANGER
		HANDY_LOG_INFO
		HANDY_LOG_WARNING
		HANDY_LOG_RESET
		HANDY_LOG_ALL

	Log level options used by
		:c:func:`handy_log_level_set`
		:c:func:`handy_log`
		:c:macro:`handy_logme`

	:c:macro:`HANDY_LOG_NONE`
		| Value set to ``0x00000000``
		| Term color

	:c:macro:`HANDY_LOG_SUCCESS`
		| Value set to ``0x00000001``
		| Green

	:c:macro:`HANDY_LOG_DANGER`
		| Value set to ``0x00000002``
		| Red

	:c:macro:`HANDY_LOG_INFO`
		| Value set to ``0x00000004``
		| Light purple

	:c:macro:`HANDY_LOG_WARNING`
		| Value set to ``0x00000008``
		| Yellow

	:c:macro:`HANDY_LOG_RESET`
		| Value set to ``0x00000010``
		| Term color

	:c:macro:`HANDY_LOG_ALL`
		| Value set to ``0xFFFFFFFF``
		| Term color

=========================================================================================================================================

===================
handy_log_level_set
===================

.. c:function:: void handy_log_level_set(enum handy_log_level_type level);

	Sets which type of messages that are allowed to be printed to an open file stream.

	Parameters:
		| **level:**
		| 32-bit integer representing the type of log to print to
		| an open file stream. Each log type has a different color.

=========================================================================================================================================

==============
handy_log_time
==============

.. c:function:: void handy_log_time(enum handy_log_level_type type, FILE *stream, const char *fmt, ...);

	Provides applications/library way to write to ``stream``
	with a time stamp and ansi color codes to colorize
	different message.

	Parameters:
		| **type:** The type of color to use with log
		| **stream:** Pointer to open file stream to print messages to
		| **fmt:** Format of the log passed to va_args
		| **... :** Variable list arguments

================
handy_log_notime
================

.. c:function:: void handy_log_notime(enum handy_log_level_type type, FILE *stream, const char *fmt, ...);

	Provides applications/library way to write to ``stream``
	without time stamp with ansi color codes to colorize
	different message.

	Parameters:
		| **type:** The type of color to use with log
		| **stream:** Pointer to open file stream to print messages to
		| **fmt:** Format of the log passed to va_args
		| **... :** Variable list arguments

=========================================================================================================================================

=========
handy_log
=========

.. c:macro:: handy_log(logType, fmt, ...)

	Log format

	timestamp - [file:function:line] message

	Prints to ``stdout`` using ansi color codes to color text.

	.. code-block::

		#define handy_log(logType, fmt, ...) \
			handy_log_time(logType, stdout, "[%s:%s:%d] " fmt, basename(__FILE__), __func__, __LINE__, ##__VA_ARGS__)

=============
handy_log_err
=============

.. c:macro:: handy_log_err(fmt, ...)

	Log format

	timestamp - [file:function:line] message

	Prints to ``stderr`` with ansi color codes the color **RED**

	.. code-block::

		#define handy_log_err(fmt, ...) \
			handy_log_time(HANDY_LOG_DANGER, stderr, "[%s:%s:%d] " fmt, basename(__FILE__), __func__, __LINE__, ##__VA_ARGS__)

===============
handy_log_print
===============

.. c:macro:: handy_log_print(logType, fmt, ...)

	Prints to ``stdout`` using ansi color codes to color text.

	.. code-block::

		#define handy_log_print(logType, fmt, ...) \
			handy_log_notime(logType, stdout, fmt, ##__VA_ARGS__)
