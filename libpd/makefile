CXX_FLAGS=-Wall -O3 -ffast-math -ftree-vectorize -fstrict-aliasing -std=c++11 -shared -march=native -mtune=native -fPIC

libPositionDecomp.so: cpp/flat_shapes.o cpp/calculator.o cpp/detector.o cpp/wrappers.o cpp/shell_shapes.o
	g++ $(CXX_FLAGS) $^ -o $@

cpp/flat_shapes.o: cpp/flat_shapes.cpp cpp/flat_shapes.h cpp/shape.h
	g++ -c $(CXX_FLAGS) $< -o $@

cpp/shell_shapes.o: cpp/shell_shapes.cpp cpp/shell_shapes.h
	g++ -c $(CXX_FLAGS) $< -o $@

cpp/detector.o: cpp/detector.cpp cpp/detector.h
	g++ -c $(CXX_FLAGS) $< -o $@

cpp/calculator.o: cpp/calculator.cpp cpp/calculator.h cpp/shape.h cpp/detector.h
	g++ -c $(CXX_FLAGS) $< -o $@

cpp/wrappers.o: cpp/wrappers.cpp cpp/wrappers.h cpp/shape.h cpp/calculator.h cpp/detector.h
	g++ -c $(CXX_FLAGS) $< -o $@

.PHONY: clean
clean:
	-rm cpp/flat_shapes.o cpp/calculator.o cpp/detector.o cpp/wrappers.o

.PHONY: cleanall
cleanall: clean
	-rm libPositionDecomp.so

