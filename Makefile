build:
	gcc -Wall -pedantic -Iinclude -lm src/main.c -o clef

clean:
	rm clef

