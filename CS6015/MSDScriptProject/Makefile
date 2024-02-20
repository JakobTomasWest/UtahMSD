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
	
	
