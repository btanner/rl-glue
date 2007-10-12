
ENV_OBJECTS = mines.o

ENV_SRC_PATH = ../../../Environments
BUILD_PATH = ./Build
RL_GLUE_PATH = ../../../RL-Glue
UTILS_PATH = ../../../Utils

CFLAGS = -I$(RL_GLUE_PATH) -I$(UTILS_PATH) -I$(AGENT_SRC_PATH) -Wall -g

all: RL_environment

include $(RL_GLUE_PATH)/RL_glue.makefile

RL_environment: $(addprefix $(BUILD_PATH)/,$(ENV_OBJECTS)) $(BUILD_PATH)/RL_client_environment.o $(BUILD_PATH)/RL_network.o
	$(CC) -o RL_environment $^

$(BUILD_PATH)/mines.o: $(ENV_SRC_PATH)/mines.c
	$(CC) -c $(CFLAGS) $^ -o $@

clean:
	rm -f $(BUILD_PATH)/*