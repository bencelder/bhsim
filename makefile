# specify which compiler
CC = gcc

# -c tells it not to link
#  Wall tells it to 
CFlags=-c -Wall

# not sure what this line does
all: sim

# target: dependencies
sim: main.o vec.o
	$(CC) main.o vec.o -o sim

main.o: main.c params.h
	$(CC) $(CFlags) main.c

vec.o: vec.c params.h
	$(CC) $(CFlags) vec.c

clean:
	rm -rf *.o sim
