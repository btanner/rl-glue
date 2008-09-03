# -------------------------------------------------
# Select the agent, environment and experiment 
# program you wish to compile with rl-glue (into
# one executable. All must be c-files
# -------------------------------------------------
AGENT = RandomAgent.c
AGENT_SRC_PATH = ../Agent/C/src
ENVIRONMENT = mines.c
ENV_SRC_PATH = ../Environment/C/src
EXPERIMENT = experiment.c
EXP_SRC_PATH = ../Experiment/C/src

RL_GLUE_PATH = ../../RL-Glue
UTILS_PATH = ../../RL-Glue/Utilities
BUILD_PATH = ./Build
BIN_PATH = .
# -------------------------------------------------
 




AGENT_OBJECTS = $(basename $(AGENT)).o C_TaskSpec_Parser.o 
ENV_OBJECTS = $(basename $(ENVIRONMENT)).o
EXP_OBJECTS = $(basename $(EXPERIMENT)).o

AGENT_NETWORKED=0
ENV_NETWORKED=0
EXP_NETWORKED=0

ifeq ($(suffix $(AGENT)),.cpp)
	CC = $(CXX)
endif

ifeq ($(suffix $(ENVIRONMENT)),.cpp)
	CC = $(CXX)
endif

ifeq ($(suffix $(EXPERIMENT)),.cpp)
	CC = $(CXX)
endif

CFLAGS  = -I$(RL_GLUE_PATH) -I$(UTILS_PATH) -I$(ENV_SRC_PATH) -I$(AGENT_SRC_PATH) -I$(EXP_SRC_PATH)
LDFLAGS = -lm

all: $(BIN_PATH)/RL_glue

include $(RL_GLUE_PATH)/Makefile

$(BUILD_PATH)/$(basename $(ENVIRONMENT)).o: $(ENV_SRC_PATH)/$(ENVIRONMENT)
	$(CC) -c $(CFLAGS) $^ -o $@

$(BUILD_PATH)/$(basename $(AGENT)).o: $(AGENT_SRC_PATH)/$(AGENT)
	$(CC) -c $(CFLAGS) $^ -o $@

$(BUILD_PATH)/$(basename $(EXPERIMENT)).o: $(EXP_SRC_PATH)/$(EXPERIMENT)
	$(CC) -c $(CFLAGS) $^ -o $@

clean:
	rm -rf RL_glue RL_glue.exe $(BUILD_PATH)/*
