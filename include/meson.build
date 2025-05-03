main_headers_dict = {}

main_headers = [
  'log.h',
  'macros.h',
  'mm.h'
]

if file_ops.enabled()
  main_headers += ['file-ops.h']

  headers = '\n#define CANDO_FILE_OPS_INTERFACE\n#include "file-ops.h"'

  main_headers_dict += {'FILE_OPS_INTERFACE': headers}
endif


if sock_tcp.enabled()
  main_headers += ['sock-tcp.h']

  headers = '\n#define CANDO_SOCK_TCP_INTERFACE\n#include "sock-tcp.h"'

  main_headers_dict += {'SOCK_TCP_INTERFACE': headers}
endif


if sock_udp.enabled()
  main_headers += ['sock-udp.h']

  headers = '\n#define CANDO_SOCK_UDP_INTERFACE\n#include "sock-udp.h"'

  main_headers_dict += {'SOCK_UDP_INTERFACE': headers}
endif


if vsock_tcp.enabled()
  main_headers += ['vsock-tcp.h']

  headers = '\n#define CANDO_VSOCK_TCP_INTERFACE\n#include "vsock-tcp.h"'

  main_headers_dict += {'VSOCK_TCP_INTERFACE': headers}
endif


if vsock_udp.enabled()
  main_headers += ['vsock-udp.h']

  headers = '\n#define CANDO_VSOCK_UDP_INTERFACE\n#include "vsock-udp.h"'

  main_headers_dict += {'VSOCK_UDP_INTERFACE': headers}
endif

conf_data = configuration_data(main_headers_dict)

conf = configure_file(input: 'cando.h.in',
                      output: 'cando.h',
                      configuration: conf_data,
                      install: false)

main_headers += [conf]
install_headers(main_headers, subdir: 'cando')
