# Experiment Source Files
EXPERIMENT = ./src/experiment.py

# Path to RL-Glue to find RL_interface and related files.
RL-GLUE = ../../RL-Glue

RL_experiment: $(EXPERIMENT) RL_netlib.py
	cp $(RL-GLUE)/Network/Experiment/RL_client_experiment.py Build/
	cp $(EXPERIMENT) $@
	chmod u+x $@

