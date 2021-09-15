##VARIABLES
CC = gcc


##COMMANDS
all : utils.o

utils.o : utils.c
	$(CC) -O3 -o ../../obj/$@ -c $<

.PHONY : clean all

clean : 
	rm *.o