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


all: clean Build run

Build:
	$(CC) $(S_FILES) -o $(EXEC) $(CC_SDL)

run:
	$(EXEC)

build_run: clean Build run

clean:
	rm -f $(EXEC)
	rm -f source/.*.swp
