# ------------------------------------------------------------------
# Changes these values to use different source files and different 
# directories 
# ------------------------------------------------------------------
AGENT = RandomAgent.c
AGENT_SRC_PATH = ./src
BUILD_PATH = ./Build
RL_GLUE_BASE = ../../..
# ------------------------------------------------------------------

AGENT_OBJECTS = $(basename $(AGENT)).o
RL_GLUE_PATH = $(RL_GLUE_BASE)/RL-Glue

CFLAGS = -I$(RL_GLUE_PATH)  -I$(AGENT_SRC_PATH) -I$(RL_GLUE_PATH)/Utilities 

ifeq ($(suffix $(AGENT)),.cpp)
	CC = $(CXX)
endif

all: RL_agent

include $(RL_GLUE_PATH)/RL_glue.makefile

AGENT_OBJECTS +=  RL_client_agent.o RL_network.o C_TaskSpec_Parser.o

RL_agent: $(addprefix $(BUILD_PATH)/, $(AGENT_OBJECTS)) 
	$(CC)  -o RL_agent $^ -lm    

$(BUILD_PATH)/$(basename $(AGENT)).o: $(AGENT_SRC_PATH)/$(AGENT)
	$(CC) -c $(CFLAGS) $^ -o $@ 

clean:
	rm -rf RL_agent RL_agent.exe $(BUILD_PATH)/*









