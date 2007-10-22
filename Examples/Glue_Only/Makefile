# Build RL-Glue so that each part will be communicating via network communication

RL_GLUE_PATH = ../../RL-Glue
BUILD_PATH = ./Build
BIN_PATH = .

AGENT_NETWORKED=1
ENV_NETWORKED=1
EXP_NETWORKED=1

CFLAGS  = -I$(RL_GLUE_PATH)

all: $(BIN_PATH)/RL_glue

include $(RL_GLUE_PATH)/RL_glue.makefile

clean:
	rm -rf RL_glue $(BUILD_PATH)/*