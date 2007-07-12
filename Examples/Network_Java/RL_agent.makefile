RL-GLUE = ../../RL-Glue
SRC_PATH = src
BIN_PATH = bin

RL_agent:
	javac $(RL-GLUE)/Java/rlglue/*.java -d $(BIN_PATH)/ -cp $(RL-GLUE)/Java/
	javac $(RL-GLUE)/Java/Agent/AgentLoader.java -d $(BIN_PATH)/ -cp $(RL-GLUE)/Java/:src/
	javac $(SRC_PATH)/SarsaAgent.java -d $(BIN_PATH)/ -cp $(RL-GLUE)/Java/
