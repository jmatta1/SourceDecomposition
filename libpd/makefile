CXX_FLAGS=-O2 -g -shared -march=native -mtune=native -fPIC

libPosition.so: cpp/flat_shapes.o cpp/calculator.o cpp/ cpp/wrappers.o
	g++ $(CXX_FLAGS) $^ -o $@

cpp/flat_shapes.o: cpp/flat_shapes.cpp cpp/flat_shapes.h cpp/shape.h
	g++ -c $(CXX_FLAGS) $< -o $@

cpp/calculator.o: cpp/calculator.cpp cpp/calculator.h cpp/shapes.h cpp/detector.h
	g++ -c $(CXX_FLAGS) $< -o $@

cpp/detector.o: cpp/detector.cpp cpp/detector.h
	g++ -c $(CXX_FLAGS) $< -o $@

cpp/wrappers.o: cpp/wrappers.cpp cpp/wrappers.h cpp/shapes.h cpp/calculator.h cpp/detector.h
	g++ -c $(CXX_FLAGS) $< -o $@

.PHONY: clean
cleanall:
	rm cpp/shapes.o cpp/calculator.o cpp/wrappers.o

.PHONY: cleanall
cleanall:
	rm cpp/shapes.o cpp/calculator.o cpp/wrappers.o libPosition.so

