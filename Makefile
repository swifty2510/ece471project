CC = gcc
CFLAGS = -O2 -Wall
LFLAGS = -lfftw3 -lm

all:	visualizer clean

visualizer:	visualizer.o devices.o
	$(CC) $(LFLAGS) -o visualizer visualizer.o devices.o

visualizer.o:	visualizer.c
	$(CC) $(CFLAGS) -c visualizer.c
devices.o:	devices.c
	$(CC) $(CFLAGS) -c devices.c
clean:
	rm -f *~ *.o
