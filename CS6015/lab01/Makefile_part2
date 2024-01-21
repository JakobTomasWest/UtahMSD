#Makefile - use build for an executable that takes your codes c++ code files and headers to create operation files
#run is a target that will execute the build - shorthand
#all is a target that you can allows you to make executable by linking the operations
#build is a target use my make to use the operation file to create an executable
#caps.o,clothing.o,main.o,pants.o are all targets that create object files from their associated prerequisites / commands
#clean is a target that remove the executable
#CFLAGS - Compiler
#CCSOURCE macro for cpp
#HEADERS macro for headers
#OBJSOURCE macro for object files
#printM is a target for enuring that my macros work
#create/removeDoucment targets are for creating a clean file and removing that clean file so that make clean still functions properly
CXX=c++
CFLAGS=-std=c++11
CCSOURCE = Caps.cpp Clothing.cpp main.cpp Pants.cpp
HEADERS = Caps.h Clothing.h Pants.h
OBJSOURCE = Caps.o Clothing.o main.o Pants.o

run:
#command to execute / prereq for run target
	./buildpt2
#the following line is just general syntax so that we can use make build in terminal
all: buildpt2
buildpt2: Caps.o Clothing.o main.o Pants.o
#implicit rule will build the targets and commands that our macro overviews
	c++ ${OBJSOURCE} -o buildpt2

#will clean build executable
clean:
	rm -f *.o*. buildpt2
	
printM:
#${} means dynamic calling from variable associated
	echo "${CXX}, ${CFLAGS}, ${CCSOURCE}, ${HEADERS}"

#if there is a file name as the target make <target> (i.e.,e clean) will not call the target but will instead check the file for if it has updated,etc

createDocument:
	touch clean
removeDocument:
	rm -f clean
#when you edit a source file, clean, and make build again it'll run the edited c++ file first and your -o build line after

#if you change pants.cpp pants.o will be recompiled so because Pant.o is associated with the other build files, they will be updated/recompliled as well for execution -compliation command and linking command
