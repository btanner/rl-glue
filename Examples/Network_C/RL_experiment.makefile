RL-GLUE = ../../RL-Glue
BUILD_PATH = Build

CC      = gcc  
CFLAGS  = -I$(RL-GLUE)/ -ansi -pedantic -Wall
LDFLAGS =

EXPERIMENT_OBJECTS = experiment.o RL_client_experiment.o RL_network_experiment.o

RL_experiment: $(EXPERIMENT_OBJECTS)
	$(CC) -o $@ $(addprefix $(BUILD_PATH)/, $(EXPERIMENT_OBJECTS))
	
experiment.o: ./src/experiment.c .
	$(CC) -c $(CFLAGS) $< -o $(BUILD_PATH)/$@

RL_client_experiment.o: $(RL-GLUE)/Network/Experiment/RL_client_experiment.c
	$(CC) -c $(CFLAGS) $< -o $(BUILD_PATH)/$@
	
RL_network_experiment.o: $(RL-GLUE)/Network/RL_network.c
	$(CC) -c $(CFLAGS) $< -o $(BUILD_PATH)/$@


