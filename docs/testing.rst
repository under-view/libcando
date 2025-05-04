Testing/Examples
================

First things first see :ref:`build libcando` to enable test option.

Running File Operations
~~~~~~~~~~~~~~~~~~~~~~~

====================
File Operations Test
====================

.. code-block:: sh

	./build/tests/test-file-ops

Running Logging
~~~~~~~~~~~~~~~

============
Logging Test
============

.. code-block:: sh

	./build/tests/test-log

Running Memory Managment
~~~~~~~~~~~~~~~~~~~~~~~~

======================
Memory Management Test
======================

.. code-block:: sh

	./build/tests/test-mm

Running Shared Memory
~~~~~~~~~~~~~~~~~~~~~

==================
Shared Memory Test
==================

.. code-block:: sh

	./build/tests/test-shm

Running TCP/UDP IP sockets
~~~~~~~~~~~~~~~~~~~~~~~~~~

===============
Socket TCP Test
===============

.. code-block:: sh

	./build/tests/test-sock-tcp

===============
Socket UDP Test
===============

.. code-block:: sh

	./build/tests/test-sock-udp

Running VM socket
~~~~~~~~~~~~~~~~~

===================
Load Kernel Modules
===================

**On host machine**

.. code-block:: sh

	sudo modprobe -a vhost_vsock vsock_loopback

==================
VM socket TCP Test
==================

.. code-block:: sh

	./build/tests/test-vsock-tcp

==================
VM socket UDP Test
==================

.. code-block:: sh

	./build/tests/test-vsock-udp
