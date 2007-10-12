
# Agent Source Files
AGENT = ../../../Agents/RandomAgent.java

# Path to RL-Glue to find RL_interface and related files.
RL_GLUE_PATH = ../../../RL-Glue

all: RL_agent

include $(RL_GLUE_PATH)/RL_glue.makefile

RL_agent: $(AGENT)
	javac -d . -cp $(RL_GLUE_PATH)/../../libraries/RLVizLib.jar $(AGENT)
	echo -e "#! /bin/bash\njava -Xmx128M -cp $(RL_GLUE_PATH)/../../libraries/RLVizLib.jar:. rlglue.agent.AgentLoader RandomAgent.RandomAgent" | cat > RL_agent
	chmod u+x RL_agent

clean:
	rm -rf RL_agent RandomAgent 