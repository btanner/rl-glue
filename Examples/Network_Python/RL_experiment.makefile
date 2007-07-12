# Experiment Source Files
EXPERIMENT = ./src/experiment.py

# Path to RL-Glue to find RL_interface and related files.
RL-GLUE = ../../RL-Glue

RL_experiment: $(EXPERIMENT) RL_network.py
	cp $(RL-GLUE)/Python/RL_client_experiment.py bin/
	cp $(EXPERIMENT) bin/$@
	chmod u+x bin/$@

