# Directories
S_DIR=source
B_DIR=.

# Files
S_FILES=$(S_DIR)/BBMAN.c

# Output
EXEC=$(B_DIR)/salida

# Build settings
CC=gcc
# SDL options
CC_SDL= `sdl2-config --cflags --libs`


all:Build

Build:
	$(CC) $(S_FILES) -o $(EXEC) $(CC_SDL)

build_run:Build
	$(EXEC)

clean:
	rm -f salida
	rm -f source/.*.swp
