RL-GLUE = ../../RL-Glue
BUILD_PATH = Build
BIN_PATH = bin
SRC_PATH = src

CC      = gcc  
CFLAGS  = -I$(RL-GLUE)/ -ansi -pedantic -Wall
LDFLAGS =

ENVIRONMENT_OBJECTS = mines.o RL_client_environment.o RL_network.o

RL_environment: $(ENVIRONMENT_OBJECTS)
	$(CC) -o $(BIN_PATH)/$@ $(addprefix $(BUILD_PATH)/, $(ENVIRONMENT_OBJECTS))
	
mines.o: $(SRC_PATH)/mines.c $(SRC_PATH)/mines.h
	$(CC) -c $(CFLAGS) $< -o $(BUILD_PATH)/$@

RL_client_environment.o: $(RL-GLUE)/Network/Environment/RL_client_environment.c
	$(CC) -c $(CFLAGS) $< -o $(BUILD_PATH)/$@
	
RL_network.o: $(RL-GLUE)/Network/RL_network.c
	$(CC) -c $(CFLAGS) $< -o $(BUILD_PATH)/$@


