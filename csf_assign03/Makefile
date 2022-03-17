#
# Makefile for CSF assignment 3
#

CPPFLAGS = -g -c -Wall -Wextra -pedantic -std=c++11

%.o : %.cpp
	g++ $(CPPFLAGS) $*.cpp -o $*.o

all : csim

csim : main.o
	g++ -o $@ main.o

main.o : main.cpp
	g++ $(CPPFLAGS) main.cpp

.PHONY: solution.zip
solution.zip :
	rm -f solution.zip
	zip -9r $@ Makefile README.txt *h *cpp

clean:
	rm -f *.o csim solution.zip