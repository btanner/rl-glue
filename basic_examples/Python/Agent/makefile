# Agent Source Files
AGENT = ../../../Agents/SarsaAgent.py

# Path to RL-Glue to find RL_interface and related files.
RL_GLUE_PATH = ../../../RL-Glue

all: RL_agent

include $(RL_GLUE_PATH)/RL_glue.makefile

RL_agent: $(AGENT) RL_network.py RL_common.py
	cp $(AGENT) $@.py
	cp $(RL_GLUE_PATH)/Python/RL_client_agent.py $@
	chmod u+x $@

clean:
	rm -f RL_agent RL_agent.py RL_common.py RL_network.py