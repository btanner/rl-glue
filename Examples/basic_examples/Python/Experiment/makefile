# Experiment Source Files
EXPERIMENT = experiment
EXPERIMENT_PATH = ../../../Experiments

# Path to RL-Glue to find RL_interface and related files.
RL_GLUE_PATH = ../../../../RL-Glue

all: RL_experiment

include $(RL_GLUE_PATH)/RL_glue.makefile

RL_experiment:
	echo -e "#! /bin/bash\n  PYTHONPATH=$(RL_GLUE_PATH)/Python python $(EXPERIMENT_PATH)/$(EXPERIMENT).py " | cat > RL_experiment
	chmod u+x RL_experiment
	
clean:
	rm -f RL_experiment *.pyc $(EXPERIMENT_PATH)/*.pyc