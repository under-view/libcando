.. default-domain:: C

futex (Fast Userspace Mutex)
============================

Header: cando/futex.h

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

=========
Functions
=========

1. :c:func:`cando_futux_lock`
#. :c:func:`cando_futex_unlock`
#. :c:func:`cando_futex_unlock_force`

API Documentation
~~~~~~~~~~~~~~~~~

================
cando_futex_lock
================

.. c:function:: void cando_futex_lock(cando_atomic_u32 *fux);

| Atomically updates futex value to the locked state.
| If value can't be changed inform kernel that a
| process needs to be put to sleep. Sets errno to
| `EINTR`_ if a call to :c:func:`cando_futex_unlock_force`
| is made.

	.. list-table::
		:header-rows: 1

		* - Param
	          - Decription
		* - fux
		  - | Pointer to 32-bit integer storing futex.

=========================================================================================================================================

==================
cando_futex_unlock
==================

.. c:function:: void cando_futex_unlock(cando_atomic_u32 *fux);

| Atomically update futex value to the unlocked state.
| Then inform kernel to wake up all processes/threads
| watching the futex.

	.. list-table::
		:header-rows: 1

		* - Param
	          - Decription
		* - fux
		  - | Pointer to 32-bit integer storing futex.

=========================================================================================================================================

========================
cando_futex_unlock_force
========================

.. c:function:: void cando_futex_unlock_force(cando_atomic_u32 *fux);

| Atomically update futex value to the force unlocked state.
| Then inform kernel to wake up all processes/threads
| watching the futex. When force unlocking if a
| process/thread is waiting on the lock. Process/thread
| will exit setting errno to `EINTR`_. Recommended to use
| function in a signal handler.

	.. list-table::
		:header-rows: 1

		* - Param
	          - Decription
		* - fux
		  - | Pointer to 32-bit integer storing futex.

=========================================================================================================================================

.. _EINTR: https://man7.org/linux/man-pages/man3/errno.3.html
