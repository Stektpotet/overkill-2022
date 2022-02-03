SOURCES := $(shell find src/ -type f | grep -E '\.(h|c)(pp)?$$')

.PHONY: help
help:
	@echo -e "Try one of these make targets:\n"
	@grep "^\.PHONY: " Makefile | cut -d" " -f2- | sed -e "s/ /\n/g" | grep -v "^_"

.PHONY: run run-with-music
run: build
	cd build && ./glowbox
run-with-music: build
	cd build && ./glowbox --enable-music

.PHONY: build
build: build/glowbox
build/glowbox: ${SOURCES} | build/Makefile has-make
	make -C build

build/Makefile: | build/ _submodules has-cmake
	cd build && cmake ..

.PHONY: _submodules
_submodules: | has-git
	@git submodule update --init

.PHONY: clean
clean:
	# TODO: submodules
	@rm -rfv build/*


# === helpers: ===

# make folders, use as order-only prerequisite
%/:
	mkdir -p $*

# check if program is available in PATH, use as order-only prerequisite
has-%:
	@command -v $* >/dev/null || ( \
		echo "ERROR: Command '$*' not found! Make sure it is installed and available in PATH"; \
		false; \
	) >&2
