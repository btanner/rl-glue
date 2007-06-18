# Agent Source Files
AGENTC = ./Agent/ZeroAgent.c
AGENTH = ./Agent/ZeroAgent.h

# Environment Source Files
ENVIRONMENTC = ./Environment/mines.c
ENVIRONMENTH = ./Environment/mines.h

# Experiment Source Files
EXPERIMENTC = ./Experiment/experiment.c
EXPERIMENTH = 

# Path to RL-Glue to find RL_glue and related files.
RL-GLUE = ../../RL-Glue

# Compiler flags
CC      = gcc
CFLAGS  = -I$(RL-GLUE)/ -ansi -pedantic -Wall
LDFLAGS =

# An example of RL-Glue compiled together in C
all: agent.o environment.o experiment.o glue.o
	$(CC) -o RL_main Build/agent.o Build/environment.o Build/experiment.o Build/glue.o

# Dependencies
agent.o: $(AGENTC) $(AGENTH)
	$(CC) -c $(CFLAGS) $(AGENTC) -o Build/agent.o

environment.o: $(ENVIRONMENTC) $(ENVIRONMENTH)
	$(CC) -c $(CFLAGS) $(ENVIRONMENTC) -o Build/environment.o

experiment.o: $(EXPERIMENTC) $(EXPERIMENTH)
	$(CC) -c $(CFLAGS) $(EXPERIMENTC) -o Build/experiment.o

glue.o: $(RL-GLUE)/RL_glue.c $(RL-GLUE)/RL_glue.h
	$(CC) -c $(CFLAGS) $(RL-GLUE)/RL_glue.c -o Build/glue.o

clean:
	rm -fr Build/*.o

tidy:
	rm -fr *.*~
	rm -fr *~
	rm -fr \#*