CC = gcc
PROD_CFLAGS = -Wall -pedantic -O3
DEV_CFLAGS = -Wall -pedantic -Wextra -fsanitize=address -fno-omit-frame-pointer
SOURCE = $(wildcard *.c) 
OUT = build/app

.PHONY: clean build dev

dev:
	mkdir -p build
	$(CC) $(DEV_CFLAGS) $(SOURCE) -o $(OUT)
	clear
	./$(OUT)

build:
	mkdir -p build 
	$(CC) $(PROD_CFLAGS) $(SOURCE) -o $(OUT)
	clear
	./$(OUT)

clean:
	rm -rf build
