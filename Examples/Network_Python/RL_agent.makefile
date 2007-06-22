# Agent Source Files
AGENT = ./src/SarsaAgent.py

# Path to RL-Glue to find RL_interface and related files.
RL-GLUE = ../../RL-Glue

# ==================================== User Code ===========================================
all: RL_agent

# ------------------------- Compile And Link the Agent (Network) ----------------------------
RL_agent: $(AGENT) RL_netlib.py RL_common.py
	cp $(AGENT) Build/$@.py
	cp $(RL-GLUE)/Network/Agent/RL_client_agent.py $@
	chmod u+x $@
# -------------------------------------------------------------------------------------------
# ===========================================================================================

# ------------------------------------ Network Library --------------------------------------
RL_netlib.py: $(RL-GLUE)/Network/RL_netlib.py
	cp $< Build/$@

RL_common.py: $(RL-GLUE)/RL_common.py
	cp $< Build/$@
# -------------------------------------------------------------------------------------------
