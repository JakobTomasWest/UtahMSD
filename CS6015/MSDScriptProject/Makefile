CXX=c++
CFLAGS=-fsanitize=undefined -fno-sanitize-recover=undefined
CCSOURCE = cmdline.cpp TESTS.cpp main.cpp parse.cpp expr.cpp val.cpp env.cpp
HEADERS = cmdline.hpp TESTS.h expr.hpp catch.hpp parse.hpp Val.h pointer.hpp env.hpp
OBJSOURCE = cmdline.o TESTS.o main.o parse.o expr.o val.o

compile: object
	c++ -std=c++17 $(CCSOURCE) -o build

#target : prequsites
#this Object file is the binary file
object: cmdline.hpp TESTS.h expr.hpp parse.hpp main.cpp catch.hpp Val.h pointer.hpp env.hpp
#compile -c will give object files?
	c++ -c -std=c++17 -fsanitize=address  main.cpp expr.cpp cmdline.cpp parse.cpp TESTS.cpp val.cpp env.cpp

buildS: expr.cpp expr.hpp main.cpp cmdline.hpp cmdline.cpp catch.hpp  parse.cpp val.cpp Val.h pointer.hpp env.hpp
	g++ -std=c++11 -fsanitize=address -fno-sanitize-recover=undefined main.cpp expr.cpp cmdline.cpp parse.cpp val.cpp env.cpp -o msdscriptS

.PHONY: build
test:
	./build --test

run: build
	./build --print
	./build --pretty-print
	./build --interp

doc: msdscript
		cd Documentation && Doxygen
# the -o means make an exectuable file with the test_msdScript.cpp and exec.cpp
# usually make will just run teh first target and will create out compile target
# which compiles my programs .cpp files
# now in order to compile the test_msdscript we have to specifically make it
# make test_msdscript which will compile basically running c++ test_msdscript.cpp exec.cpp -o test_msdscript
#target          prerequistes
test_msdscript: test_msdscript.cpp exec.cpp exec.h
	c++ test_msdscript.cpp exec.cpp -o test_msdscript

#will clean build eecutable
clean:
	rm -rf *.o build
	


#c++ -std=c++17 -fsanitize=undefined -fno-sanitize-recover=undefined cmdline.cpp TESTS.cpp main.cpp parse.cpp expr.cpp val.cpp -o build
#c++ -std=c++17 -fsanitize=undefined cmdline.cpp TESTS.cpp main.cpp parse.cpp expr.cpp val.cpp -o build