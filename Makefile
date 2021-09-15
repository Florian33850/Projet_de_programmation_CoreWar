##VARIABLES


EXEC = evolution
OBJ = obj
BIN = bin
##COMMANDS

all : $(EXEC)


$(EXEC) : analyser evolution-algorithm exmars mars-interface utils json
	gcc -fopenmp -O3 -o src/evolution-algorithm/$@ $(OBJ)/*.o 

.PHONY : clean mrproper setup setaway analyser evolution-algorithm exmars mars-interface utils json 

analyser : 
	cd src/analyser && $(MAKE)

evolution-algorithm :
	cd src/evolution-algorithm && $(MAKE)

exmars : 
	cd src/exmars && $(MAKE)

mars-interface : 
	cd src/mars-interface && $(MAKE)

utils : 
	cd src/utils && $(MAKE)

json : 
	cd lib/cJSON && $(MAKE)
	cp lib/cJSON/cJSON.o obj

setup : 
	mkdir obj
	mkdir bin
	mkdir include
	cp src/analyser/*.h include
	cp src/evolution-algorithm/*.h include
	cp src/exmars/*.h include
	cp src/mars-interface/*.h include 
	cp src/utils/*.h include
	cp lib/cJSON/cJSON.h include

clean :
	rm obj/*.o
	cd lib/cJSON && $(MAKE) clean

mrproper : clean 
	rm src/evolution-algorithm/evolution

setaway : 
	cd lib/cJSON && $(MAKE) clean
	rm src/evolution-algorithm/evolution
	rm -r obj
	rm -r bin 
	rm -r include 