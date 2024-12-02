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

	.. list-table::
		:header-rows: 1

		* - Param
	          - Decription
		* - fileInfo
		  - | Pointer to a ``struct`` :c:struct:`cando_file_ops_create_info`.
		    | The use of pointer to a void is to limit amount
		    | of columns required to define a function.

	Returns:
		| **on success:** pointer to a ``struct`` :c:struct:`cando_file_ops`
		| **on failure:** ``NULL``

=========================================================================================================================================

============================
cando_file_ops_truncate_file
============================

.. c:function:: int cando_file_ops_truncate_file(struct cando_file_ops *flops, const long unsigned int dataSize);

	Adjust file to a size of precisely length bytes

	.. list-table::
		:header-rows: 1

		* - Param
	          - Decription
		* - flops
		  - | Pointer to a valid ``struct`` :c:struct:`cando_file_ops`
		* - dataSize
		  - | Size in bytes file will be `truncate(2)`_'d to.

	Returns:
		| **on success:** 0
		| **on failure:** # < 0

=========================================================================================================================================

=============================
cando_file_ops_zero_copy_info
=============================

.. c:struct:: cando_file_ops_zero_copy_info

	.. c:member::
		size_t dataSize;
		int    infd;
		off_t  *inOffset;
		int    outfd;
		off_t  *outOffset;

	:c:member:`dataSize`
		| Total size of the data to copy

	:c:member:`infd`
		| Input file descriptor to copy data from

	:c:member:`inOffset`
		| Byte offset in the :c:member:`infd` open file to copy from
		| **NOTE:** `splice(2)`_ may updates the variable

	:c:member:`outfd`
		| Output file descriptor to copy data to

	:c:member:`outOffset`
		| Byte offset in the :c:member:`outfd` open file to copy X amount
		| of data from the given offset.
		| **NOTE:** `splice(2)`_ may updates the variable

========================
cando_file_ops_zero_copy
========================

.. c:function:: int cando_file_ops_zero_copy(struct cando_file_ops *flops, const void *fileInfo);

	Sets the data in a file at a given offset up to a given size
	without copying the buffer into userspace.

	.. list-table::
		:header-rows: 1

		* - Param
	          - Decription
		* - flops
		  - | Pointer to a valid ``struct`` :c:struct:`cando_file_ops`
		* - fileInfo
		  - | Pointer to a ``struct`` :c:struct:`cando_file_ops_zero_copy_info`.
		    | The use of pointer to a void is to limit amount
		    | of columns required to define a function.

	Returns:
		| **on success:** 0
		| **on failure:** -1

=========================================================================================================================================

=======================
cando_file_ops_get_data
=======================

.. c:function:: const void *cando_file_ops_get_data(struct cando_file_ops *flops, const unsigned long int offset);

	Returns file data stored at a given offset

	.. list-table::
		:header-rows: 1

		* - Param
	          - Decription
		* - flops
		  - | Pointer to a valid ``struct`` :c:struct:`cando_file_ops`
		* - offset
		  - | Byte offset within the file

	Returns:
		| **on success:** Pointer to file data at a given index
		| **on failure:** ``NULL``

=========================================================================================================================================

=======================
cando_file_ops_get_line
=======================

.. c:function:: const char *cando_file_ops_get_line(struct cando_file_ops *flops, const unsigned long int lineNum);

	Returns file data stored at a given line.
	Returned output excludes newline character.

	.. list-table::
		:header-rows: 1

		* - Param
	          - Decription
		* - flops
		  - | Pointer to a valid ``struct`` :c:struct:`cando_file_ops`
		* - lineNum
		  - | Line in file to get data from

	Returns:
		| **on success:** Pointer to file data at a given line
		| **on failure:** ``NULL``

=========================================================================================================================================

=============================
cando_file_ops_get_line_count
=============================

.. c:function:: long int cando_file_ops_get_line_count(struct cando_file_ops *flops);

	Returns the amount of lines a file contains

	.. list-table::
		:header-rows: 1

		* - Param
	          - Decription
		* - flops
		  - | Pointer to a valid ``struct`` :c:struct:`cando_file_ops`

	Returns:
		| **on success:** Line count
		| **on failure:** -1

=========================================================================================================================================

=====================
cando_file_ops_get_fd
=====================

.. c:function:: int cando_file_ops_get_fd(struct cando_file_ops *flops);

	Returns file descriptor to open file

	.. list-table::
		:header-rows: 1

		* - Param
	          - Decription
		* - flops
		  - | Pointer to a valid ``struct`` :c:struct:`cando_file_ops`

	Returns:
		| **on success:** File descriptor to open file
		| **on failure:** -1

=========================================================================================================================================

============================
cando_file_ops_get_data_size
============================

.. c:function:: size_t cando_file_ops_get_data_size(struct cando_file_ops *flops);

	Returns size of the `mmap(2)`_'d buffer associated
	with the open file.

	.. list-table::
		:header-rows: 1

		* - Param
	          - Decription
		* - flops
		  - | Pointer to a valid ``struct`` :c:struct:`cando_file_ops`

	Returns:
		| **on success:** Size of the `mmap(2)`_ buffer
		| **on failure:** -1

=========================================================================================================================================

===========================
cando_file_ops_get_filename
===========================

.. c:function:: const char *cando_file_ops_get_filename(struct cando_file_ops *flops);

	Return file name of open file associated with
	the ``struct`` :c:struct:`cando_file_ops` context.

	.. list-table::
		:header-rows: 1

		* - Param
	          - Decription
		* - flops
		  - | Pointer to a valid ``struct`` :c:struct:`cando_file_ops`

	Returns:
		| **on success:** File name of open file
		| **on failure:** ``NULL``

=========================================================================================================================================

============================
cando_file_ops_set_data_info
============================

.. c:struct:: cando_file_ops_set_data_info

	.. c:member::
		unsigned long int offset;
		size_t            dataSize;
		const void        *data;

	:c:member:`offset`
		| Byte offset within the file

	:c:member:`dataSize`
		| Size in bytes to copy into file at :c:member:`offset`

	:c:member:`data`
		| Data to copy at the given file offset

=======================
cando_file_ops_set_data
=======================

.. c:function:: int cando_file_ops_set_data(struct cando_file_ops *flops, const void *fileInfo);

	Sets data in a file at a given offset up to a given size.

	.. list-table::
		:header-rows: 1

		* - Param
	          - Decription
		* - flops
		  - | Pointer to a valid ``struct`` :c:struct:`cando_file_ops`
		* - fileInfo
		  - | Pointer to a ``struct`` :c:struct:`cando_file_ops_set_data_info`.
		    | The use of pointer to a void is to limit amount
		    | of columns required to define a function.

	Returns:
		| **on success:** 0
		| **on failure:** -1

=========================================================================================================================================

======================
cando_file_ops_destroy
======================

.. c:function:: void cando_file_ops_destroy(struct cando_file_ops *flops);

	Frees any allocated memory and closes FD's (if open) created after
	:c:func:`cando_file_ops_create` call.

	.. list-table::
		:header-rows: 1

		* - Param
	          - Decription
		* - flops
		  - | Pointer to a valid ``struct`` :c:struct:`cando_file_ops`

=========================================================================================================================================

.. _mmap(2):  https://man7.org/linux/man-pages/man2/mmap.2.html
.. _open(2):  https://man7.org/linux/man-pages/man2/open.2.html
.. _creat(2):  https://man7.org/linux/man-pages/man2/open.2.html
.. _pipe(2):  https://man7.org/linux/man-pages/man2/pipe.2.html
.. _truncate(2):  https://man7.org/linux/man-pages/man2/pipe.2.html
.. _splice(2):  https://man7.org/linux/man-pages/man2/splice.2.html
