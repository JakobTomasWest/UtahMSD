#CFLAGS - Compiler
#CCSOURCE macro for cpp
#HEADERS macro for headers
#OBJSOURCE macro for object files
CXX=c++
CFLAGS=-std=c++17
CCSOURCE = cmdline.cpp TESTS.cpp main.cpp parse.cpp expr.cpp
HEADERS = cmdline.hpp TESTS.h expr.hpp catch.hpp parse.hpp
OBJSOURCE = cmdline.o TESTS.o main.o parse.o expr.o

compile:
#-o take all object files and make an executable file- if
# object file create it from teh cpp files --> implicit rule
	c++ -std=c++17 *.cpp -o build
#target : prequsites
#this Object file is the binary file
object: cmdline.hpp TESTS.h expr.hpp parse.hpp main.cpp catch.hpp
#compile -c will give object files?
	c++ -c -std=c++17 main.cpp expr.cpp cmdline.cpp parse.cpp TESTS.cpp

library: object
#create the library.a file from object files
	ar -ruv libMSDScript.a cmdline.o expr.o TESTS.o parse.o main.o

buildLib:
# Directory to store the library
	c++ -std=c++17 main.cpp -L../Library_MSDScript -lMSDScript -o myExec
	
.PHONY: build
test: build compile
	./build --test

doc: msdscript
		cd Documentation && Doxygen

#will clean build eecutable
clean:
	rm -rf *.o build
	

# ar -ruv libMSDscript.a *.o
# used to create the library .a file from the object files^
#libmsd.a
