
# Environment Source Files
ENVIRONMENT = ../../../Environments/MinesEnvironment.java

# Path to RL-Glue to find RL_interface and related files.
RL_GLUE_BASE = ../../../..
RL_GLUE_PATH = $(RL_GLUE_BASE)/RL-Glue
UTILS_PATH = $(RL_GLUE_BASE)/Utils

BUILD_PATH = ./Build

all: RL_environment

include $(RL_GLUE_PATH)/RL_glue.makefile

ifeq ($(shell uname -o),Cygwin)
	CLASSPATH = `cygpath -wp $(BUILD_PATH):$(RL_GLUE_PATH)/Java/RL-Glue.jar`
	CLASSPATH2 = "\`cygpath -wp $(BUILD_PATH):$(RL_GLUE_PATH)/Java/RL-Glue.jar\`"
else
	CLASSPATH = $(BUILD_PATH):$(RL_GLUE_PATH)/Java/RL-Glue.jar
endif

RL_environment: $(ENVIRONMENT) $(BUILD_PATH)/rlVizLib/utilities/TaskSpecObject.class
	javac -d $(BUILD_PATH) -classpath $(CLASSPATH) $(ENVIRONMENT)
	echo -e "#! /bin/bash\njava -Xmx128M -classpath $(CLASSPATH2) rlglue.environment.EnvironmentLoader MinesEnvironment.MinesEnvironment" | cat > RL_environment
	chmod u+x RL_environment
	
clean:
	rm -rf RL_environment Build/*


