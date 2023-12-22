Build libcando
==============

Meson
~~~~~

.. code-block::
        :linenos:

        $ meson setup [options] build
        $ ninja -C build -j$(nproc)

.. code-block::
        :linenos:

	# Yocto Project SDK Builds
	$ meson setup --prefix="${SDKTARGETSYSROOT}/usr" \
	              --libdir="${SDKTARGETSYSROOT}/usr/lib64" \
		      [options] \
	              build
        $ ninja -C build -j$(nproc)

**Meson Options**

All options/features are disabled by default.

.. code-block::
        :linenos:

        c_std=c11
        buildtype=release
        default_library=shared
        tests=true        # Default [false]
        docs=true         # Default [false]

**Use with your Meson project**

.. code-block::
        :linenos:

        # Clone libcando or create a cando.wrap under <source_root>/subprojects
        project('name', 'c')

        cando_dep = dependency('cando', fallback : 'cando', 'cando_dep')

        executable('exe', 'src/main.c', dependencies : cando_dep)

Documentation (Sphinx)
~~~~~~~~~~~~~~~~~~~~~~

libcando uses sphinx framework for documentation. Primarily utilizing `The C Domain`_.

https://www.sphinx-doc.org/en/master/man/sphinx-build.html

**Dependencies**

- python3-pip

.. code-block::

        $ git clone https://github.com/under-view/libcando.git
        $ cd cando
        $ sudo pip3 install -r docs/requirements.txt

        # If no build directory exists
        $ meson setup -Ddocs=true build

        # If build directory exists
        $ meson configure -Ddocs=true build

	$ ninja docs -C build

.. _The C Domain: https://www.sphinx-doc.org/en/master/usage/restructuredtext/domains.html#the-c-domain
