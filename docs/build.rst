Build libhandy
==============

Meson
~~~~~

.. code-block::
        :linenos:

        $ meson setup [options] build
        $ ninja -C build -j$(nproc)
        $ sudo ninja -C build install

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

        # Clone libhandy or create a handy.wrap under <source_root>/subprojects
        project('name', 'c')

        handy_dep = dependency('handy', fallback : 'handy', 'handy_dep')

        executable('exe', 'src/main.c', dependencies : handy_dep)

Documentation (Sphinx)
~~~~~~~~~~~~~~~~~~~~~~

libhandy uses sphinx framework for documentation. Primarily utilizing `The C Domain`_.

https://www.sphinx-doc.org/en/master/man/sphinx-build.html

**Dependencies**

- python3-pip

.. code-block::

        $ git clone https://github.com/under-view/handy.git
        $ cd handy
        $ sudo pip3 install -r docs/requirements.txt

        # If no build directory exists
        $ meson setup -Ddocs=true build

        # If build directory exists
        $ meson configure -Ddocs=true build

.. _The C Domain: https://www.sphinx-doc.org/en/master/usage/restructuredtext/domains.html#the-c-domain
