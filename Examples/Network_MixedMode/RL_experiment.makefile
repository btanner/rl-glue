RL-GLUE = ../../RL-Glue
BUILD_PATH = Build
BIN_PATH = bin
SRC_PATH = src

EXPCC      = gcc  
CFLAGS  = -I$(RL-GLUE)/ -ansi -pedantic -Wall
LDFLAGS =

EXPERIMENT_OBJECTS = experiment.o RL_client_experiment.o RL_network.o

RL_experiment: $(EXPERIMENT_OBJECTS)
	$(EXPCC) -o $(BIN_PATH)/$@ $(addprefix $(BUILD_PATH)/, $(EXPERIMENT_OBJECTS))
	
experiment.o: $(SRC_PATH)/experiment.c .
	$(EXPCC) -c $(CFLAGS) $< -o $(BUILD_PATH)/$@

RL_client_experiment.o: $(RL-GLUE)/Network/Experiment/RL_client_experiment.c
	$(EXPCC) -c $(CFLAGS) $< -o $(BUILD_PATH)/$@
	
RL_network.o: $(RL-GLUE)/Network/RL_network.c
	$(EXPCC) -c $(CFLAGS) $< -o $(BUILD_PATH)/$@


