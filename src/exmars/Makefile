##VARIABLES
CC = gcc

HEADERS = exhaust.h sim.h pmars.h insn.h insn_help.h pspace.h 
OBJ_DIR = ../../obj

##COMMANDS
all : sim.o pspace.o pmars.o

sim.o : sim.c $(HEADERS)
	$(CC) -O3 -o $(OBJ_DIR)/$@ -c $< 

pspace.o : pspace.c $(HEADERS)
	$(CC) -O3 -o $(OBJ_DIR)/$@ -c $< 

pmars.o : pmars.c $(HEADERS)
	$(CC) -O3 -o $(OBJ_DIR)/$@ -c $< 
