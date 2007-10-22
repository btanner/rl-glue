# Build RL-Glue so that each part will be communicating via network communication

ENVIRONMENT = mines.c
ENV_OBJECTS = $(basename $(ENVIRONMENT)).o

ENV_SRC_PATH = ../Environments

RL_GLUE_PATH = ../../RL-Glue
BUILD_PATH = ./Build
BIN_PATH = .

AGENT_NETWORKED=1
ENV_NETWORKED=0
EXP_NETWORKED=1

ifeq ($(suffix $(ENVIRONMENT)),.cpp)
	CC = $(CXX)
endif

CFLAGS  = -I$(RL_GLUE_PATH) -I$(ENV_SRC_PATH)

all: $(BIN_PATH)/RL_glue

include $(RL_GLUE_PATH)/RL_glue.makefile

$(BUILD_PATH)/$(basename $(ENVIRONMENT)).o: $(ENV_SRC_PATH)/$(ENVIRONMENT)
	$(CC) -c $(CFLAGS) $^ -o $@

clean:
	rm -rf RL_glue $(BUILD_PATH)/*