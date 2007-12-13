
# Experiment Source Files
EXPERIMENT = ../../../Experiments/Experiment.java

# Path to RL-Glue to find RL_interface and related files.
RL_GLUE_BASE = ../../../..
RL_GLUE_PATH = $(RL_GLUE_BASE)/RL-Glue

BUILD_PATH = ./Build

all: RL_experiment

include $(RL_GLUE_PATH)/RL_glue.makefile

ifeq ($(shell uname -o),Cygwin)
	CLASSPATH=`cygpath -wp $(BUILD_PATH):$(RL_GLUE_PATH)/Java/RL-Glue.jar`
	CLASSPATH2="\`cygpath -wp $(BUILD_PATH):$(RL_GLUE_PATH)/Java/RL-Glue.jar\`"
else
	CLASSPATH=$(BUILD_PATH):$(RL_GLUE_PATH)/Java/RL-Glue.jar
endif

RL_experiment: $(EXPERIMENT)
	javac -d $(BUILD_PATH) -classpath $(CLASSPATH) $(EXPERIMENT)
	echo -e "#! /bin/bash\njava -Xmx128M -classpath $(CLASSPATH2) Experiment" | cat > RL_experiment
	chmod u+x RL_experiment

clean:
	rm -f RL_experiment Build/*


