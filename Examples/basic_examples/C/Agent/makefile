
AGENT = RandomAgent.c
AGENT_OBJECTS = $(basename $(AGENT)).o Glue_utilities.o

AGENT_SRC_PATH = ../../../Agents
BUILD_PATH = ./Build
RL_GLUE_BASE = ../../../..
RL_GLUE_PATH = $(RL_GLUE_BASE)/RL-Glue
UTILS_PATH = $(RL_GLUE_BASE)/Utils

CFLAGS = -I$(RL_GLUE_PATH) -I$(UTILS_PATH) -I$(AGENT_SRC_PATH)

ifeq ($(suffix $(AGENT)),.cpp)
	CC = $(CXX)
endif

all: RL_agent

include $(RL_GLUE_PATH)/RL_glue.makefile

AGENT_OBJECTS += RL_client_agent.o RL_network.o

RL_agent: $(addprefix $(BUILD_PATH)/, $(AGENT_OBJECTS))
	$(CC) -o RL_agent $^ -lm

$(BUILD_PATH)/$(basename $(AGENT)).o: $(AGENT_SRC_PATH)/$(AGENT)
	$(CC) -c $(CFLAGS) $^ -o $@

clean:
	rm -rf RL_agent $(BUILD_PATH)/*









