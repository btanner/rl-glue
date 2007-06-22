# Agent Source Files
ENVIRONMENT = ./src/mines.py

# Path to RL-Glue to find RL_interface and related files.
RL-GLUE = ../../RL-Glue

# ==================================== User Code ===========================================
all: RL_environment

# ------------------------- Compile And Link the Agent (Network) ----------------------------
RL_environment: $(ENVIRONMENT) RL_netlib.py RL_common.py
	cp $(ENVIRONMENT) Build/$@.py
	cp $(RL-GLUE)/Network/Environment/RL_client_environment.py $@
	chmod u+x $@
# -------------------------------------------------------------------------------------------
# ===========================================================================================

# ------------------------------------ Network Library --------------------------------------
RL_netlib.py: $(RL-GLUE)/Network/RL_netlib.py
	cp $< Build/$@

RL_common.py: $(RL-GLUE)/RL_common.py
	cp $< Build/$@
# -------------------------------------------------------------------------------------------
