compile:
	c++ -std=c++17 *.cpp -o build



.PHONY: build
test: build compile
	./build --test

run: build
	./build --print
	./build --pretty-print
	./build --interp

doc: msdscript
		cd Documentation && Doxygen

#will clean build eecutable
clean:
	rm -rf *.o build
	
	
#compile the test_msdscript program
test_msdscript: test_msdscript.cpp
    g++ -o test_msdscript test_msdscript.cpp

#add a custom target for testing
.PHONY: test
test: msdscript test_msdscript
    ./test_msdscript ./msdscript
