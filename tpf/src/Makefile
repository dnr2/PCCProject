FLAGS=-std=c++11 -Wall -Wextra -O2

all: makedir tpf clean

tpf: tpf.o tpf_algorithm.o
	g++ $(FLAGS) tpf.o tpf_algorithm.o -o ../bin/tpf

tpf.o: tpf.cpp
	g++ $(FLAGS) -c tpf.cpp

tpf_algorithm.o: tpf_algorithm.cpp
	g++ $(FLAGS) -c tpf_algorithm.cpp

clean:
	rm -rf *o tpf

makedir:
	mkdir -p ../bin/
