RL-GLUE = ../../RL-Glue
UTILS   = ../../Utils

BUILD_PATH = Build
BIN_PATH = bin
SRC_PATH = src

CC      = gcc
CFLAGS  = -I$(RL-GLUE)/ -ansi -pedantic -Wall
LDFLAGS =

AGENT_OBJECTS = SarsaAgent.o Glue_utilities.o RL_client_agent.o RL_network_agent.o

RL_agent: $(AGENT_OBJECTS)
	$(CC) -o $(BIN_PATH)/$@ $(addprefix $(BUILD_PATH)/, $(AGENT_OBJECTS))
	
SarsaAgent.o: $(SRC_PATH)/SarsaAgent.c $(SRC_PATH)/SarsaAgent.h
	$(CC) -c $(CFLAGS) -I$(UTILS) $< -o $(BUILD_PATH)/$@

Glue_utilities.o: $(UTILS)/Glue_utilities.c $(UTILS)/Glue_utilities.h
	$(CC) -c $(CFLAGS) $< -o $(BUILD_PATH)/$@

RL_client_agent.o: $(RL-GLUE)/Network/Agent/RL_client_agent.c
	$(CC) -c $(CFLAGS) $< -o $(BUILD_PATH)/$@
	
RL_network_agent.o: $(RL-GLUE)/Network/RL_network.c
	$(CC) -c $(CFLAGS) $< -o $(BUILD_PATH)/$@
