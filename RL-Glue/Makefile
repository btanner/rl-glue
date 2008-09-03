.PHONY: all clean client
#Important to put RL_GLUE_PATH in front of things in case this file is included in another makefile in a different directory
RL_GLUE_HEADERS= $(RL_GLUE_PATH)/include
VPATH = $(RL_GLUE_PATH):$(RL_GLUE_PATH)/src

NETWORK_OBJECTS=RL_network.o RL_network_glue.o
OBJECTS = RL_glue.o C_TaskSpec_Parser.o
CLIENT_OBJECTS=RL_client_agent.o RL_client_environment.o RL_client_experiment.o


ifndef AGENT_NETWORKED
	AGENT_NETWORKED=1
endif

ifndef ENV_NETWORKED
	ENV_NETWORKED=1
endif

ifndef EXP_NETWORKED
	EXP_NETWORKED=1
endif

ifndef BUILD_BASE_PATH
	BUILD_BASE_PATH = ./build
endif

ifndef BUILD_C_PATH
	BUILD_C_PATH = $(BUILD_BASE_PATH)/C
endif

ifndef BUILD_CPP_PATH
	BUILD_CPP_PATH = $(BUILD_BASE_PATH)/CPP
endif

ifndef BIN_PATH
	BIN_PATH = ./bin
endif

ifndef RL_GLUE_PATH
	RL_GLUE_PATH = .
endif

ifndef CFLAGS
	CFLAGS = -I$(RL_GLUE_HEADERS) -ansi -pedantic -Wall
endif

EXTRA_OBJECTS = 

ifeq ($(AGENT_NETWORKED),1)
	OBJECTS += RL_server_agent.o RL_network_agent.o
else
	OBJECTS += RL_direct_agent.o
	EXTRA_OBJECTS += $(AGENT_OBJECTS)
endif

ifeq ($(ENV_NETWORKED),1)
	OBJECTS += RL_server_environment.o RL_network_environment.o
else
	OBJECTS += RL_direct_environment.o
	EXTRA_OBJECTS += $(ENV_OBJECTS)
endif

ifeq ($(EXP_NETWORKED),1)
	OBJECTS += RL_server_experiment.o RL_network_experiment.o
else
	EXTRA_OBJECTS += $(EXP_OBJECTS)
endif

ifneq ("$(AGENT_NETWORKED)$(ENV_NETWORKED)$(EXP_NETWORKED)","000")
	OBJECTS += $(NETWORK_OBJECTS)
endif

ifeq ($(CC),$(CXX))
	RL_GLUE_OBJECT_PATH = $(BUILD_CPP_PATH)
else
	RL_GLUE_OBJECT_PATH = $(BUILD_C_PATH)
endif


all: client $(BIN_PATH)/RL_glue

client: $(addprefix $(RL_GLUE_OBJECT_PATH)/,$(CLIENT_OBJECTS))


$(BIN_PATH)/RL_glue: $(addprefix $(RL_GLUE_OBJECT_PATH)/,$(OBJECTS)) $(addprefix $(BUILD_C_PATH)/,$(EXTRA_OBJECTS)) 
	$(CC) -o $(BIN_PATH)/RL_glue $^ $(LDFLAGS)

$(BUILD_C_PATH)/RL_glue.o: RL_glue.c
	$(CC) -c $(CFLAGS) $^ -o $@

$(BUILD_C_PATH)/RL_network.o: RL_network.c
	$(CC) -c $(CFLAGS) $^ -o $@

$(BUILD_C_PATH)/RL_network_glue.o: RL_network_glue.c
	$(CC) -c $(CFLAGS) $^ -o $@

$(BUILD_C_PATH)/RL_server_experiment.o: RL_server_experiment.c
	$(CC) -c $(CFLAGS) $^ -o $@

$(BUILD_C_PATH)/RL_network_experiment.o: RL_network_experiment.c
	$(CC) -c $(CFLAGS) $^ -o $@

$(BUILD_C_PATH)/RL_client_experiment.o: RL_client_experiment.c
	$(CC) -c $(CFLAGS) $^ -o $@

$(BUILD_C_PATH)/RL_server_agent.o: RL_server_agent.c
	$(CC) -c $(CFLAGS) $^ -o $@

$(BUILD_C_PATH)/RL_network_agent.o: RL_network_agent.c
	$(CC) -c $(CFLAGS) $^ -o $@

$(BUILD_C_PATH)/RL_client_agent.o: RL_client_agent.c
	$(CC) -c $(CFLAGS) $^ -o $@

$(BUILD_C_PATH)/RL_server_environment.o: RL_server_environment.c
	$(CC) -c $(CFLAGS) $^ -o $@
	
$(BUILD_C_PATH)/RL_network_environment.o: RL_network_environment.c
	$(CC) -c $(CFLAGS) $^ -o $@

$(BUILD_C_PATH)/RL_client_environment.o: RL_client_environment.c
	$(CC) -c $(CFLAGS) $^ -o $@

$(BUILD_C_PATH)/RL_direct_agent.o: RL_direct_agent.c
	$(CC) -c $(CFLAGS) $^ -o $@

$(BUILD_C_PATH)/RL_direct_environment.o: RL_direct_environment.c
	$(CC) -c $(CFLAGS) $^ -o $@


$(BUILD_C_PATH)/C_TaskSpec_Parser.o: C_TaskSpec_Parser.c
	$(CC) -c $(CFLAGS) $^ -o $@

#$(BUILD_C_PATH)/RLStruct_util.o: RLStruct_util.c
#	$(CC) -c $(CFLAGS) $^ -o $@

$(BUILD_C_PATH)/Glue_utilities.o: Glue_utilities.c
	$(CC) -c $(CFLAGS) $^ -o $@


$(BUILD_CPP_PATH)/RL_glue.o: RL_glue.c
	$(CC) -c $(CFLAGS) $^ -o $@

$(BUILD_CPP_PATH)/RL_network.o: RL_network.c
	$(CC) -c $(CFLAGS) $^ -o $@

$(BUILD_CPP_PATH)/RL_network_glue.o: RL_network_glue.c
	$(CC) -c $(CFLAGS) $^ -o $@

$(BUILD_CPP_PATH)/RL_server_experiment.o: RL_server_experiment.c
	$(CC) -c $(CFLAGS) $^ -o $@

$(BUILD_CPP_PATH)/RL_network_experiment.o: RL_network_experiment.c
	$(CC) -c $(CFLAGS) $^ -o $@

$(BUILD_CPP_PATH)/RL_client_experiment.o: RL_client_experiment.c
	$(CC) -c $(CFLAGS) $^ -o $@

$(BUILD_CPP_PATH)/RL_server_agent.o: RL_server_agent.c
	$(CC) -c $(CFLAGS) $^ -o $@

$(BUILD_CPP_PATH)/RL_network_agent.o: RL_network_agent.c
	$(CC) -c $(CFLAGS) $^ -o $@

$(BUILD_CPP_PATH)/RL_client_agent.o: RL_client_agent.c
	$(CC) -c $(CFLAGS) $^ -o $@

$(BUILD_CPP_PATH)/RL_server_environment.o: RL_server_environment.c
	$(CC) -c $(CFLAGS) $^ -o $@

$(BUILD_CPP_PATH)/RL_network_environment.o: RL_network_environment.c
	$(CC) -c $(CFLAGS) $^ -o $@

$(BUILD_CPP_PATH)/RL_client_environment.o: RL_client_environment.c
	$(CC) -c $(CFLAGS) $^ -o $@

$(BUILD_CPP_PATH)/RL_direct_agent.o: RL_direct_agent.c
	$(CC) -c $(CFLAGS) $^ -o $@

$(BUILD_CPP_PATH)/RL_direct_environment.o: RL_direct_environment.c
	$(CC) -c $(CFLAGS) $^ -o $@

$(BUILD_CPP_PATH)/C_TaskSpec_Parser.o: C_TaskSpec_Parser.c
	$(CC) -c $(CFLAGS) $^ -o $@

# $(BUILD_CPP_PATH)/RLStruct_util.o: RLStruct_util.c
# 	$(CC) -c $(CFLAGS) $^ -o $@

$(BUILD_CPP_PATH)/Glue_utilities.o: Glue_utilities.c
	$(CC) -c $(CFLAGS) $^ -o $@


clean : 
	rm -Rf $(BUILD_CPP_PATH)/*
	rm -Rf $(BUILD_C_PATH)/*
	rm $(BIN_PATH)/RL_glue