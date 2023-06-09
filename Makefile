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
				 -g 			 \
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
 
test:
	./$(PROJ_NAME) -i backup.vpp ./tests/Lorem.txt ./tests/pixil-frame-0.png ./tests/nokia_standard.mp3
	./$(PROJ_NAME) -x backup.vpp

check:
	diff -w ./tests/Lorem.txt ./tests/new_Lorem.txt
	diff -w ./tests/pixil-frame-0.png ./tests/new_pixil-frame-0.png
	diff -w ./tests/nokia_standard.mp3 ./tests/new_nokia_standard.mp3

leak-check:
	valgrind --leak-check=full --show-leak-kinds=all ./$(PROJ_NAME) -i backup.vpp ./tests/Lorem.txt ./tests/pixil-frame-0.png ./tests/nokia_standard.mp3
	valgrind --leak-check=full --show-leak-kinds=all ./$(PROJ_NAME) -x backup.vpp

clean:
	$(RM) ./objects/*.o $(PROJ_NAME) *~

.PHONY: all clean