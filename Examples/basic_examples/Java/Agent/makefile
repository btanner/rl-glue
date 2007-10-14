
# Agent Source Files
AGENT = ../../../Agents/RandomAgent.java

# Path to RL-Glue to find RL_interface and related files.
RL_GLUE_BASE = ../../../..
RL_GLUE_PATH = $(RL_GLUE_BASE)/RL-Glue
UTILS_PATH = $(RL_GLUE_BASE)/Utils
BUILD_PATH = ./Build

all: RL_agent

include $(RL_GLUE_PATH)/RL_glue.makefile

RL_agent: $(AGENT) $(BUILD_PATH)/rlVizLib/utilities/TaskSpecObject.class
	javac -d $(BUILD_PATH) -cp $(BUILD_PATH):$(RL_GLUE_PATH)/Java/RL-Glue.jar $(AGENT)
	echo -e "#! /bin/bash\njava -Xmx128M -cp $(RL_GLUE_PATH)/Java/RL-Glue.jar:$(BUILD_PATH) rlglue.agent.AgentLoader RandomAgent.RandomAgent" | cat > RL_agent
	chmod u+x RL_agent

clean:
	rm -rf RL_agent Build/*