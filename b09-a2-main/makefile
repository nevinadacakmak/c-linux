# Compiler and Flags
CC = gcc
CFLAGS = -Wall -Werror
OBJFLAG = -c

# Source Files
SRC = structs.c parsing_proc.c outputs.c main.c

# Object Files
OBJ = structs.o parsing_proc.o outputs.o main.o

all: FD_table

FD_table: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

main.o: $(SRC)
	$(CC) $(CFLAGS) $(OBJFLAG) $^

structs.o: structs.c parsing_proc.c structs.h
	$(CC) $(CFLAGS) $(OBJFLAG) structs.c parsing_proc.c

parsing_proc.o: parsing_proc.c structs.c parsing_proc.h
	$(CC) $(CFLAGS) $(OBJFLAG) parsing_proc.c structs.c

outputs.o: outputs.c structs.c outputs.h
	$(CC) $(CFLAGS) $(OBJFLAG) outputs.c structs.c

.PHONY: clean all

clean:
	rm *.o 

help: 
	@echo: "clean: cleans all the .o files"
	@echo: "all: compile code with debugging options to create executable FD_table"
