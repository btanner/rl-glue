# ------------------------------------------------------------------
# Changes these values to use different source files and different 
# directories 
# ------------------------------------------------------------------
EXPERIMENT = Experiment.java
EXP_SRC_PATH = ./src
BUILD_PATH = ./Build
RL_GLUE_BASE = ../../..
# ------------------------------------------------------------------

RL_GLUE_PATH = $(RL_GLUE_BASE)/RL-Glue
RL_GLUE_JAR = $(RL_GLUE_PATH)/Java/RL-Glue.jar

all: RL_experiment Make_exp_executable

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

RL_experiment: $(EXP_SRC_PATH)/$(EXPERIMENT)
	javac -d $(BUILD_PATH) -classpath $(CLASSPATH) $(EXP_SRC_PATH)/$(EXPERIMENT)

Make_exp_executable: 
	echo $(ECHOFLAG) "#! /bin/bash\njava -Xmx128M -classpath $(CLASSPATH2) Experiment" | cat > RL_experiment
	chmod u+x RL_experiment

clean:
	rm -f RL_experiment Build/*


