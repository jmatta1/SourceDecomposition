ROOT_FLAGS=`root-config --cflags --libs`
#CPP_FLAGS=-O3 -g -march=native -mtune=native -fstrict-aliasing ${ROOT_FLAGS}
CPP_FLAGS= --std=c++11 -g -march=native -mtune=native ${ROOT_FLAGS}

positionDecomp: main.cpp Internals.h
	g++ ${CPP_FLAGS} $< -o $@

.PHONY:clean
clean:
	-rm positionDecomp
