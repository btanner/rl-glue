# Experiment Source Files
EXPERIMENT = ./src/experiment.py

# Path to RL-Glue to find RL_interface and related files.
RL-GLUE = ../../RL-Glue

# ==================================== User Code ===========================================
all: RL_experiment

# ---------------------- Compile And Link the Experiment (Network) --------------------------
RL_experiment: $(EXPERIMENT) RL_client_experiment.py RL_netlib.py
	cp $(RL-GLUE)/Network/Experiment/RL_client_experiment.py Build/
	cp $(EXPERIMENT) $@
	chmod u+x $@
# -------------------------------------------------------------------------------------------
# ===========================================================================================

# ------------------------------------ Network Library --------------------------------------
RL_netlib.py: $(RL-GLUE)/Network/RL_netlib.py
	cp $< Build/$@

RL_common.py: $(RL-GLUE)/RL_common.py
	cp $< Build/$@
# -------------------------------------------------------------------------------------------
