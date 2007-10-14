.PHONY: all clean

VPATH = $(RL_GLUE_PATH):$(RL_GLUE_PATH)/Network:$(RL_GLUE_PATH)/Network/Glue:$(UTILS_PATH)
VPATH += :$(RL_GLUE_PATH)/Network/Agent:$(RL_GLUE_PATH)/Network/Environment:$(RL_GLUE_PATH)/Network/Experiment

NETWORK_OBJECTS=RL_network.o
OBJECTS = RL_glue.o

ifeq ($(AGENT_NETWORKED),1)
	OBJECTS += RL_server_agent.o RL_network_agent.o
	PREREQS += RL_agent
else
	OBJECTS += RL_direct_agent.o $(AGENT_OBJECTS)
endif

ifeq ($(ENV_NETWORKED),1)
	OBJECTS += RL_server_environment.o RL_network_environment.o
	PREREQS += RL_environment
else
	OBJECTS += RL_direct_environment.o $(ENV_OBJECTS)
endif


ifneq ("$(AGENT_NETWORKED)$(ENV_NETWORKED)","00")
	OBJECTS += RL_server_experiment.o RL_network_experiment.o RL_network_glue.o $(NETWORK_OBJECTS)
	PREREQS += RL_experiment
else
	OBJECTS += $(EXP_OBJECTS)
endif

$(BIN_PATH)/RL_glue: $(addprefix $(BIN_PATH)/,$(PREREQS)) $(addprefix $(BUILD_PATH)/,$(OBJECTS))
	$(CC) -o $(BIN_PATH)/RL_glue $(addprefix $(BUILD_PATH)/,$(OBJECTS))



$(BIN_PATH)/RL_environment: $(addprefix $(BUILD_PATH)/,$(ENV_OBJECTS)) $(BUILD_PATH)/RL_client_environment.o
	$(CC) -o $(BIN_PATH)/RL_environment $(addprefix $(BUILD_PATH)/,$(ENV_OBJECTS)) $(BUILD_PATH)/RL_client_environment.o

$(BIN_PATH)/RL_experiment: $(addprefix $(BUILD_PATH)/,$(EXP_OBJECTS)) $(BUILD_PATH)/RL_client_experiment.o
	$(CC) -o $(BIN_PATH)/RL_experiment $(addprefix $(BUILD_PATH)/, $(EXP_OBJECTS)) $(BUILD_PATH)/RL_client_experiment.o


$(BUILD_PATH)/RL_glue.o: RL_glue.c
	$(CC) -c $(CFLAGS) $^ -o $@

$(BUILD_PATH)/RL_network.o: RL_network.c
	$(CC) -c $(CFLAGS) $^ -o $@

$(BUILD_PATH)/RL_network_glue.o: RL_network_glue.c
	$(CC) -c $(CFLAGS) $^ -o $@

$(BUILD_PATH)/RL_server_experiment.o: RL_server_experiment.c
	$(CC) -c $(CFLAGS) $^ -o $@

$(BUILD_PATH)/RL_network_experiment.o: RL_network_experiment.c
	$(CC) -c $(CFLAGS) $^ -o $@

$(BUILD_PATH)/RL_server_agent.o: RL_server_agent.c
	$(CC) -c $(CFLAGS) $^ -o $@

$(BUILD_PATH)/RL_network_agent.o: RL_network_agent.c
	$(CC) -c $(CFLAGS) $^ -o $@

$(BUILD_PATH)/RL_server_environment.o: RL_server_environment.c
	$(CC) -c $(CFLAGS) $^ -o $@

$(BUILD_PATH)/RL_network_environment.o: RL_network_environment.c
	$(CC) -c $(CFLAGS) $^ -o $@

$(BUILD_PATH)/RL_direct_agent.o: RL_direct_agent.c
	$(CC) -c $(CFLAGS) $^ -o $@

$(BUILD_PATH)/RL_direct_environment.o: RL_direct_environment.c
	$(CC) -c $(CFLAGS) $^ -o $@

$(BUILD_PATH)/RL_client_environment.o: RL_client_environment.c
	$(CC) -c $(CFLAGS) $^ -o $@

$(BUILD_PATH)/RL_client_agent.o: RL_client_agent.c
	$(CC) -c $(CFLAGS) $^ -o $@

$(BUILD_PATH)/RL_client_experiment.o: RL_client_experiment.c
	$(CC) -c $(CFLAGS) $^ -o $@

$(BUILD_PATH)/Glue_utilities.o: Glue_utilities.c
	$(CC) -c $(CFLAGS) $^ -o $@

$(BUILD_PATH)/rlVizLib/utilities/TaskSpecObject.class: TaskSpecObject.java
	javac -d $(BUILD_PATH) $^

RL_network.py: $(RL_GLUE_PATH)/Python/RL_network.py
	cp $^ $@

RL_common.py: $(RL_GLUE_PATH)/Python/RL_common.py
	cp $^ $@