default: check

build:
	scons

check: build
	bash scripts/runabouts.sh $(shell find out -name "about_*")

clean:
	scons -c
	find . -name "*~" -exec rm -vf \{\} \;
	rm -rf out
