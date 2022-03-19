#
# Makefile for CSF assignment 3
#

CPPFLAGS = -g -O2 -Wall -Wextra -pedantic

%.o : %.cpp
	g++ $(CPPFLAGS) -c $*.cpp -o $*.o

all : csim

csim : main.o set.o
	g++ -o $@ main.o set.o

set.o: set.cpp set.h slot.h

main.o : main.cpp set.h



.PHONY: solution.zip
solution.zip :
	rm -f solution.zip
	zip -9r $@ Makefile README.txt *h *cpp

clean:
	rm -f *.o csim solution.zip