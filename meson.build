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
project_name = meson.project_name()
so_version = '.'.join(meson.project_version())
cc = meson.get_compiler('c')

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

if get_option('tests')
  subdir('tests')
endif

pkg = import('pkgconfig')

pkglibs = [libhandy]
pkgsubdirs = []
pkg.generate(libraries: pkglibs,
             subdirs : pkgsubdirs,
             version : so_version,
             name : project_name,
             filebase : 'handy',
             description : 'A common utility library for C programs/libraries.')
