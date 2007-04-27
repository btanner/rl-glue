CXX = g++

CFLAGS = -ansi -pedantic -Wall
LDFLAGs= 

all: server client

server: RLnet.o server.o 
	$(CXX) RLnet.o server.o -o server 

client:	RLnet.o client.o
	$(CXX) RLnet.o client.o -o client

server.o: server.cpp
	$(CXX) $(CFLAGS) -c server.cpp

client.o: client.cpp	
	$(CXX) $(CFLAGS) -c client.cpp

RLnet.o: RLnet.c RLnet.h
	$(CXX) $(CFLAGS) -c RLnet.c 

clean: tidy
	rm -fr server
	rm -fr client

tidy:
	rm -fr *.*~
	rm -fr *~
	rm -fr \#*
	rm -fr *.o
