# Path to RL-Glue to find RL_interface and related files.
RL-GLUE = ../../RL-Glue

RL_network.py: $(RL-GLUE)/Python/Network/RL_network.py
	cp $< Build/$@

RL_common.py: $(RL-GLUE)/Python/RL_common.py
	cp $< Build/$@
