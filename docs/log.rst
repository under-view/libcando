.. default-domain:: C

log
===

Header: cando/log.h

Table of contents (click to go)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

======
Macros
======

1. :c:macro:`cando_log`
#. :c:macro:`cando_log_err`
#. :c:macro:`cando_log_print`

=====
Enums
=====

1. :c:enum:`cando_log_level_type`

======
Unions
======

=======
Structs
=======

=========
Functions
=========

1. :c:func:`cando_log_level_set`
#. :c:func:`cando_log_time`
#. :c:func:`cando_log_notime`

API Documentation
~~~~~~~~~~~~~~~~~

====================
cando_log_level_type
====================

.. c:enum:: cando_log_level_type

	.. c:macro::
		CANDO_LOG_NONE
		CANDO_LOG_SUCCESS
		CANDO_LOG_DANGER
		CANDO_LOG_INFO
		CANDO_LOG_WARNING
		CANDO_LOG_RESET
		CANDO_LOG_ALL

	Log level options used by
		:c:func:`cando_log_level_set`
		:c:func:`cando_log`
		:c:macro:`cando_logme`

	:c:macro:`CANDO_LOG_NONE`
		| Value set to ``0x00000000``
		| Term color

	:c:macro:`CANDO_LOG_SUCCESS`
		| Value set to ``0x00000001``
		| Green

	:c:macro:`CANDO_LOG_DANGER`
		| Value set to ``0x00000002``
		| Red

	:c:macro:`CANDO_LOG_INFO`
		| Value set to ``0x00000004``
		| Light purple

	:c:macro:`CANDO_LOG_WARNING`
		| Value set to ``0x00000008``
		| Yellow

	:c:macro:`CANDO_LOG_RESET`
		| Value set to ``0x00000010``
		| Term color

	:c:macro:`CANDO_LOG_ALL`
		| Value set to ``0xFFFFFFFF``
		| Term color

=========================================================================================================================================

===================
cando_log_level_set
===================

.. c:function:: void cando_log_level_set(enum cando_log_level_type level);

	Sets which type of messages that are allowed to be printed to an open file stream.

	Parameters:
		| **level:**
		| 32-bit integer representing the type of log to print to
		| an open file stream. Each log type has a different color.

=========================================================================================================================================

==============
cando_log_time
==============

.. c:function:: void cando_log_time(enum cando_log_level_type type, FILE *stream, const char *fmt, ...);

	Provides applications/library way to write to ``stream``
	with a time stamp and ansi color codes to colorize
	different message.

	Parameters:
		| **type:** The type of color to use with log
		| **stream:** Pointer to open file stream to print messages to
		| **fmt:** Format of the log passed to va_args
		| **... :** Variable list arguments

================
cando_log_notime
================

.. c:function:: void cando_log_notime(enum cando_log_level_type type, FILE *stream, const char *fmt, ...);

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
cando_log
=========

.. c:macro:: cando_log(logType, fmt, ...)

	Log format

	timestamp - [file:function:line] message

	Prints to ``stdout`` using ansi color codes to color text.

	.. code-block::

		#define cando_log(logType, fmt, ...) \
			cando_log_time(logType, stdout, "[%s:%s:%d] " fmt, basename(__FILE__), __func__, __LINE__, ##__VA_ARGS__)

=============
cando_log_err
=============

.. c:macro:: cando_log_err(fmt, ...)

	Log format

	timestamp - [file:function:line] message

	Prints to ``stderr`` with ansi color codes the color **RED**

	.. code-block::

		#define cando_log_err(fmt, ...) \
			cando_log_time(CANDO_LOG_DANGER, stderr, "[%s:%s:%d] " fmt, basename(__FILE__), __func__, __LINE__, ##__VA_ARGS__)

===============
cando_log_print
===============

.. c:macro:: cando_log_print(logType, fmt, ...)

	Prints to ``stdout`` using ansi color codes to color text.

	.. code-block::

		#define cando_log_print(logType, fmt, ...) \
			cando_log_notime(logType, stdout, fmt, ##__VA_ARGS__)
