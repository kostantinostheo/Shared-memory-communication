CC=g++

CFLAGS=-c 
LFLAGS=-pthread -lrt

all: feeder

feeder: feeder.o my_methods.o shared_memory.o child.o
	$(CC) $(LFLAGS) -o feeder feeder.cpp my_methods.cpp shared_memory.cpp child.cpp

feeder.o: feeder.cpp
	$(CC) $(CFLAGS) $(LFLAGS) feeder.cpp

my_methods.o: my_methods.cpp
	$(CC) $(CFLAGS) $(LFLAGS) my_methods.cpp

shared_memory.o: shared_memory.cpp
	$(CC) $(CFLAGS) $(LFLAGS) shared_memory.cpp

child.o: child.cpp
	$(CC) $(CFLAGS) $(LFLAGS) child.cpp


clean:
	$(info Cleaning everything please stand by...)
	@rm -rf *o feeder
	@rm -rf *o FinalProcessStats.txt
	@sleep 2
	clear

	