ENV_RL_GLUE = ../../RL-Glue
ENV_BUILD_PATH = Build
ENV_BIN_PATH = bin
ENV_SRC_PATH = src

ENV_CC      = gcc  
ENV_CFLAGS  = -I$(ENV_RL_GLUE)/ -ansi -pedantic -Wall
ENV_LDFLAGS =

ENV_OBJECTS = TestEnvironment.o RL_client_environment.o RL_environment_netlib.o

RL_environment: $(ENV_OBJECTS)
	$(ENV_CC) -o $(ENV_BIN_PATH)/$@ $(addprefix $(ENV_BUILD_PATH)/, $(ENV_OBJECTS)) $(ENV_LDFLAGS)
	
TestEnvironment.o: $(ENV_SRC_PATH)/TestEnvironment.c $(ENV_SRC_PATH)/TestEnvironment.h
	$(ENV_CC) -c $(ENV_CFLAGS) $< -o $(ENV_BUILD_PATH)/$@

RL_client_environment.o: $(ENV_RL_GLUE)/Network/Environment/RL_client_environment.c
	$(ENV_CC) -c $(ENV_CFLAGS) $< -o $(ENV_BUILD_PATH)/$@
	
# Give this a unique name so it doesn't conflict with other RL_network.o rules included by the main makefile 
# Alernatively we could just build it once.
RL_environment_netlib.o: $(ENV_RL_GLUE)/Network/RL_network.c
	$(ENV_CC) -c $(ENV_CFLAGS) $< -o $(ENV_BUILD_PATH)/$@


