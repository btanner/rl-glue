
AGENT_OBJECTS = SarsaAgent.o Glue_utilities.o

AGENT_SRC_PATH = ../../../Agents
BUILD_PATH = ./Build
RL_GLUE_PATH = ../../../RL-Glue
UTILS_PATH = ../../../Utils

CFLAGS = -I$(RL_GLUE_PATH) -I$(UTILS_PATH) -I$(AGENT_SRC_PATH) -Wall -g

all: RL_agent

include $(RL_GLUE_PATH)/RL_glue.makefile

RL_agent: $(addprefix $(BUILD_PATH)/,$(AGENT_OBJECTS)) $(BUILD_PATH)/RL_client_agent.o $(BUILD_PATH)/RL_network.o
	$(CC) -o RL_agent $^

$(BUILD_PATH)/SarsaAgent.o: $(AGENT_SRC_PATH)/SarsaAgent.c
	$(CC) -c $(CFLAGS) $^ -o $@

clean:
	rm -f $(BUILD_PATH)/*









