libcmocka = dependency('cmocka', required: true)

progs = [ 'test-log.c' ]

if file_ops.enabled()
  progs += ['test-file-ops.c']
endif

original_args = pargs
foreach p : progs
  exec_name = p.substring(0,-2) # remove .c extension from name

  if exec_name == 'test-file-ops'
    pargs += ['-DTESTER_FILE_ONE="@0@/tests/tester-file-one.txt"'.format(src_root_dir)]
  endif

  exec = executable(exec_name, p,
                    link_with: libcando,
                    dependencies: [libcmocka],
                    include_directories: [inc],
                    c_args: pargs,
                    install: false)

  test(exec_name, exec)
  pargs = original_args
endforeach
