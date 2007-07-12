# This makefile is an example of how to compile an agent, an environment, an experiment
# and RL-Glue 2.0 into a single executable.

# Path to RL-Glue to find RL_glue and related files.
RL-GLUE = ../../RL-Glue

CC      = gcc  
CFLAGS  = -I$(RL-GLUE)/ -ansi -pedantic -Wall
LDFLAGS =

OBJECTS = SarsaAgent.o mines.o experiment.o

# Link our objects into RL_glue
RL_glue: RL_glue.o $(OBJECTS)
	$(CC) -o bin/$@ Build/RL_glue.o $(addprefix Build/, $(OBJECTS))

# Compile RL-Glue
RL_glue.o: $(RL-GLUE)/RL_glue.c $(RL-GLUE)/RL_glue.h
	$(CC) -c $(CFLAGS) $(RL-GLUE)/RL_glue.c -o Build/$@

# Compile our agent, environment, and experiment
$(OBJECTS): %.o: src/%.c
	$(CC) -c $(CFLAGS) $< -o Build/$@

clean:
	rm -fr Build/*.o
	rm -fr bin/RL_glue

tidy:
	rm -fr *.*~
	rm -fr *~
	rm -fr \#*
