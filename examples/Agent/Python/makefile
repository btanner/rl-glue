# ------------------------------------------------------------------
# Changes these values to use different source files and different 
# directories 
# ------------------------------------------------------------------
AGENT = RandomAgent
AGENT_SRC_PATH = ./src
BUILD_PATH = ./Build
RL_GLUE_BASE = ../../..
# ------------------------------------------------------------------


RL_GLUE_PATH = $(RL_GLUE_BASE)/RL-Glue

all: RL_agent

include $(RL_GLUE_PATH)/Makefile

ECHOFLAG = -e

ifeq ($(shell uname -s),Darwin)
	ECHOFLAG = 
endif

RL_agent:
	echo $(ECHOFLAG) "#! /bin/bash\n PYTHONPATH=$(RL_GLUE_PATH)/Python:$(AGENT_SRC_PATH) python -c \"import rlglue.agent.AgentLoader\" $(AGENT)" | cat > RL_agent
	chmod u+x RL_agent

clean:
	rm -f RL_agent *.pyc src/*.pyc 
	rm -f -r src/$(AGENT).app