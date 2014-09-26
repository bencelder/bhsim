# specify which compiler
CC = gcc

# -c tells it not to link
#  Wall tells it to output warnings
# -O3 optimizes
#  -S outputs assembly code in .s files
CFlags=-c -Wall

# not sure what this line does
#all: sim

# target: dependencies
sim: main.o vec.o bhtree.o
	$(CC) main.o vec.o bhtree.o -o sim

main.o: main.c params.h bhtree.h main.h
	$(CC) $(CFlags) main.c

vec.o: vec.c params.h
	$(CC) $(CFlags) vec.c

bhtree.o: bhtree.c bhtree.h main.h
	$(CC) $(CFlags) bhtree.c

clean:
	rm -rf *.o sim
	rm data/*.dat
	rm images/*.png
