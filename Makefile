CC = gcc
FLAGS = -c

all: copy

main: copy.cpp
	$(CC) $(CFLAGS) copy.cpp $(CPP) -o copy

clean:
	rm -f copy

