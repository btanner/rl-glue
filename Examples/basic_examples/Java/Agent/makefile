
# Agent Source Files
AGENT = ../../../Agents/RandomAgent.java

# Path to RL-Glue to find RL_interface and related files.
RL_GLUE_BASE = ../../../..
RL_GLUE_PATH = $(RL_GLUE_BASE)/RL-Glue
RL_GLUE_JAR = $(RL_GLUE_PATH)/Java/RL-Glue.jar
UTILS_PATH = $(RL_GLUE_BASE)/Utils
BUILD_PATH = ./Build

all: RL_agent

include $(RL_GLUE_PATH)/RL_glue.makefile

ECHOFLAG = -e

ifeq ($(shell uname -s),Darwin)
	ECHOFLAG = 
endif
ifeq ($(shell uname -s),Cygwin)
	CLASSPATH=`cygpath -wp $(BUILD_PATH):$(RL_GLUE_PATH)/Java/RL-Glue.jar`
	CLASSPATH2="\`cygpath -wp $(BUILD_PATH):$(RL_GLUE_PATH)/Java/RL-Glue.jar\`"
else
	CLASSPATH=$(BUILD_PATH):$(RL_GLUE_JAR)
	CLASSPATH2=$(CLASSPATH)
endif 

RL_agent: $(AGENT) $(BUILD_PATH)/rlVizLib/utilities/TaskSpecObject.class
	javac -d $(BUILD_PATH) -classpath $(CLASSPATH) $(AGENT)
	echo $(ECHOFLAG) "#! /bin/bash\njava -Xmx128M -classpath $(CLASSPATH2) rlglue.RLGlueCore --agent RandomAgent.RandomAgent" | cat > RL_agent
	chmod u+x RL_agent

clean:
	rm -rf RL_agent Build/*
