# Path to RL-Glue to find RL_interface and related files.
RL-GLUE = ../../RL-Glue

# Compiler flags
CC         = gcc
CFLAGS     = -I$(RL-GLUE)/ -ansi -pedantic -Wall
LDFLAGS    =
BUILD_PATH = Build/

# Object files and libraries required for linking our agent
# Simply leave these blank if you are not using a C agent.
AGENT_OBJECTS       = ZeroAgent.o
AGENT_LDFLAGS       =

# Object files and libraries required for linking our environment
# Simply leave these blank if you are not using a C environment
ENVIRONMENT_OBJECTS = mines.o
ENVIRONMENT_LDFLAGS =

# Object files and libraries required for linking our experiment
# Simply leave these blank if you are not using a C experiment
EXPERIMENT_OBJECTS  = experiment.o
EXPERIMENT_LDFLAGS  =

# Uncomment next line to use network agent
AGENT_NET       = yes
ENVIRONMENT_NET = yes
EXPERIMENT_NET  = yes

all: ZeroAgent.o mines.o experiment.o RL_GLUE

ZeroAgent.o: ./Agent/ZeroAgent.c ./Agent/ZeroAgent.h
	$(CC) $(CFLAGS) -c $< -o $(addprefix $(BUILD_PATH), $@)

mines.o: ./Environment/mines.c ./Environment/mines.h
	$(CC) $(CFLAGS) -c $< -o $(addprefix $(BUILD_PATH), $@)

experiment.o: ./Experiment/experiment.c
	$(CC) $(CFLAGS) -c $< -o $(addprefix $(BUILD_PATH), $@)

# ============================================================================================
#                                     RLGLUE 2.0
# ============================================================================================
GLUE_AGENT        = RL_direct_agent.o $(AGENT_OBJECTS)
GLUE_ENVIRONMENT  = RL_direct_environment.o $(ENVIRONMENT_OBJECTS)
GLUE_EXPERIMENT   = RL_direct_experiment.o $(EXPERIMENT_OBJECTS)


AGENT_TARGET      = 
ifdef AGENT_NET
GLUE_AGENT        = RL_server_agent.o
USE_NETWORK       = yes
ifdef AGENT_OBJECTS
AGENT_TARGET      = RL_agent
endif
endif

ENVIRONMENT_TARGET =
ifdef ENVIRONMENT_NET
GLUE_ENVIRONMENT   = RL_server_environment.o 
USE_NETWORK        = yes
ifdef ENVIRONMENT_OBJECTS
ENVIRONMENT_TARGET = RL_environment
endif
endif

EXPERIMENT_TARGET = 
ifdef EXPERIMENT_NET
GLUE_EXPERIMENT   = RL_server_experiment.o
USE_NETWORK       = yes
ifdef EXPERIMENT_OBJECTS
EXPERIMENT_TARGET = RL_experiment
endif
endif

ifdef USE_NETWORK
NETWORK_OBJECT = RL_network.o
NETLIB_OBJECT  = RL_netlib.o
else
NETWORK_OBJECT =
NETLIB_OBJECT  =  
endif


RL_GLUE: RL_glue $(AGENT_TARGET) $(ENVIRONMENT_TARGET) $(EXPERIMENT_TARGET)

# ------------------------------- Compile And Link RL-Glue 2.0 ------------------------------
RL_glue: RL_glue.o $(NETWORK_OBJECT) $(NETLIB_OBJECT) $(GLUE_EXPERIMENT) $(GLUE_ENVIRONMENT) $(GLUE_AGENT) 
	$(CC) $(LDFLAGS) -o $@  $(addprefix $(BUILD_PATH), RL_glue.o)      \
				$(addprefix $(BUILD_PATH), $(NETWORK_OBJECT))   \
				$(addprefix $(BUILD_PATH), $(NETLIB_OBJECT))    \
				$(addprefix $(BUILD_PATH), $(GLUE_EXPERIMENT))  \
				$(addprefix $(BUILD_PATH), $(GLUE_ENVIRONMENT)) \
				$(addprefix $(BUILD_PATH), $(GLUE_AGENT))
# -------------------------------------------------------------------------------------------

# ------------------------------ Compile And Link the Agent ---------------------------------
ifdef AGENT_OBJECTS
RL_agent: $(AGENT_OBJECTS) RL_client_agent.o RL_netlib.o
	$(CC) $(LDFLAGS) -o $@  $(addprefix $(BUILD_PATH), $(AGENT_OBJECTS)) $(addprefix $(BUILD_PATH), RL_client_agent.o) $(addprefix $(BUILD_PATH), RL_netlib.o) $(AGENT_LDFLAGS)
endif
# -------------------------------------------------------------------------------------------

# --------------------------- Compile And Link the Environment ------------------------------
ifdef ENVIRONMENT_OBJECTS
RL_environment: $(ENVIRONMENT_OBJECTS) RL_client_environment.o RL_netlib.o
	$(CC) $(LDFLAGS) -o $@  $(addprefix $(BUILD_PATH), $(ENVIRONMENT_OBJECTS)) $(addprefix $(BUILD_PATH), RL_client_environment.o) $(addprefix $(BUILD_PATH), RL_netlib.o)
endif
# -------------------------------------------------------------------------------------------

# --------------------------- Compile And Link the Experiment -------------------------------
ifdef EXPERIMENT_OBJECTS
RL_experiment: $(EXPERIMENT_OBJECTS) RL_client_experiment.o RL_netlib.o
	$(CC) $(LDFLAGS) -o $@  $(addprefix $(BUILD_PATH), $(EXPERIMENT_OBJECTS)) $(addprefix $(BUILD_PATH), RL_client_experiment.o) $(addprefix $(BUILD_PATH), RL_netlib.o)
endif
# -------------------------------------------------------------------------------------------


# ===================================== INTERFACE ============================================
# ------------------------------- Compile The RL-Interface ----------------------------------
RL_glue.o: $(RL-GLUE)/RL_glue.c $(RL-GLUE)/RL_glue.h
	$(CC) $(CFLAGS) -c $< -o $(addprefix $(BUILD_PATH), $@)
# -------------------------------------------------------------------------------------------


# ====================================== DIRECT =============================================
# ----------------------------------- Direct Call Objects -----------------------------------
RL_direct_glue.o: $(RL-GLUE)/Direct/Glue/RL_direct_glue.c
	$(CC) $(CFLAGS) -c $< -o $(addprefix $(BUILD_PATH), $@)

RL_direct_agent.o: $(RL-GLUE)/Direct/Agent/RL_direct_agent.c
	$(CC) $(CFLAGS) -c $< -o $(addprefix $(BUILD_PATH), $@)

RL_direct_environment.o: $(RL-GLUE)/Direct/Environment/RL_direct_environment.c
	$(CC) $(CFLAGS) -c $< -o $(addprefix $(BUILD_PATH), $@)

RL_direct_experiment.o: $(RL-GLUE)/Direct/Experiment/RL_direct_experiment.c
	$(CC) $(CFLAGS) -c $< -o $(addprefix $(BUILD_PATH), $@)
# -------------------------------------------------------------------------------------------

# ====================================== NETWORK ============================================
# -------------------------------------- Client ---------------------------------------------
RL_client_agent.o: $(RL-GLUE)/Network/Agent/RL_client_agent.c
	$(CC) $(CFLAGS) -c $< -o $(addprefix $(BUILD_PATH), $@)

RL_client_environment.o: $(RL-GLUE)/Network/Environment/RL_client_environment.c
	$(CC) $(CFLAGS) -c $< -o $(addprefix $(BUILD_PATH), $@)

RL_client_experiment.o: $(RL-GLUE)/Network/Experiment/RL_client_experiment.c
	$(CC) $(CFLAGS) -c $< -o $(addprefix $(BUILD_PATH), $@)
# -------------------------------------------------------------------------------------------

# -------------------------------------- Server ---------------------------------------------
RL_server_agent.o: $(RL-GLUE)/Network/Glue/RL_server_agent.c
	$(CC) $(CFLAGS) -c $< -o $(addprefix $(BUILD_PATH), $@)

RL_server_environment.o: $(RL-GLUE)/Network/Glue/RL_server_environment.c
	$(CC) $(CFLAGS) -c $< -o $(addprefix $(BUILD_PATH), $@)

RL_server_experiment.o: $(RL-GLUE)/Network/Glue/RL_server_experiment.c
	$(CC) $(CFLAGS) -c $< -o $(addprefix $(BUILD_PATH), $@)
# -------------------------------------------------------------------------------------------

# ------------------------------------ Network Library --------------------------------------
RL_network.o: $(RL-GLUE)/Network/RL_network.c
	$(CC) $(CFLAGS) -c $< -o $(addprefix $(BUILD_PATH), $@)

RL_netlib.o: $(RL-GLUE)/Network/RL_netlib.c $(RL-GLUE)/Network/RL_netlib.h
	$(CC) $(CFLAGS) -c $< -o $(addprefix $(BUILD_PATH), $@)
# -------------------------------------------------------------------------------------------


clean:
	rm -fr $(addprefix $(BUILD_PATH), *.o)
	rm -fr RL_main
	rm -fr RL_glue
	rm -fr RL_agent
	rm -fr RL_environment
	rm -fr RL_experiment

tidy:
	rm -fr *.*~
	rm -fr *~
	rm -fr \#*