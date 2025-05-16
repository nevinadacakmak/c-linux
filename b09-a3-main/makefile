# Compiler and Flags
CC = gcc
CFLAGS = -Wall -Werror
LMFLAG = -lm
OBJFLAG = -c

# Source Files
SRC = calculations.c printing.c main.c signals.c

# Object Files
OBJ = calculations.o printing.o main.o signals.o

all: myMonitoringTool

myMonitoringTool: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LMFLAG)

main.o: $(SRC)
	$(CC) $(CFLAGS) $(OBJFLAG) $^ $(LMFLAG)

calculations.o: calculations.h calculations.c main.c
	$(CC) $(CFLAGS) $(OBJFLAG) calculations.c main.c $(LMFLAG)

printing.o: printing.h printing.c main.c
	$(CC) $(CFLAGS) $(OBJFLAG) printing.c main.c $(LMFLAG)

.PHONY: clean all

clean:
	rm *.o 

help: 
	@echo: "clean: cleans all the .o files"
	@echo: "all: compile code with debugging options to create executable myMonitoringTool"