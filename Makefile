.PHONY: all cmake build clef run

all: run

cmake:
	cmake -B build

build: cmake
	make --directory build

run: build
	./clef
