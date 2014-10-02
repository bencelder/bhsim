# specify which compiler
# ubuntu 4.9: gcc-4.9
CC = gcc
#CC = gcc-4.9
#CC = /usr/local/bin/gcc

# -c tells it not to link
#  Wall tells it to output warnings
# -O3 optimizes
#  -S outputs assembly code in .s files
CFlags=-c -Wall -O3 -pg

# target: dependencies
#
# not sure what this line does
#all: sim

# the -lm tells it explicitly to link the math library
# (needed on ubuntu)
# -pg profiles the code.  Compile, run, then
#  gprof go -p
sim: main.o vec.o bhtree.o
	$(CC) main.o vec.o bhtree.o -o sim -lm -fopenmp -pg

main.o: main.c params.h bhtree.h main.h
	$(CC) $(CFlags) main.c -fopenmp

vec.o: vec.c params.h
	$(CC) $(CFlags) vec.c

bhtree.o: bhtree.c bhtree.h main.h
	$(CC) $(CFlags) bhtree.c

# fix this to run all commands no matter what
clean:
	rm -rf *.o sim;
	rm data/*.dat;
	rm images/*.png;
	rm *.out;
