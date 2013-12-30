env = Environment()

env['CFLAGS'] = ['-I/usr/local/include', '-Wall', '-Werror', '-Wunused-parameter']
env['LINKFLAGS'] = ['-L/usr/local/lib']
env['LIBS'] = ['ev']

env.Program('out/about_empty_loop', ['src/about_empty_loop.c'])
env.Program('out/about_timer', ['src/about_timer.c'])
env.Program('out/about_timer_override', ['src/about_timer_override.c'])
env.Program('out/about_user_context', ['src/about_user_context.c'])
env.Program('out/about_io', ['src/about_io.c'])
env.Program('out/about_now', ['src/about_now.c'])
env.Program('out/about_periodic', ['src/about_periodic.c'])
