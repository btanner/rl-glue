
ENVIRONMENT = mines.c
ENV_OBJECTS = $(basename $(ENVIRONMENT)).o

ENV_SRC_PATH = ../../../Environments
BUILD_PATH = ./Build
RL_GLUE_BASE = ../../../..
RL_GLUE_PATH = $(RL_GLUE_BASE)/RL-Glue
UTILS_PATH = $(RL_GLUE_BASE)/Utils

CFLAGS = -I$(RL_GLUE_PATH) -I$(UTILS_PATH) -I$(ENV_SRC_PATH)

ifeq ($(suffix $(ENVIRONMENT)),.cpp)
	CC = $(CXX)
endif

all: RL_environment

include $(RL_GLUE_PATH)/RL_glue.makefile

ENV_OBJECTS += RL_client_environment.o RL_network.o

RL_environment: $(addprefix $(BUILD_PATH)/, $(ENV_OBJECTS))
	$(CC) -o RL_environment $^

$(BUILD_PATH)/$(basename $(ENVIRONMENT)).o: $(ENV_SRC_PATH)/$(ENVIRONMENT)
	$(CC) -c $(CFLAGS) $^ -o $@

clean:
	rm -rf RL_environment $(BUILD_PATH)/*