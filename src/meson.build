fs = [
  'src/log.c',
  'src/mm.c'
]

if file_ops.enabled()
  fs += ['src/file-ops.c']
endif

if vsock_tcp.enabled()
  fs += ['src/vsock-tcp.c']
endif

if vsock_udp.enabled()
  fs += ['src/vsock-udp.c']
endif
