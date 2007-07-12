# Agent Source Files
AGENT = ./src/SarsaAgent.py

# Path to RL-Glue to find RL_interface and related files.
RL-GLUE = ../../RL-Glue

RL_agent: $(AGENT) RL_network.py RL_common.py
	cp $(AGENT) bin/$@.py
	cp $(RL-GLUE)/Python/RL_client_agent.py bin/$@
	chmod u+x bin/$@
