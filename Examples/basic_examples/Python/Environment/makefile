# Environment Source Files
ENVIRONMENT = ../../../Environments/mines.py

# Path to RL-Glue to find RL_interface and related files.
RL_GLUE_PATH = ../../../RL-Glue

all: RL_environment

include $(RL_GLUE_PATH)/RL_glue.makefile

RL_environment: $(ENVIRONMENT) RL_network.py RL_common.py
	cp $(ENVIRONMENT) $@.py
	cp $(RL_GLUE_PATH)/Python/RL_client_environment.py $@
	chmod u+x $@
	
clean:
	rm -f RL_environment RL_environment.py RL_network.py RL_common.py