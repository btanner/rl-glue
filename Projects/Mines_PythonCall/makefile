# Agent Source Files
AGENT = ./Agent/ZeroAgent.py

# Environment Source Files
ENVIRONMENT = ./Environment/mines.py

# Experiment Source Files
EXPERIMENT = ./Experiment/experiment.py

# Path to RL-Glue to find RL_interface and related files.
RL-GLUE = ../../RL-Glue

# Compiler flags
CC      = gcc
CFLAGS  = -I$(RL-GLUE)/ -ansi -pedantic -Wall
LDFLAGS =

# ==================================== User Code ===========================================
all: RL_glue RL_agent RL_environment RL_experiment

# ----------------------- Compile And Link RL-Glue 2.0 (Agent/Environment Network) ----------------------
RL_glue: RL_interface.o RL_network.o RL_netlib.o RL_server_experiment.o RL_server_environment.o RL_server_agent.o 
	$(CC) $(LDFLAGS) -o $@ Build/RL_interface.o Build/RL_network.o Build/RL_netlib.o Build/RL_server_experiment.o Build/RL_server_environment.o Build/RL_server_agent.o
# -------------------------------------------------------------------------------------------

# ------------------------- Compile And Link the Agent (Network) ----------------------------

RL_agent: $(AGENT) RL_netlib.py RL_common.py
	cp $(AGENT) Build/$@.py
	cp $(RLGLUE_PATH)/Network/Agent/RL_client_agent.py $@
	chmod u+x $@
# -------------------------------------------------------------------------------------------

# ---------------------- Compile And Link the Environment (Network) -------------------------
RL_environment: $(ENVIRONMENT) RL_netlib.py RL_common.py
	cp $(ENVIRONMENT) Build/$@.py
	cp $(RLGLUE_PATH)/Network/Environment/RL_client_environment.py $@
	chmod u+x $@
# -------------------------------------------------------------------------------------------

# ---------------------- Compile And Link the Experiment (Network) --------------------------
RL_experiment: $(EXPERIMENT) RL_client_experiment.py RL_netlib.py
	cp $(RLGLUE_PATH)/Network/Experiment/RL_client_experiment.py Build/
	cp $(EXPERIMENT) $@
	chmod u+x $@
# -------------------------------------------------------------------------------------------
# ===========================================================================================






# ============================================================================================
#                                     RLGLUE 2.0
# ============================================================================================

# Path to RL-Glue to find RL_interface and related files.
RLGLUE_PATH = ../../RL-Glue

# Compiler flags
RLGLUE_CC      = gcc
RLGLUE_CFLAGS  = -I$(RLGLUE_PATH)/ -ansi -pedantic -Wall

# ===================================== INTERFACE ============================================
# ------------------------------- Compile The RL-Interface ----------------------------------
RL_interface.o: $(RLGLUE_PATH)/RL_interface.c $(RLGLUE_PATH)/RL_interface.h
	$(RLGLUE_CC) $(RLGLUE_CFLAGS) -c $< -o Build/$@
# -------------------------------------------------------------------------------------------

# ====================================== DIRECT =============================================
# ----------------------------------- Direct Call Objects -----------------------------------
RL_direct_glue.o: $(RLGLUE_PATH)/Direct/Glue/RL_direct_glue.c
	$(RLGLUE_CC) $(RLGLUE_CFLAGS) -c $< -o Build/$@

RL_direct_agent.o: $(RLGLUE_PATH)/Direct/Agent/RL_direct_agent.c
	$(RLGLUE_CC) $(RLGLUE_CFLAGS) -c $< -o Build/$@

RL_direct_environment.o: $(RLGLUE_PATH)/Direct/Environment/RL_direct_environment.c
	$(RLGLUE_CC) $(RLGLUE_CFLAGS) -c $< -o Build/$@

RL_direct_experiment.o: $(RLGLUE_PATH)/Direct/Experiment/RL_direct_experiment.c
	$(RLGLUE_CC) $(RLGLUE_CFLAGS) -c $< -o Build/$@
# -------------------------------------------------------------------------------------------

# ====================================== NETWORK ============================================
# -------------------------------------- Client ---------------------------------------------
RL_client_agent.py: $(RLGLUE_PATH)/Network/Agent/RL_client_agent.py
	cp $< Build/$@

RL_client_environment.py: $(RLGLUE_PATH)/Network/Environment/RL_client_environment.py
	cp $< Build/$@

RL_client_experiment.py: $(RLGLUE_PATH)/Network/Experiment/RL_client_experiment.py
	cp $< Build/$@
# -------------------------------------------------------------------------------------------

# -------------------------------------- Server ---------------------------------------------
RL_server_agent.o: $(RLGLUE_PATH)/Network/Glue/RL_server_agent.c
	$(RLGLUE_CC) $(RLGLUE_CFLAGS) -c $< -o Build/$@

RL_server_environment.o: $(RLGLUE_PATH)/Network/Glue/RL_server_environment.c
	$(RLGLUE_CC) $(RLGLUE_CFLAGS) -c $< -o Build/$@

RL_server_experiment.o: $(RLGLUE_PATH)/Network/Glue/RL_server_experiment.c
	$(RLGLUE_CC) $(RLGLUE_CFLAGS) -c $< -o Build/$@
# -------------------------------------------------------------------------------------------

# ------------------------------------ Network Library --------------------------------------
RL_network.o: $(RLGLUE_PATH)/Network/RL_network.c
	$(RLGLUE_CC) $(RLGLUE_CFLAGS) -c $< -o Build/$@

RL_netlib.o: $(RLGLUE_PATH)/Network/RL_netlib.c $(RLGLUE_PATH)/Network/RL_netlib.h
	$(RLGLUE_CC) $(RLGLUE_CFLAGS) -c $< -o Build/$@

RL_netlib.py: $(RLGLUE_PATH)/Network/RL_netlib.py
	cp $< Build/$@

RL_common.py: $(RLGLUE_PATH)/RL_common.py
	cp $< Build/$@
# -------------------------------------------------------------------------------------------


clean:
	rm -fr Build/*.o

tidy:
	rm -fr *.*~
	rm -fr *~
	rm -fr \#*