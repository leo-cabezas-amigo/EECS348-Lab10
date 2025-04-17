CC = g++
CFLAGS =  # No CFLAGS by default.

myprogram: main.o doublestr.o
	$(CC) $(CFLAGS) main.o doublestr.o -o main

main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp

doublestr.o: doublestr.cpp
	$(CC) $(CFLAGS) -c doublestr.cpp

clean:
	rm -f main *.o