
EXP_OBJECTS = experiment.o

EXP_SRC_PATH = ../../../Experiments
BUILD_PATH = ./Build
RL_GLUE_PATH = ../../../RL-Glue
UTILS_PATH = ../../../Utils

CFLAGS = -I$(RL_GLUE_PATH) -I$(UTILS_PATH) -I$(AGENT_SRC_PATH) -Wall -g

all: RL_experiment

include $(RL_GLUE_PATH)/RL_glue.makefile

RL_experiment: $(addprefix $(BUILD_PATH)/,$(EXP_OBJECTS)) $(BUILD_PATH)/RL_client_experiment.o $(BUILD_PATH)/RL_network.o
	$(CC) -o RL_experiment $^

$(BUILD_PATH)/experiment.o: $(EXP_SRC_PATH)/experiment.c
	$(CC) -c $(CFLAGS) $^ -o $@

clean:
	rm -f $(BUILD_PATH)/*