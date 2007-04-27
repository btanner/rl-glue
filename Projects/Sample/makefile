CC = gcc
RL-Glue = ../../RL-Glue

# We need to search the RL-Glue directory RL_Interface.h 
CFLAGS = -I$(RL-Glue)/ -ansi -pedantic -Wall

all: Glue Agent Environment Benchmark
	$(CC) -o RL_main Build/RL_Interface.o Build/ZeroAgent.o Build/mines.o Build/main.o

Glue: RL_Interface.o
Agent: ZeroAgent.o
Environment: mines.o
Benchmark: main.o

RL_Interface.o: $(RL-Glue)/RL_Interface.c $(RL-Glue)/RL_Interface.h
	$(CC) -c $(CFLAGS) $< -o Build/RL_Interface.o

ZeroAgent.o: ./Agent/ZeroAgent.c ./Agent/ZeroAgent.h
	$(CC) -c $(CFLAGS) $< -o Build/ZeroAgent.o

mines.o: ./Environment/mines.c ./Environment/mines.h
	$(CC) -c $(CFLAGS) $< -o Build/mines.o

main.o: ./Benchmark/main.c
	$(CC) -c $(CFLAGS) $< -o Build/main.o

clean:
	rm -fr Build/*.o
	rm -fr RL_main

tidy:
	rm -fr *.*~
	rm -fr *~
	rm -fr \#*
