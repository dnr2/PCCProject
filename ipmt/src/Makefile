FLAGS=-std=c++0x -Wall -Wextra -O2

all: makedir ipmt clean

ipmt: ipmt.o ipmt_algorithm.o SuffixTree.o SuffixArray.o
	g++ $(FLAGS) ipmt.o ipmt_algorithm.o SuffixTree.o SuffixArray.o -o ../bin/ipmt

ipmt.o: ipmt.cpp
	g++ $(FLAGS) -c ipmt.cpp

ipmt_algorithm.o: ipmt_algorithm.cpp
	g++ $(FLAGS) -c ipmt_algorithm.cpp

SuffixTree.o: SuffixTree.cpp
	g++ $(FLAGS) -c SuffixTree.cpp

SuffixArray.o: SuffixArray.cpp
	g++ $(FLAGS) -c SuffixArray.cpp	
	
clean:
	rm -rf *o ipmt

makedir:
	mkdir -p ../bin/
