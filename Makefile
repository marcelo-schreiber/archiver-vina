# Name of the project
PROJ_NAME=vina++

# .c files
C_SOURCE=$(wildcard ./src/*.c)

# .h files
H_SOURCE=$(wildcard ./src/*.h)

# Object files
OBJ=$(subst .c,.o,$(subst src,objects,$(C_SOURCE)))

# Compiler and linker
CC=gcc

# Flags for compiler
CFLAGS=-c         \
         -W         \
         -Wall      \
		 		 -std=c99   \

# Command used at clean target
RM = rm -rf

# Compilation and linking
all: objFolder $(PROJ_NAME)
 
$(PROJ_NAME): $(OBJ)
	$(CC) $^ -o $@ 
 
./objects/%.o: ./src/%.c ./src/%.h
	$(CC) $< $(CFLAGS) -o $@
 
./objects/main.o: ./src/main.c $(H_SOURCE)
	$(CC) $< $(CFLAGS) -o $@
 
objFolder:
	@mkdir -p objects
 
clean:
	$(RM) ./objects/*.o $(PROJ_NAME) *~

.PHONY: all clean