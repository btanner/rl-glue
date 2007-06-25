# Path to RL-Glue to find RL_interface and related files.
RL-GLUE = ../../RL-Glue

RL_netlib.py: $(RL-GLUE)/Python/RL_netlib.py
	cp $< Build/$@

RL_common.py: $(RL-GLUE)/Python/RL_common.py
	cp $< Build/$@
