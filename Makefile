CC = gcc
PROD_CFLAGS = -Wall -pedantic -O3
DEV_CFLAGS = -Wall -pedantic -Wextra -fsanitize=address -fno-omit-frame-pointer -Wconversion
DEVASYNC_CFLAGS = -pthread -Wall -pedantic -Wextra -fsanitize=address -fno-omit-frame-pointer -Wconversion
SOURCE = $(wildcard src/*.c src/lib/*.c src/utils/*.c)
OUT = build/app

.PHONY: clean build dev

dev:
	mkdir -p build
	$(CC) $(DEV_CFLAGS) $(SOURCE) -o $(OUT)
	./$(OUT)

devAsync:
	mkdir -p build
	$(CC) $(DEVASYNC_CFLAGS) ./src/lib/async.c ./test.c -o build/asyncTest
	./build/asyncTest
	

build:
	mkdir -p build 
	$(CC) $(PROD_CFLAGS) $(SOURCE) -o $(OUT)
	clear
	./$(OUT)

clean:
	rm -rf build
	clear
