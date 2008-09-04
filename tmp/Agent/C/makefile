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
RL_GLUE_HEADERS = $(RL_GLUE_PATH)/include
RL_GLUE_BUILD = $(RL_GLUE_PATH)/build/C
CFLAGS = -I$(RL_GLUE_HEADERS)  -I$(AGENT_SRC_PATH)



ifeq ($(suffix $(AGENT)),.cpp)
CC = $(CXX)
RL_GLUE_BUILD = $(RL_GLUE_PATH)/build/CPP
endif

ifeq ($(CC), $(CXX))
RL_GLUE_BUILD = $(RL_GLUE_PATH)/build/CPP
endif


RL_GLUE_CLIENT_OBJECTS = RL_client_agent.o RL_network.o C_TaskSpec_Parser.o

all: RL_agent

RL_agent: $(addprefix $(BUILD_PATH)/, $(AGENT_OBJECTS))  
	$(CC)   $(addprefix $(RL_GLUE_BUILD)/, $(RL_GLUE_CLIENT_OBJECTS)) -o RL_agent $^ -lm    

$(BUILD_PATH)/$(basename $(AGENT)).o: $(AGENT_SRC_PATH)/$(AGENT)
	$(CC) -c $(CFLAGS) $^ -o $@ 

clean:
	rm -rf RL_agent RL_agent.exe $(BUILD_PATH)/*









