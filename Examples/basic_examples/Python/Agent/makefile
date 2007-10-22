# Agent Source Files
AGENT = RandomAgent
AGENT_PATH = ../../../Agents

# Path to RL-Glue to find RL_interface and related files.
RL_GLUE_PATH = ../../../../RL-Glue

all: RL_agent

include $(RL_GLUE_PATH)/RL_glue.makefile

RL_agent:
	echo -e "#! /bin/bash\n PYTHONPATH=$(RL_GLUE_PATH)/Python:$(AGENT_PATH) python -c \"import rlglue.agent.AgentLoader\" $(AGENT)" | cat > RL_agent
	chmod u+x RL_agent

clean:
	rm -f RL_agent *.pyc $(AGENT_PATH)/*.pyc