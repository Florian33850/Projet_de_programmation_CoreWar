##VARIABLES

CC = gcc
INCLUDE = ../../include
EXEC = evolution
BIN_DIR = ../../bin
OBJ_DIR = ../../obj
##COMMANDS
all : main.o scoring.o evolution-algorithm.o evolution-parameters.o 

main.o : main.c $(INCLUDE)/*.h ../../lib/cJSON/cJSON.h
	$(CC) -O3 -fopenmp -O3 -o $(OBJ_DIR)/$@ -c $< 

evolution-parameters.o : evolution-parameters.c $(INCLUDE)/*.h ../../lib/cJSON/cJSON.h 
	$(CC) -O3 -o $(OBJ_DIR)/$@ -c $< 

evolution-algorithm.o : evolution-algorithm.c $(INCLUDE)/*.h 
	$(CC) -O3 -o $(OBJ_DIR)/$@ -c $< 

scoring.o : scoring.c $(INCLUDE)/*.h ../../lib/cJSON/cJSON.h 
	$(CC) -O3 -o $(OBJ_DIR)/$@ -c $< 
