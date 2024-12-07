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

1. :c:struct:`cando_log_error_struct`

=========
Functions
=========

1. :c:func:`cando_log_set_level`
#. :c:func:`cando_log_set_write_fd`
#. :c:func:`cando_log_get_error`
#. :c:func:`cando_log_get_error_code`
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
		:c:func:`cando_log_set_level`
		:c:macro:`cando_log`
		:c:macro:`cando_log_err`
		:c:macro:`cando_log_print`

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
cando_log_set_level
===================

.. c:function:: void cando_log_set_level(enum cando_log_level_type level);

	Sets which type of messages that are allowed to be printed to an open file.

	.. list-table::
		:header-rows: 1

		* - Param
	          - Decription
		* - level
		  - | 32-bit integer representing the type of log to print to
		    | an open file. Each log type has a different color

=========================================================================================================================================

======================
cando_log_set_write_fd
======================

.. c:function:: void cando_log_set_write_fd(int fd);

	Sets the internal global write file descriptor
	to caller define file descriptor.

	Default is set to ``STDOUT_FILENO``.

	.. list-table::
		:header-rows: 1

		* - Param
	          - Decription
		* - fd
		  - | File descriptor to an open file.

=========================================================================================================================================

======================
cando_log_error_struct
======================

.. c:struct:: cando_log_error_struct

	Structure used to store and acquire
	error string and code for multiple
	struct context's.

	.. c:member::
		unsigned int code;
		char         buffer[CANDO_PAGE_SIZE];

	:c:member:`code`
		| Error code or errno

	:c:member:`buffer`
		| Buffer to store error string

===================
cando_log_get_error
===================

.. c:function:: const char *cando_log_get_error(void *context);

	Returns a string with the error defined given
	caller provided a context with first members
	of the context being a ``struct`` :c:struct:`cando_log_error_struct`.

	.. list-table::
		:header-rows: 1

		* - Param
	          - Decription
		* - context
		  - | Pointer to an arbitrary context.
		    | Start of context must be a ``struct`` :c:struct:`cando_log_error_struct`.
		    | If ``NULL`` passed the internal global will be utilized.

	Returns:
		| **on success:** Passed context error string
		| **on failure:** ``NULL``

========================
cando_log_get_error_code
========================

.. c:function:: unsigned int cando_log_get_error_code(void *context);

	Returns unsigned integer with the error code
	given caller provided a context with first members
	of the context being a ``struct`` :c:struct:`cando_log_error_struct`.

	.. list-table::
		:header-rows: 1

		* - Param
	          - Decription
		* - context
		  - | Pointer to an arbitrary context.
		    | Start of context must be a ``struct`` :c:struct:`cando_log_error_struct`.
		    | If ``NULL`` passed the internal global will be utilized.

	Returns:
		| **on success:** Passed context error code or errno
		| **on failure:** ``UINT32_MAX``

=========================================================================================================================================

==============
cando_log_time
==============

.. c:function:: void cando_log_time(enum cando_log_level_type type, const char *fmt, ...);

	Provides applications/library way to write to an open file
	with a time stamp and ansi color codes to colorize
	different message.

	.. list-table::
		:header-rows: 1

		* - Param
	          - Decription
		* - type
		  - | The type of color to use with log 
		* - fmt
		  - | Format of the log passed to va_args
		* - ...
		  - | Variable list arguments

================
cando_log_notime
================

.. c:function:: void cando_log_notime(enum cando_log_level_type type, const char *fmt, ...);

	Provides applications/library way to write to an open file
	without time stamp with ansi color codes to colorize
	different message.

	.. list-table::
		:header-rows: 1

		* - Param
	          - Decription
		* - type
		  - | The type of color to use with log 
		* - fmt
		  - | Format of the log passed to va_args
		* - ...
		  - | Variable list arguments

=========================================================================================================================================

=========
cando_log
=========

.. c:macro:: cando_log(logType, fmt, ...)

	Log format

	timestamp - [file:line] message

	Default prints to ``stdout`` using ansi color codes to color text.

	Caller may change the open file in which logs are printed to via
	a call to :c:func:`cando_log_set_write_fd`

	.. code-block::

		#define cando_log(logType, fmt, ...) \
			cando_log_time(logType, "[%s:%d] " fmt, __FILE_NAME__, __LINE__, ##__VA_ARGS__)

=============
cando_log_err
=============

.. c:macro:: cando_log_err(fmt, ...)

	Log format

	timestamp - [file:line] message

	Prints to ``stderr`` with ansi color codes the color **RED**.

	Caller may change the open file in which logs are printed to via
	a call to :c:func:`cando_log_set_write_fd`

	.. code-block::

		#define cando_log_err(fmt, ...) \
			cando_log_time(CANDO_LOG_DANGER, "[%s:%d] " fmt, __FILE_NAME__, __LINE__, ##__VA_ARGS__)

===============
cando_log_print
===============

.. c:macro:: cando_log_print(logType, fmt, ...)

	Log format

	NONE

	Default prints to ``stdout`` using ansi color codes to color text.

	Caller may change the open file in which logs are printed to via
	a call to :c:func:`cando_log_set_write_fd`

	.. code-block::

		#define cando_log_print(logType, fmt, ...) \
			cando_log_notime(logType, fmt, ##__VA_ARGS__)
