# Build RL-Glue so that each part will be communicating via network communication

RL-GLUE = ../../RL-Glue
BUILD_PATH = Build
BIN_PATH = ../../../libraries

CC      = gcc
CFLAGS  = -I$(RL-GLUE)/ -ansi -pedantic -Wall
LDFLAGS =

GLUE_OBJECTS = RL_glue.o RL_network.o RL_server_experiment.o RL_server_agent.o RL_server_environment.o RL_network_experiment.o RL_network_agent.o RL_network_environment.o RL_network_glue.o 

#clean up

all: RL_glue

clean: 
	rm -Rf $(BUILD_PATH)/*
	rm -f $(BIN_PATH)/RL_glue

# Link everything together
RL_glue: $(GLUE_OBJECTS)
	$(CC) -o $(BIN_PATH)/$@ $(addprefix $(BUILD_PATH)/, $(GLUE_OBJECTS))

# Build the main interface. 
RL_glue.o: $(RL-GLUE)/RL_glue.c
	$(CC) $(CFLAGS) -c $< -o $(BUILD_PATH)/$@

# Build the network stuff
RL_network.o: $(RL-GLUE)/Network/RL_network.c
	$(CC) $(CFLAGS) -c $< -o $(BUILD_PATH)/$@

RL_server_experiment.o: $(RL-GLUE)/Network/Glue/RL_server_experiment.c
	$(CC) $(CFLAGS) -c $< -o $(BUILD_PATH)/$@

RL_server_agent.o: $(RL-GLUE)/Network/Glue/RL_server_agent.c 
	$(CC) $(CFLAGS) -c $< -o $(BUILD_PATH)/$@

RL_server_environment.o: $(RL-GLUE)/Network/Glue/RL_server_environment.c 
	$(CC) $(CFLAGS) -c $< -o $(BUILD_PATH)/$@

RL_network_experiment.o: $(RL-GLUE)/Network/Glue/RL_network_experiment.c
	$(CC) $(CFLAGS) -c $< -o $(BUILD_PATH)/$@

RL_network_agent.o: $(RL-GLUE)/Network/Glue/RL_network_agent.c
	$(CC) $(CFLAGS) -c $< -o $(BUILD_PATH)/$@

RL_network_environment.o: $(RL-GLUE)/Network/Glue/RL_network_environment.c
	$(CC) $(CFLAGS) -c $< -o $(BUILD_PATH)/$@

RL_network_glue.o: $(RL-GLUE)/Network/Glue/RL_network_glue.c
	$(CC) $(CFLAGS) -c $< -o $(BUILD_PATH)/$@



