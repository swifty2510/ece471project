CC = gcc
CFLAGS = -O2 -Wall
LFLAGS =

all:	visualizer clean

visualizer:	visualizer.o
	$(CC) $(LFLAGS) -o visualizer visualizer.o

visualizer.o:	visualizer.c
	$(CC) $(CFLAGS) -c visualizer.c

clean:	
	rm -f *~ *.o visualizer
