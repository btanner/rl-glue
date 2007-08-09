AGENT_RL_GLUE = ../../RL-Glue
AGENT_UTILS   = ../../Utils

AGENT_BUILD_PATH = Build
AGENT_BIN_PATH = bin
AGENT_SRC_PATH = src

AGENT_CC      = gcc
AGENT_CFLAGS  = -g -I$(AGENT_RL_GLUE)/ -ansi -pedantic -Wall
AGENT_LDFLAGS =

AGENT_OBJECTS = SarsaAgent.o Glue_utilities.o RL_client_agent.o RL_agent_netlib.o

RL_agent: $(AGENT_OBJECTS)
	$(AGENT_CC) -o $(AGENT_BIN_PATH)/$@ $(addprefix $(AGENT_BUILD_PATH)/, $(AGENT_OBJECTS)) $(AGENT_LDFLAGS)

SarsaAgent.o: $(AGENT_SRC_PATH)/SarsaAgent.c $(AGENT_SRC_PATH)/SarsaAgent.h
	$(AGENT_CC) -c $(AGENT_CFLAGS) -I$(AGENT_UTILS) $< -o $(AGENT_BUILD_PATH)/$@

Glue_utilities.o: $(AGENT_UTILS)/Glue_utilities.c $(AGENT_UTILS)/Glue_utilities.h
	$(AGENT_CC) -c $(AGENT_CFLAGS) $< -o $(AGENT_BUILD_PATH)/$@

RL_client_agent.o: $(AGENT_RL_GLUE)/Network/Agent/RL_client_agent.c
	$(AGENT_CC) -c $(AGENT_CFLAGS) $< -o $(AGENT_BUILD_PATH)/$@

# Give this a unique name so it doesn't conflict with other RL_network.o rules included by the main makefile 
# Alernatively we could just build it once.
RL_agent_netlib.o: $(AGENT_RL_GLUE)/Network/RL_network.c
	$(AGENT_CC) -c $(AGENT_CFLAGS) $< -o $(AGENT_BUILD_PATH)/$@
