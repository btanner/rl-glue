# Build RL-Glue so that each part will be communicating via network communication

AGENT = RandomAgent.c
AGENT_OBJECTS = $(basename $(AGENT)).o Glue_utilities.o

AGENT_SRC_PATH = ../Agents

RL_GLUE_PATH = ../../RL-Glue
UTILS_PATH = ../../Utils
BUILD_PATH = ./Build
BIN_PATH = .

AGENT_NETWORKED=0
ENV_NETWORKED=1
EXP_NETWORKED=1

ifeq ($(suffix $(AGENT)),.cpp)
	CC = $(CXX)
endif

CFLAGS  = -I$(RL_GLUE_PATH) -I$(UTILS_PATH) -I$(AGENT_SRC_PATH)
LDFLAGS = -lm

all: $(BIN_PATH)/RL_glue

include $(RL_GLUE_PATH)/RL_glue.makefile

$(BUILD_PATH)/$(basename $(AGENT)).o: $(AGENT_SRC_PATH)/$(AGENT)
	$(CC) -c $(CFLAGS) $^ -o $@

clean:
	rm -rf RL_glue $(BUILD_PATH)/*