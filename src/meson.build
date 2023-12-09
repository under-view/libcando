fs = [ 'log.c' ]
libhandy_deps = []

libhandy_static = static_library(
  'handy', files(fs),
  dependencies: libhandy_deps,
  include_directories: inc,
  c_args: pargs
)
