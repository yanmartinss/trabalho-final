all: bin/main bin/main_decoder

bin/main: build/main.o build/quadtree.o
	gcc -Wall -Iinclude -o bin/main build/main.o build/quadtree.o

build/main.o: src/main.c include/quadtree.h | build
	gcc -Wall -Iinclude -c src/main.c -o build/main.o

build/quadtree.o: src/quadtree.c include/quadtree.h | build
	gcc -Wall -Iinclude -c src/quadtree.c -o build/quadtree.o

bin/main_decoder: build/main_decoder.o build/quadtree_decoder.o
	gcc -Wall -Iinclude -o bin/main_decoder build/main_decoder.o build/quadtree_decoder.o

build/main_decoder.o: src/main_decoder.c include/quadtree_decoder.h | build
	gcc -Wall -Iinclude -c src/main_decoder.c -o build/main_decoder.o

build/quadtree_decoder.o: src/quadtree_decoder.c include/quadtree_decoder.h | build
	gcc -Wall -Iinclude -c src/quadtree_decoder.c -o build/quadtree_decoder.o

build:
	mkdir -p build

clean:
	rm -f build/*.o bin/main bin/main_decoder