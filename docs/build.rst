Build libcando
==============

Dependencies
~~~~~~~~~~~~

For better dependency version control underview builds all packages required from source.

=======================
Build Underview Depends
=======================

See `build-underview-depends (dependencies)`_ to get going. For this repo
you only need the system dependencies.

.. code-block::

	$ git clone https://github.com/under-view/build-underview-depends.git
	$ source setenvars.sh

=================
Yocto Project SDK
=================

Download SDK from `build-underview-depends (releases)`_

.. code-block::

	$ ./x86_64-0.1.0-underview.sh
	sdk # Folder to place libs
	$ source environment-setup-zen1-underview-linux

Meson
~~~~~

=======
Options
=======

All options/features are disabled by default.

.. code-block::
	:linenos:

	c_std=c11
	buildtype=release
	default_library=shared
	tests=true           # Default [false]
	docs=true            # Default [false]
	file-ops=enabled     # Default [disabled]
	vsocket-tcp=enabled  # Default [disabled]

======================
Build/Install (Normal)
======================

.. code-block::

	$ meson setup [options] build
	$ ninja install -C build

.. code-block::

	$ meson setup \
		-Dtests="true" \
		-Ddocs="false" \
		-Dfile-ops="enabled" \
		-Dvsock-tcp="enabled" \
		build
	$ ninja install -C build

===================
Build/Install (SDK)
===================

.. code-block::

	# Yocto Project SDK Builds
	$ meson setup \
		--prefix="${SDKTARGETSYSROOT}/usr" \
		--libdir="${SDKTARGETSYSROOT}/usr/lib64" \
		[options] \
		build
	$ ninja install -C build

.. code-block::

	$ meson setup \
		--prefix="${SDKTARGETSYSROOT}/usr" \
		--libdir="${SDKTARGETSYSROOT}/usr/lib64" \
		-Dtests="true" \
		-Ddocs="false" \
		-Dfile-ops="enabled" \
		-Dvsock-tcp="enabled" \
		build
	$ ninja install -C build

=======
Include
=======

.. code-block::
	:linenos:

	# Clone libcando or create a cando.wrap under <source_root>/subprojects
	project('name', 'c')

	cando_dep = dependency('cando', required : true)

	executable('exe', 'src/main.c', dependencies : cando_dep)

.. code-block::
	:linenos:

	#include <cando/cando.h>

Documentation (Sphinx)
~~~~~~~~~~~~~~~~~~~~~~

libcando uses sphinx framework for documentation. Primarily utilizing `The C Domain`_.

https://www.sphinx-doc.org/en/master/man/sphinx-build.html

============
Dependencies
============

- python3-pip

==========
Build Docs
==========

.. code-block::

	$ git clone https://github.com/under-view/libcando.git
	$ cd libcando
	$ sudo pip3 install -r docs/requirements.txt

	# If no build directory exists
	$ meson setup -Ddocs=true build

	# If build directory exists
	$ meson configure -Ddocs=true build

	$ ninja docs -C build

.. _build-underview-depends: https://github.com/under-view/build-underview-depends
.. _build-underview-depends (dependencies): https://github.com/under-view/build-underview-depends#dependencies
.. _build-underview-depends (releases): https://github.com/under-view/build-underview-depends/releases
.. _The C Domain: https://www.sphinx-doc.org/en/master/usage/restructuredtext/domains.html#the-c-domain
