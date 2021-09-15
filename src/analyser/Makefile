##VARIABLES
CC = gcc

INCLUDE = ../../include
OBJ_DIR = ../../obj


##COMMANDS
all : analyser.o generator.o


analyser.o : analyser.c $(INCLUDE)/*.h ../../lib/cJSON/cJSON.h
	$(CC) -O3 -o $(OBJ_DIR)/$@ -c $< 

generator.o : generator.c $(INCLUDE)/*.h ../../lib/cJSON/cJSON.h
	$(CC) -O3 -o $(OBJ_DIR)/$@ -c $< 

