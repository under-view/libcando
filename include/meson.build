main_headers_dict = {}

main_headers = [
  'log.h',
  'macros.h'
]

if file_ops.enabled()
  main_headers += ['file-ops.h']

  headers = '#include "file-ops.h"'

  main_headers_dict += {'FILE_OPS_INTERFACE': headers}
endif

conf_data = configuration_data(main_headers_dict)

conf = configure_file(input: 'cando.h.in',
                      output: 'cando.h',
                      configuration: conf_data,
                      install: false)

main_headers += [conf]
install_headers(main_headers, subdir: 'cando')
