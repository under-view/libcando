.. default-domain:: C

shm (Shared Memory)
===================

Header: cando/shm.h

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

1. :c:struct:`cando_shm`
#. :c:struct:`cando_shm_create_info`
#. :c:struct:`cando_shm_data_info`

=========
Functions
=========

1. :c:func:`cando_shm_create`
#. :c:func:`cando_shm_data_read`
#. :c:func:`cando_shm_data_write`
#. :c:func:`cando_shm_get_fd`
#. :c:func:`cando_shm_get_data`
#. :c:func:`cando_shm_get_data_size`
#. :c:func:`cando_shm_destroy`
#. :c:func:`cando_shm_sizeof`

API Documentation
~~~~~~~~~~~~~~~~~

===================
cando_sem (private)
===================

| Structure defining the cando_sem (cando sempahore) instance.

.. c:struct:: cando_sem

	.. c:member::
		sem_t *read_sem;
		char  read_sem_file[SEM_FILE_NAME_MAX];

	:c:member:`read_sem`
		| Pointer to POSIX semaphore used to synchronize reads.

	:c:member:`read_sem_file`
		| Name of the POSIX semaphore used to synchronize reads.

===================
cando_shm (private)
===================

| Structure defining the cando_shm instance.

.. c:struct:: cando_shm

	.. c:member::
		struct cando_log_error_struct err;
		bool                          free;
		int                           fd;
		char                          shm_file[SHM_FILE_NAME_MAX];
		size_t                        data_sz;
		void                          *data;
		sem_t                         *write_sem;
		char                          write_sem_file[SEM_FILE_NAME_MAX];
		int                           sem_count;
		struct cando_sem              sems[SEM_COUNT_MAX];

	:c:member:`err`
		| Stores information about the error that occured
		| for the given instance and may later be retrieved
		| by caller.

	:c:member:`free`
		| If structure allocated with `calloc(3)`_ member will be
		| set to true so that, we know to call `free(3)`_ when
		| destroying the instance.

	:c:member:`fd`
		| Open file descriptor to POSIX shared memory.

	:c:member:`shm_file`
		| Name of the POSIX shared memory file starting with ``'/'``.

	:c:member:`data`
		| Pointer to `mmap(2)`_ map'd shared memory data.

	:c:member:`data_sz`
		| Total size of the shared memory region mapped with `mmap(2)`_.

	:c:member:`write_sem`
		| Pointer to POSIX semaphore used to syncronize writes.

	:c:member:`write_sem_file`
		| Name of the POSIX semaphore used to synchronize writes.

	:c:member:`sem_count`
		| Amount of semaphores in :c:member:`sems`.

	:c:member:`sems`
		| Array of ``struct`` :c:struct:`cando_sem` which contain pointers
		| to read semaphores.

=========================================================================================================================================

=====================
cando_shm_create_info
=====================

| Structure passed to :c:func:`cando_shm_create` used
| to define shared memory and semaphore names.

.. c:struct:: cando_shm_create_info

	.. c:member::
		const char *shm_file;
		size_t     shm_size;
		const char *sem_file;
		int        sem_count;

	:c:member:`shm_file`
		| Shared memory file name. Must start
		| with the character ``'/'``.

	:c:member:`shm_size`
		| Size of shared memory.

	:c:member:`sem_file`
		| Prefix of semaphore file name. Must
		| start with the character ``'/'``.

	:c:member:`sem_count`
		| Amount of POSIX semaphores to create.
		| May be zero.

.. c:function:: struct cando_shm *cando_shm_create(struct cando_shm *shm, const void *shm_info);

| Creates POSIX shared memory and semaphores.

	.. list-table::
		:header-rows: 1

		* - Param
	          - Decription
		* - shm
		  - | May be ``NULL`` or a pointer to a ``struct`` :c:struct:`cando_shm`.
		    | If ``NULL`` memory will be allocated and return to
		    | caller. If not ``NULL`` address passed will be used
		    | to store the newly created ``struct`` :c:struct:`cando_shm`
		    | instance.
		* - shm_info
		  - | Implementation uses a pointer to a
		    | ``struct`` :c:struct:`cando_shm_create_info`.
		    | no other implementation may be passed to
		    | this parameter.

	Returns:
		| **on success:** Pointer to a ``struct`` :c:struct:`cando_shm`
		| **on failure:** ``NULL``

=========================================================================================================================================

===================
cando_shm_data_info
===================

| Structure defining what operations to perform
| and data to retrieve during calls to
| :c:func:`cando_shm_data_read` and :c:func:`cando_shm_data_write`.

.. c:struct:: cando_shm_data_info

	.. c:member::
		void          *data;
		size_t        size;
		size_t        offset;
		int           sem_index;
		unsigned char block : 1;

	:c:member:`data`
		| Pointer to a buffer that will either be used
		| to store shm data or write to shm data.

	:c:member:`size`
		| Size in bytes to read from or write to shared memory.

	:c:member:`offset`
		| Byte offset in shared memory to either read
		| from or write to.

	:c:member:`sem_index`
		| Read semaphore index to either lock or unlock.

	:c:member:`block`
		| Determines if caller wants to perform a blocking
		| or busy wait when waiting for the read/write
		| semaphore to lock/unlock.

===================
cando_shm_data_read
===================

.. c:function:: int cando_shm_data_read(struct cando_shm *shm, const void *shm_info);

| Reads data stored in shared memory at
| caller defined offset and writes into
| a caller defined buffer.

	.. list-table::
		:header-rows: 1

		* - Param
	          - Decription
		* - shm
		  - | Pointer to a valid ``struct`` :c:struct:`cando_shm`.
		* - shm_info
		  - | Must pass a pointer to a ``struct`` :c:struct:`cando_shm_data_info`.

	Returns:
		| **on success:** 0
		| **on failure:** -1

====================
cando_shm_data_write
====================

.. c:function:: int cando_shm_data_write(struct cando_shm *shm, const void *shm_info);

| Write data stored in caller defined buffer
| into shared memory at a caller defined
| shared memory offset.

	.. list-table::
		:header-rows: 1

		* - Param
	          - Decription
		* - shm
		  - | Pointer to a valid ``struct`` :c:struct:`cando_shm`.
		* - shm_info
		  - | Must pass a pointer to a ``struct`` :c:struct:`cando_shm_data_info`.

	Returns:
		| **on success:** 0
		| **on failure:** -1

=========================================================================================================================================

================
cando_shm_get_fd
================

.. c:function:: int cando_shm_get_fd(struct cando_shm *shm);

| Returns file descriptor to the POSIX shared memory
| created after call to :c:func:`cando_shm_create`.

	.. list-table::
		:header-rows: 1

		* - Param
	          - Decription
		* - shm
		  - | Pointer to a valid ``struct`` :c:struct:`cando_shm`.

	Returns:
		| **on success:** File descriptor to POSIX shared memory
		| **on failure:** -1

=========================================================================================================================================

==================
cando_shm_get_data
==================

.. c:function:: void *cando_shm_get_data(struct cando_shm *shm);

| Returns `mmap(2)`_ map'd POSIX shared memory buffer
| created after call to :c:func:`cando_shm_create`.

	.. list-table::
		:header-rows: 1

		* - Param
	          - Decription
		* - shm
		  - | Pointer to a valid ``struct`` :c:struct:`cando_shm`.

	Returns:
		| **on success:** Pointer to map'd shared memory buffer
		| **on failure:** NULL

=========================================================================================================================================

=======================
cando_shm_get_data_size
=======================

.. c:function:: size_t cando_shm_get_data_size(struct cando_shm *shm);

| Returns size of POSIX shared memory buffer
| created after call to :c:func:`cando_shm_create`.

	.. list-table::
		:header-rows: 1

		* - Param
	          - Decription
		* - shm
		  - | Pointer to a valid ``struct`` :c:struct:`cando_shm`.

	Returns:
		| **on success:** Size of POSIX shared memory buffer
		| **on failure:** Maximum size or -1

=========================================================================================================================================

=================
cando_shm_destroy
=================

.. c:function:: void cando_shm_destroy(struct cando_shm *shm);

| Frees any allocated memory and closes FD's (if open) created after
| :c:func:`cando_shm_create` call.

	.. list-table::
		:header-rows: 1

		* - Param
	          - Decription
		* - shm
		  - | Pointer to a valid ``struct`` :c:struct:`cando_shm`.

=========================================================================================================================================

====================
cando_shm_get_sizeof
====================

.. c:function:: int cando_shm_get_sizeof(void);

| Returns size of the internal structure. So,
| if caller decides to allocate memory outside
| of API interface they know the exact amount
| of bytes.

	Returns:
		| **on success:** sizeof(``struct`` :c:struct:`cando_shm`)
		| **on failure:** sizeof(``struct`` :c:struct:`cando_shm`)

=========================================================================================================================================

.. _mmap(2): https://www.man7.org/linux/man-pages/man2/mmap.2.html
.. _calloc(3): https://www.man7.org/linux/man-pages/man3/malloc.3.html
.. _free(3): https://www.man7.org/linux/man-pages/man3/free.3.html
