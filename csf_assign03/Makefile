#
# Makefile for CSF assignment 3
#

CPPFLAGS = -g -c -Wall -Wextra -pedantic -std=c++11

%.o : %.cpp
	g++ $(CPPFLAGS) $*.cpp -o $*.o

all : cache_simulator

cache_simulator : main.o
	g++ -o $@ main.o

main.o : main.cpp
	g++ $(CPPFLAGS) main.cpp

.PHONY: solutio.zip
solution.zip :
	rm -f solution.zip
	zip -9r $@ Makefile README.txt *h *cpp

clean:
	rm -f *.o cache_simulator solution.zip