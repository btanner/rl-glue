
# Experiment Source Files
EXPERIMENT = ../../../Experiments/Experiment.java

# Path to RL-Glue to find RL_interface and related files.
RL_GLUE_PATH = ../../../RL-Glue

all: RL_experiment

include $(RL_GLUE_PATH)/RL_glue.makefile

RL_experiment: $(EXPERIMENT)
	javac -d . -cp $(RL_GLUE_PATH)/../../libraries/RLVizLib.jar $(EXPERIMENT)
	echo -e "#! /bin/bash\njava -Xmx128M -cp $(RL_GLUE_PATH)/../../libraries/RLVizLib.jar:. Experiment" | cat > RL_experiment
	chmod u+x RL_experiment

clean:
	rm -f RL_experiment Experiment.class