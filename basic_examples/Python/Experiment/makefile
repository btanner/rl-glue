# Experiment Source Files
EXPERIMENT = ../../../Experiments/experiment.py

# Path to RL-Glue to find RL_interface and related files.
RL_GLUE_PATH = ../../../RL-Glue

all: RL_experiment

include $(RL_GLUE_PATH)/RL_glue.makefile

RL_experiment: $(EXPERIMENT) RL_network.py RL_common.py
	cp $(EXPERIMENT) $@
	cp $(RL_GLUE_PATH)/Python/RL_client_experiment.py $@.py
	chmod u+x $@
	
clean:
	rm -f RL_experiment RL_experiment.py RL_network.py RL_common.py