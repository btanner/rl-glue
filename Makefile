.PHONY: all rl-glue examples clean
BIN_PATH = ./RL-Glue/bin
RL_GLUE_PATH = ./RL-Glue
BUILD_PATH = ./RL-Glue/Build

AGENT_NETWORKED=1
ENV_NETWORKED=1
EXP_NETWORKED=1

CFLAGS = -I$(RL_GLUE_PATH) -std=c99 -O2

all: rl-glue examples
	@echo "*****************************************************************"
	@echo ""
	@echo "RL_glue executable has been built in ./RL-Glue/bin/. Examples have been built in ./Examples. See README.txt for more information."
	@echo ""
	@echo "*****************************************************************"

rl-glue: $(BIN_PATH)/RL_glue

examples:
	cd ./Examples/basic_examples/C/Agent && make
	cd ./Examples/basic_examples/C/Environment && make
	cd ./Examples/basic_examples/C/Experiment && make
	cd ./Examples/basic_examples/Java/Agent && make
	cd ./Examples/basic_examples/Java/Environment && make
	cd ./Examples/basic_examples/Java/Experiment && make
	cd ./Examples/basic_examples/Python/Agent && make
	cd ./Examples/basic_examples/Python/Environment && make
	cd ./Examples/basic_examples/Python/Experiment && make
	cd ./Examples/Glue_Only && make
	cd ./Examples/Glue_plus_Agent && make
	cd ./Examples/Glue_plus_Env && make
	cd ./Examples/Glue_plus_Agent_Env && make
	cd ./Examples/Glue_plus_all && make


clean:
	-rm $(BUILD_PATH)/*
	-rm $(BUILD_PATH)_cpp/*
	-rm $(BIN_PATH)/RL_glue
	-rm $(RL_GLUE_PATH)/Python/rlglue/*.pyc
	-rm $(RL_GLUE_PATH)/Python/rlglue/agent/*.pyc
	-rm $(RL_GLUE_PATH)/Python/rlglue/environment/*.pyc
	-rm $(RL_GLUE_PATH)/Python/rlglue/network/*.pyc
	cd ./Examples/basic_examples/C/Agent && make clean
	cd ./Examples/basic_examples/C/Environment && make clean
	cd ./Examples/basic_examples/C/Experiment && make clean
	cd ./Examples/basic_examples/Java/Agent && make clean
	cd ./Examples/basic_examples/Java/Environment && make clean
	cd ./Examples/basic_examples/Java/Experiment && make clean
	cd ./Examples/basic_examples/Python/Agent && make clean
	cd ./Examples/basic_examples/Python/Environment && make clean
	cd ./Examples/basic_examples/Python/Experiment && make clean
	cd ./Examples/Glue_Only && make clean
	cd ./Examples/Glue_plus_Agent && make clean
	cd ./Examples/Glue_plus_Env && make clean
	cd ./Examples/Glue_plus_Agent_Env && make clean
	cd ./Examples/Glue_plus_all && make clean

include $(RL_GLUE_PATH)/RL_glue.makefile
