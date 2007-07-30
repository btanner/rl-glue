RL-GLUE = ../../RL-Glue
SRC_PATH = src
BIN_PATH = bin

RL_agent:
	javac $(SRC_PATH)/SarsaAgent.java -d $(BIN_PATH)/ -cp $(RL-GLUE)/Java/RL-Glue.jar
