.PHONY: all rl-glue examples clean
BIN_PATH = ./RL-Glue/bin
RL_GLUE_PATH = ./RL-Glue
BUILD_PATH = ./RL-Glue/Build
  
AGENT_NETWORKED=1
ENV_NETWORKED=1
EXP_NETWORKED=1

CFLAGS = -I$(RL_GLUE_PATH) -std=c99 -O2
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
	-rm $(BUILD_PATH)/*
	-rm $(BUILD_PATH)_cpp/*
	-rm $(BIN_PATH)/RL_glue
	-rm $(RL_GLUE_PATH)/Python/rlglue/*.pyc
	-rm $(RL_GLUE_PATH)/Python/rlglue/agent/*.pyc
	-rm $(RL_GLUE_PATH)/Python/rlglue/environment/*.pyc
	-rm $(RL_GLUE_PATH)/Python/rlglue/network/*.pyc
	cd ./Examples && make clean


include $(RL_GLUE_PATH)/RL_glue.makefile