# ------------------------------------------------------------------
# Changes these values to use different source files and different 
# directories 
# ------------------------------------------------------------------
EXPERIMENT = experiment
EXPERIMENT_SRC_PATH = src
BUILD_PATH = ./Build
RL_GLUE_BASE = ../../..
# ------------------------------------------------------------------

RL_GLUE_PATH = $(RL_GLUE_BASE)/RL-Glue

all: RL_experiment

include $(RL_GLUE_PATH)/Makefile

ECHOFLAG = -e

ifeq ($(shell uname -s),Darwin)
	ECHOFLAG = 
endif

RL_experiment:
	echo $(ECHOFLAG) "#! /bin/bash\n  PYTHONPATH=$(RL_GLUE_PATH)/Python python $(EXPERIMENT_SRC_PATH)/$(EXPERIMENT).py " | cat > RL_experiment
	chmod u+x RL_experiment
	
clean:
	rm -f RL_experiment *.pyc src/*.pyc
	rm -f -r $(EXPERIMENT).app