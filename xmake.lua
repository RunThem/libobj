--- Project name
set_project('libobj')

--- Project version
set_version('0.0.1')

--- xmake configure
set_xmakever('2.6.1')

--- Build mode
add_rules('mode.debug', 'mode.valgrind', 'mode.release')

--- No warning
set_warnings('all', 'error')

add_cflags('-Wno-free-nonheap-object')

--- main target
target('obj', function()
  set_kind('static')
  add_files('src/obj.c')
  add_headerfiles('src/obj.h')
end)

target('test', function()
  set_kind('binary')
  set_default(false)
  add_files('src/main.c')

  add_deps('obj')
end)
