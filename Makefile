.PHONY: all cmake build clef run

all: run

cmake:
	mkdir -p build
	cmake -B build

build: cmake
	make --directory build

clef: build
	@if [ ! -L ./clef ]; then ln -s build/clef ./clef; fi

run: build clef
	./clef
