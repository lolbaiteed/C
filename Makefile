CC = gcc
PROD_CFLAGS = -Wall -pedantic -O3
DEV_CFLAGS = -Wall -pedantic -Wextra -fsanitize=address -fno-omit-frame-pointer -Wconversion
DEVASYNC_CFLAGS = -pthread -Wall -pedantic -Wextra -fsanitize=address -fno-omit-frame-pointer -Wconversion

# Tool definitions
PREPROCESSOR = src/tools/preproc

# File finding
ZC_SOURCE = $(wildcard src/*.zc src/lib/*.zc src/utils/*.zc)
GENERATED_C = $(ZC_SOURCE:.zc=.c)
SOURCE = $(wildcard src/*.c src/lib/*.c src/utils/*.c)
# We want to exclude the preprocessor source itself from the main build if it accidentally gets caught, 
# but it's in src/tools so the wildcard src/utils/*.c won't catch it. 
# However, `wildcard src/*.c` might catch stuff we don't want if we aren't careful.
# For now, current patterns are safe.

OUT = build/app

.PHONY: clean build dev preprocess

# Ensure preprocessor is built
$(PREPROCESSOR): src/tools/preprocessor.c
	$(CC) src/tools/preprocessor.c -o $(PREPROCESSOR)

# Pattern rule to convert .zc to .c
%.c: %.zc $(PREPROCESSOR)
	$(PREPROCESSOR) $< > $@

preprocess: $(GENERATED_C)

dev: preprocess
	mkdir -p build
	$(CC) $(DEV_CFLAGS) $(SOURCE) -o $(OUT)
	./$(OUT)

devAsync:
	mkdir -p build
	$(CC) $(DEVASYNC_CFLAGS) ./src/lib/async.c ./test.c -o build/asyncTest
	./build/asyncTest

build: preprocess
	mkdir -p build 
	$(CC) $(PROD_CFLAGS) $(SOURCE) -o $(OUT)
	clear
	./$(OUT)

clean:
	rm -rf build
	rm -f $(GENERATED_C)
	clear