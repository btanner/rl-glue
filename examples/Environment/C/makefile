# ------------------------------------------------------------------
# Changes these values to use different source files and different 
# directories 
# ------------------------------------------------------------------
ENVIRONMENT = mines.c
ENV_SRC_PATH = ./src
BUILD_PATH = ./Build
RL_GLUE_BASE = ../../..
# ------------------------------------------------------------------

ENV_OBJECTS = $(basename $(ENVIRONMENT)).o 

RL_GLUE_PATH = $(RL_GLUE_BASE)/RL-Glue
RL_GLUE_HEADERS = $(RL_GLUE_PATH)/include
RL_GLUE_BUILD = $(RL_GLUE_PATH)/build/C
CFLAGS = -I$(RL_GLUE_HEADERS)  -I$(ENV_SRC_PATH)

ifeq ($(suffix $(ENVIRONMENT)),.cpp)
	CC = $(CXX)
	RL_GLUE_BUILD = $(RL_GLUE_PATH)/build/CPP
endif

ifeq ($(CC), $(CXX))
RL_GLUE_BUILD = $(RL_GLUE_PATH)/build/CPP
endif

all: RL_environment

RL_GLUE_CLIENT_OBJECTS = RL_client_environment.o RL_network.o 

RL_environment: $(addprefix $(BUILD_PATH)/, $(ENV_OBJECTS))
	$(CC)  $(addprefix $(RL_GLUE_BUILD)/, $(RL_GLUE_CLIENT_OBJECTS)) -o RL_environment $^

$(BUILD_PATH)/$(basename $(ENVIRONMENT)).o: $(ENV_SRC_PATH)/$(ENVIRONMENT)
	$(CC) 	-c $(CFLAGS) $^ -o $@

clean:
	rm -rf RL_environment RL_environment.exe $(BUILD_PATH)/*
