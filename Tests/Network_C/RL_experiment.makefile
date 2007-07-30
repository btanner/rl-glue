EXP_RL_GLUE = ../../RL-Glue
EXP_BUILD_PATH = Build
EXP_BIN_PATH = bin
EXP_SRC_PATH = src

EXP_CC      = gcc  
EXP_CFLAGS  = -I$(EXP_RL_GLUE)/ -ansi -pedantic -Wall
EXP_LDFLAGS =

EXPERIMENT_OBJECTS = experiment.o RL_client_experiment.o RL_experiment_netlib.o

RL_experiment: $(EXPERIMENT_OBJECTS)
	$(EXP_CC) -o $(EXP_BIN_PATH)/$@ $(addprefix $(EXP_BUILD_PATH)/, $(EXPERIMENT_OBJECTS)) $(EXP_LDFLAGS)
	
experiment.o: $(EXP_SRC_PATH)/experiment.c .
	$(EXP_CC) -c $(EXP_CFLAGS) $< -o $(EXP_BUILD_PATH)/$@

RL_client_experiment.o: $(EXP_RL_GLUE)/Network/Experiment/RL_client_experiment.c
	$(EXP_CC) -c $(EXP_CFLAGS) $< -o $(EXP_BUILD_PATH)/$@
	
# Give this a unique name so it doesn't conflict with other RL_network.o rules included by the main makefile 
# Alernatively we could just build it once.
RL_experiment_netlib.o: $(EXP_RL_GLUE)/Network/RL_network.c
	$(EXP_CC) -c $(EXP_CFLAGS) $< -o $(EXP_BUILD_PATH)/$@


