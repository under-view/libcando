fs = [
  'src/log.c',
  'src/mm.c'
]

if file_ops.enabled()
  fs += ['src/file-ops.c']
endif

if sock_udp.enabled()
  fs += ['src/sock-udp.c']
endif

if vsock_tcp.enabled()
  fs += ['src/vsock-tcp.c']
endif

if vsock_udp.enabled()
  fs += ['src/vsock-udp.c']
endif
