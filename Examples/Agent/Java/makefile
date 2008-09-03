# ------------------------------------------------------------------
# Changes these values to use different source files and different 
# directories 
# ------------------------------------------------------------------
AGENT = RandomAgent.java
AGENT_SRC_PATH = ./src
BUILD_PATH = ./Build
RL_GLUE_BASE = ../../..
# ------------------------------------------------------------------

AGENT_NAME = $(basename $(AGENT))
RL_GLUE_PATH = $(RL_GLUE_BASE)/RL-Glue
RL_GLUE_JAR = $(RL_GLUE_PATH)/Java/RL-Glue.jar

all: RL_agent Make_agent_executable

include $(RL_GLUE_PATH)/Makefile

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

#RL_agent: $(AGENT_SRC_PATH)/$(AGENT) $(BUILD_PATH)/rlVizLib/utilities/TaskSpecObject.class
#	javac -d $(BUILD_PATH) -classpath $(CLASSPATH) $(AGENT_SRC_PATH)/$(AGENT)

RL_agent: $(AGENT_SRC_PATH)/$(AGENT)
	javac -d $(BUILD_PATH) -classpath $(CLASSPATH) $(AGENT_SRC_PATH)/$(AGENT)

Make_agent_executable:
	echo $(ECHOFLAG) "#! /bin/bash\njava -Xmx128M -classpath $(CLASSPATH2) rlglue.RLGlueCore --agent $(AGENT_NAME).$(AGENT_NAME)" | cat > RL_agent
	chmod u+x RL_agent

clean:
	rm -rf RL_agent Build/*
