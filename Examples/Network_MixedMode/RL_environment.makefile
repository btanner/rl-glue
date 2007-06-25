RL-GLUE = ../../RL-Glue
BUILD_PATH = Build

CC      = gcc  
CFLAGS  = -I$(RL-GLUE)/ -ansi -pedantic -Wall
LDFLAGS =

OBJECTS = mines.o RL_client_environment.o RL_network_environment.o

RL_environment: $(OBJECTS)
	$(CC) -o $@ $(addprefix $(BUILD_PATH)/, $(OBJECTS))
	
mines.o: ./src/mines.c ./src/mines.h
	$(CC) -c $(CFLAGS) $< -o $(BUILD_PATH)/$@

RL_client_environment.o: $(RL-GLUE)/Network/Environment/RL_client_environment.c
	$(CC) -c $(CFLAGS) $< -o $(BUILD_PATH)/$@
	
RL_network_environment.o: $(RL-GLUE)/Network/RL_network.c
	$(CC) -c $(CFLAGS) $< -o $(BUILD_PATH)/$@


