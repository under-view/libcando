project(
  'handy',
  'c',
  version: '0.0.1',
  license: 'MIT',
  meson_version: '>=0.56.0',
  default_options: [
    'warning_level=2',
    'werror=true',
    'prefix=/usr/'
  ],
)

pargs = []
cc = meson.get_compiler('c')
so_version = '.'.join(meson.project_version())
project_name = meson.project_name()
src_root_dir = meson.current_source_dir()
build_root_dir = meson.current_build_dir()

inc = include_directories('include')

subdir('include')
subdir('src')

libhandy = library(
  project_name,
  version: so_version,
  link_whole: libhandy_static,
  include_directories: inc,
  c_args: pargs,
  install: true
)

pkg = import('pkgconfig')

pkglibs = [libhandy]
pkgsubdirs = []
pkg.generate(libraries: pkglibs,
             subdirs : pkgsubdirs,
             version : so_version,
             name : project_name,
             filebase : 'handy',
             description : 'A common utility library for C programs/libraries.')

if get_option('tests')
  subdir('tests')
endif

if get_option('docs')
  docs_dir = src_root_dir + '/docs'
  docs_build_dir = build_root_dir + '/docs'
  sphinx = find_program('sphinx-build-5', 'sphinx-build', required: true)

  if sphinx.found()
    subdir('docs')
    custom_target('docs',
                  command: [sphinx, '-W', '-b', 'html', docs_dir, '@OUTPUT@'],
                  input: docs_src, output: 'docs', install: false, install_dir: '')
  endif
endif
