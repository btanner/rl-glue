
# Environment Source Files
ENVIRONMENT = ../../../Environments/MinesEnvironment.java

# Path to RL-Glue to find RL_interface and related files.
RL_GLUE_PATH = ../../../RL-Glue

all: RL_environment

include $(RL_GLUE_PATH)/RL_glue.makefile

RL_environment: $(ENVIRONMENT)
	javac -d . -cp $(RL_GLUE_PATH)/../../libraries/RLVizLib.jar $(ENVIRONMENT)
	echo -e "#! /bin/bash\njava -Xmx128M -cp $(RL_GLUE_PATH)/../../libraries/RLVizLib.jar:. rlglue.environment.EnvironmentLoader MinesEnvironment.MinesEnvironment" | cat > RL_environment
	chmod u+x RL_environment
	
clean:
	rm -rf RL_environment MinesEnvironment