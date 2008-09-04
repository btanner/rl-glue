# ------------------------------------------------------------------
# Changes these values to use different source files and different 
# directories 
# ------------------------------------------------------------------
ENVIRONMENT = MinesEnvironment.java
ENV_SRC_PATH = ./src
BUILD_PATH = ./Build
RL_GLUE_BASE = ../../..
# ------------------------------------------------------------------

ENV_NAME= $(basename $(ENVIRONMENT))
RL_GLUE_PATH = $(RL_GLUE_BASE)/RL-Glue
RL_GLUE_JAR = $(RL_GLUE_PATH)/Java/RL-Glue.jar

all: RL_environment Make_env_executable

include $(RL_GLUE_PATH)/Makefile

ECHOFLAG=-e

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

RL_environment: $(ENV_SRC_PATH)/$(ENVIRONMENT)
	javac -d $(BUILD_PATH) -classpath $(CLASSPATH) $(ENV_SRC_PATH)/$(ENVIRONMENT)

Make_env_executable: 
#	echo $(ECHOFLAG) "#! /bin/bash\njava -Xmx128M -classpath $(CLASSPATH2) rlglue.environment.EnvironmentLoader $(ENV_NAME).$(ENV_NAME)" | cat > RL_environment
	echo $(ECHOFLAG) "#! /bin/bash\njava -Xmx128M -classpath $(CLASSPATH2) rlglue.RLGlueCore --environment $(ENV_NAME).$(ENV_NAME)" | cat > RL_environment
	chmod u+x RL_environment
	
clean:
	rm -rf RL_environment Build/*


