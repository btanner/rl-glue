# Env Source Files
ENV = mines
ENV_PATH = ../../../Environments

# Path to RL-Glue to find RL_interface and related files.
RL_GLUE_PATH = ../../../../RL-Glue

all: RL_environment

include $(RL_GLUE_PATH)/RL_glue.makefile

RL_environment:
	echo -e "#! /bin/bash\n PYTHONPATH=$(RL_GLUE_PATH)/Python:$(ENV_PATH) python -c \"import rlglue.environment.EnvironmentLoader\" $(ENV)" | cat > RL_environment
	chmod u+x RL_environment

clean:
	rm -f RL_environment *.pyc $(ENV_PATH)/*.pyc