CXX_FLAGS=-O3 -shared -march=native -mtune=native -fPIC

libPosition.so: libPosition.C
	g++ $(CXX_FLAGS) $< -o $@

.PHONY: clean
clean:
	rm libPosition.so

