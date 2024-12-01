.. default-domain:: C

file-ops
========

Header: cando/file-ops.h

Table of contents (click to go)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

======
Macros
======

=====
Enums
=====

======
Unions
======

=======
Structs
=======

1. :c:struct:`cando_file_ops`
#. :c:struct:`cando_file_ops_create_info`
#. :c:struct:`cando_file_ops_zero_copy_info`
#. :c:struct:`cando_file_ops_set_data_info`

=========
Functions
=========

1. :c:func:`cando_file_ops_create`
#. :c:func:`cando_file_ops_truncate_file`
#. :c:func:`cando_file_ops_zero_copy`
#. :c:func:`cando_file_ops_get_data`
#. :c:func:`cando_file_ops_get_line`
#. :c:func:`cando_file_ops_get_line_count`
#. :c:func:`cando_file_ops_get_fd`
#. :c:func:`cando_file_ops_get_data_size`
#. :c:func:`cando_file_ops_get_filename`
#. :c:func:`cando_file_ops_set_data`
#. :c:func:`cando_file_ops_destroy`

API Documentation
~~~~~~~~~~~~~~~~~

==============
cando_file_ops
==============

.. c:struct:: cando_file_ops

	.. c:member::
		int    fd;
		int    pipefds[2];
		char   fname[FILE_NAME_LEN_MAX];
		size_t dataSize;
		void   *data;
		void   *retData;

	:c:member:`fd`
		| File descriptor to open file.

	:c:member:`pipefds`
		| File descriptors associated with an open pipe.
		| **pipefds[0]** - Read end of the pipe
		| **pipefds[1]** - Write end of the pipe

	:c:member:`fname`
		| String representing the file name.

	:c:member:`dataSize`
		| Total size of the file that was mapped with `mmap(2)`_.

	:c:member:`data`
		| Pointer to `mmap(2)`_ file data.

	:c:member:`retData`
		| Buffer of data that may be manipulated prior to
                | returning to caller.

==========================
cando_file_ops_create_info
==========================

.. c:struct:: cando_file_ops_create_info

	.. c:member::
		const char        *fileName;
		unsigned long int dataSize;
		off_t             offset;
		unsigned char     createPipe : 1;

	:c:member:`fileName`
		| Full path to file caller wants to `open(2)`_ | `creat(2)`_.

	:c:member:`dataSize`
		| Size in bytes caller newly created file will be.
		| If :c:member:`createPipe` is true this member is ignored.

	:c:member:`offset`
		| Offset within the file to `mmap(2)`_.
		| If :c:member:`createPipe` is true this member is ignored.

	:c:member:`createPipe`
		| Boolean to enable/disable creation of a `pipe(2)`_.

=====================
cando_file_ops_create
=====================

.. c:function:: struct cando_file_ops *cando_file_ops_create(const void *fileInfo);

	Creates or opens caller define file

	========  ==========
	Param     Decription
	========  ==========
	fileInfo  Pointer to a `struct` :c:struct:`cando_file_ops_create_info`.
	          The use of pointer to a void is to limit amount
	          of columns required to define a function.
	========  ==========

	Returns:
		| **on success:** pointer to a ``struct`` :c:struct:`cando_file_ops`
		| **on failure:** NULL

=========================================================================================================================================

.. _mmap(2):  https://man7.org/linux/man-pages/man2/mmap.2.html
.. _open(2):  https://man7.org/linux/man-pages/man2/open.2.html
.. _creat(2):  https://man7.org/linux/man-pages/man2/open.2.html
.. _pipe(2):  https://man7.org/linux/man-pages/man2/pipe.2.html
