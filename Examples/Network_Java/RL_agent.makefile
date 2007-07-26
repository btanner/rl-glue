RL-GLUE = ../../RL-Glue
SRC_PATH = src
BIN_PATH = bin

RL_agent:
	# The Agent we want to compile
	javac $(SRC_PATH)/SarsaAgent.java -d $(BIN_PATH)/ -cp $(RL-GLUE)/Java/

	# RL-Glue
	javac $(RL-GLUE)/Java/Agent/AgentLoader.java -d $(BIN_PATH)/ -cp $(RL-GLUE)/Java/:src/
	javac $(RL-GLUE)/Java/rlglue/Action.java -d $(BIN_PATH)/ -cp $(RL-GLUE)/Java/
	javac $(RL-GLUE)/Java/rlglue/Agent.java -d $(BIN_PATH)/ -cp $(RL-GLUE)/Java/
	javac $(RL-GLUE)/Java/rlglue/ClientAgent.java -d $(BIN_PATH)/ -cp $(RL-GLUE)/Java/
	javac $(RL-GLUE)/Java/rlglue/Network.java -d $(BIN_PATH)/ -cp $(RL-GLUE)/Java/
	javac $(RL-GLUE)/Java/rlglue/Observation.java -d $(BIN_PATH)/ -cp $(RL-GLUE)/Java/
	javac $(RL-GLUE)/Java/rlglue/Random_seed_key.java -d $(BIN_PATH)/ -cp $(RL-GLUE)/Java/
	javac $(RL-GLUE)/Java/rlglue/State_key.java -d $(BIN_PATH)/ -cp $(RL-GLUE)/Java/
