# Agent Source Files
ENVIRONMENT = ./src/mines.py

# Path to RL-Glue to find RL_interface and related files.
RL-GLUE = ../../RL-Glue

RL_environment: $(ENVIRONMENT) RL_network.py RL_common.py
	cp $(ENVIRONMENT) bin/$@.py
	cp $(RL-GLUE)/Python/RL_client_environment.py bin/$@
	chmod u+x bin/$@
