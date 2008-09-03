# ------------------------------------------------------------------
# Changes these values to use different source files and different 
# directories 
# ------------------------------------------------------------------
Environment = mines
ENV_SRC_PATH = ./src
BUILD_PATH = ./Build
RL_GLUE_BASE = ../../..
# ------------------------------------------------------------------

RL_GLUE_PATH = $(RL_GLUE_BASE)/RL-Glue

all: RL_environment

include $(RL_GLUE_PATH)/Makefile

ECHOFLAG = -e

ifeq ($(shell uname -s),Darwin)
	ECHOFLAG = 
endif

RL_environment:
	echo $(ECHOFLAG) "#! /bin/bash\n PYTHONPATH=$(RL_GLUE_PATH)/Python:$(ENV_SRC_PATH) python -c \"import rlglue.environment.EnvironmentLoader\" $(Environment)" | cat > RL_environment
	chmod u+x RL_environment

clean:
	rm -f RL_environment *.pyc src/*.pyc
	rm -f -r $(Environment).app