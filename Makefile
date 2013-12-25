default: check

build:
	scons

check: build
	bash scripts/runabouts.sh	\
		out/about_empty_loop	\
		out/about_timer		\
		out/about_user_context	\
		out/about_io		\

clean:
	scons -c
	find . -name "*~" -exec rm -vf \{\} \;
	rm -rf out
