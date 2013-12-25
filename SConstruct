env = Environment()
env['CFLAGS'] = ['-I/usr/local/include']
env['LINKFLAGS'] = ['-L/usr/local/lib']
env['LIBS'] = ['ev']
env.Program('out/about_empty_loop', ['src/about_empty_loop.c'])
env.Program('out/about_timer', ['src/about_timer.c'])
