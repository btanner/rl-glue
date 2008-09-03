.PHONY: all rl-glue examples clean
BIN_PATH = ./RL-Glue/bin
RL_GLUE_PATH = ./RL-Glue
BUILD_BASE_PATH = ./RL-Glue/build
BUILD_C_PATH = $(BUILD_BASE_PATH)/C
BUILD_CPP_PATH = $(BUILD_BASE_PATH)/CPP
RL_GLUE_HEADERS = $(RL_GLUE_PATH)/include
AGENT_NETWORKED=1
ENV_NETWORKED=1
EXP_NETWORKED=1

CFLAGS = -I$(RL_GLUE_HEADERS) -std=c99
LDFLAGS = -lm

all: rl-glue examples
	@echo "*****************************************************************"
	@echo ""
	@echo "RL_glue executable has been built in ./RL-Glue/bin/. Examples have been built in ./Examples. See README.txt for more information."
	@echo ""
	@echo "*****************************************************************"

rl-glue: $(BIN_PATH)/RL_glue

examples:
	cd ./Examples && make



clean:
	-rm $(BUILD_C_PATH)/*
	-rm $(BUILD_CPP_PATH)/*
	-rm $(BIN_PATH)/RL_glue
	cd ./Examples && make clean


include $(RL_GLUE_PATH)/Makefile