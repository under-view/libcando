.. default-domain:: C

log
===

Header: handy/log.h

Table of contents (click to go)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

======
Macros
======

1. :c:macro:`handy_logme`

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
#. :c:func:`handy_log`

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

=========
handy_log
=========

.. c:function:: void handy_log(enum handy_log_level_type type, FILE *stream, const char *fmt, ...);

	Provides applications/library way to write to ``stream`` injecting
	asnsi character to display colors for different message types.

	Parameters:
		| **type:** The type of color to use with log
		| **stream:** Pointer to open file stream to print messages to
		| **fmt:** Format of the log passed to va_args
		| **... :** Variable list arguments

=========================================================================================================================================

===========
handy_logme
===========

.. c:macro:: handy_logme(logType, fmt, ...)

	Log format

	timestamp - [file:function:line] message

	.. code-block::

		#define handy_logme(logType, fmt, ...) \
			handy_log(logType, stdout, "[%s:%s:%d] " fmt, basename(__FILE__), __func__, __LINE__, ##__VA_ARGS__)

===============
handy_logme_err
===============

.. c:macro:: handy_logme_err(fmt, ...)

	Log format

	timestamp - [file:function:line] message

	Prints to ``stderr`` with the color **RED**

	.. code-block::

		#define handy_logme_err(fmt, ...) \
			handy_log(HANDY_LOG_DANGER, stderr, "[%s:%s:%d] " fmt, basename(__FILE__), __func__, __LINE__, ##__VA_ARGS__)
