##VARIABLES
CC = gcc


INCLUDE = ../../include
OBJ_DIR = ../../obj

##COMMANDS

.PHONY : evolution statistics

all : evolution

evolution : simple-interface.o statistics

statistics : redcode-switch.o interface.o util.o 

interface.o : interface.c $(INCLUDE)/utils.h $(INCLUDE)/pmars.h interface.h $(INCLUDE)/exhaust.h
	$(CC) -O3 -o $(OBJ_DIR)/$@ -c $<

redcode-switch.o : redcode-switch.c interface.h util.h $(INCLUDE)/utils.h $(INCLUDE)/pmars.h 
	$(CC) -O3 -o $(OBJ_DIR)/$@ -c $< 

util.o : util.c interface.h $(INCLUDE)/utils.h $(INCLUDE)/exhaust.h 
	$(CC) -O3 -o $(OBJ_DIR)/$@ -c $< 

simple-interface.o : simple-interface.c interface.h util.h $(INCLUDE)/utils.h $(INCLUDE)/pmars.h $(INCLUDE)/exhaust.h $(INCLUDE)/pspace.h $(INCLUDE)/sim.h
	$(CC) -O3 -fopenmp -O3 -o $(OBJ_DIR)/$@ -c $< 