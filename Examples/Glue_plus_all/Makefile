# Build RL-Glue so that each part will be communicating via network communication

AGENT = RandomAgent.c
AGENT_OBJECTS = $(basename $(AGENT)).o Glue_utilities.o

AGENT_SRC_PATH = ../Agents

ENVIRONMENT = mines.c
ENV_OBJECTS = $(basename $(ENVIRONMENT)).o

ENV_SRC_PATH = ../Environments

EXPERIMENT = experiment.c
EXP_OBJECTS = $(basename $(EXPERIMENT)).o

EXP_SRC_PATH = ../Experiments

RL_GLUE_PATH = ../../RL-Glue
UTILS_PATH = ../../Utils
BUILD_PATH = ./Build
BIN_PATH = .

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

include $(RL_GLUE_PATH)/RL_glue.makefile

$(BUILD_PATH)/$(basename $(ENVIRONMENT)).o: $(ENV_SRC_PATH)/$(ENVIRONMENT)
	$(CC) -c $(CFLAGS) $^ -o $@

$(BUILD_PATH)/$(basename $(AGENT)).o: $(AGENT_SRC_PATH)/$(AGENT)
	$(CC) -c $(CFLAGS) $^ -o $@

$(BUILD_PATH)/$(basename $(EXPERIMENT)).o: $(EXP_SRC_PATH)/$(EXPERIMENT)
	$(CC) -c $(CFLAGS) $^ -o $@

clean:
	rm -rf RL_glue $(BUILD_PATH)/*