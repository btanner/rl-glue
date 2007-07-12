# Path to RL-Glue to find RL_interface and related files.
RL-GLUE = ../../RL-Glue

RL_network.py: $(RL-GLUE)/Python/RL_network.py
	cp $< bin/$@

RL_common.py: $(RL-GLUE)/Python/RL_common.py
	cp $< bin/$@
