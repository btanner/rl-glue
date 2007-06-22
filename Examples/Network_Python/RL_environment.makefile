# Agent Source Files
ENVIRONMENT = ./src/mines.py

# Path to RL-Glue to find RL_interface and related files.
RL-GLUE = ../../RL-Glue

RL_environment: $(ENVIRONMENT) RL_netlib.py RL_common.py
	cp $(ENVIRONMENT) Build/$@.py
	cp $(RL-GLUE)/Network/Environment/RL_client_environment.py $@
	chmod u+x $@
