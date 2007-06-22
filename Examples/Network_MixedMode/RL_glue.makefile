# Build RL-Glue so that each part will be communicating via network communication

RL-GLUE = ../../RL-Glue
BUILD_PATH = Build

CC      = gcc  
CFLAGS  = -I$(RL-GLUE)/ -ansi -pedantic -Wall
LDFLAGS =

GLUE_OBJECTS = RL_glue.o RL_network.o RL_netlib.o RL_server_agent.o RL_server_environment.o experiment.o

# Link everything together
RL_glue: $(GLUE_OBJECTS)
	$(CC) -o $@ $(addprefix $(BUILD_PATH)/, $(GLUE_OBJECTS))

# Build the main interface. 
RL_glue.o: $(RL-GLUE)/RL_glue.c
	$(CC) $(CFLAGS) -c $< -o $(BUILD_PATH)/$@

# Build the network stuff
RL_network.o: $(RL-GLUE)/Network/RL_network.c
	$(CC) $(CFLAGS) -c $< -o $(BUILD_PATH)/$@

# Build more network stuff
RL_netlib.o: $(RL-GLUE)/Network/RL_netlib.c
	$(CC) $(CFLAGS) -c $< -o $(BUILD_PATH)/$@

# Compile the thing that make the Glue think its talking to an agent when its really talking over a socket
# If you wanted to compile the agent into the glue, you wouldn't need to compile this, or link to it. Instead
# you would just compile and link in your agent	
RL_server_agent.o: $(RL-GLUE)/Network/Glue/RL_server_agent.c 
	$(CC) $(CFLAGS) -c $< -o $(BUILD_PATH)/$@

# Compile the thing that make the Glue think its talking to an environment when its really talking over a socket
# If you wanted to compile the environment into the glue, you wouldn't need to compile this, or link to it. Instead
# you would just compile and link in your environment	
RL_server_environment.o: $(RL-GLUE)/Network/Glue/RL_server_environment.c 
	$(CC) $(CFLAGS) -c $< -o $(BUILD_PATH)/$@

experiment.o: ./src/experiment.c .
	$(CC) -c $(CFLAGS) $< -o $(BUILD_PATH)/$@
