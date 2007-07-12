RL-GLUE = ../../RL-Glue
SRC_PATH = src
BIN_PATH = bin

RL_environment:
	javac $(RL-GLUE)/Java/rlglue/*.java -d $(BIN_PATH)/ -cp $(RL-GLUE)/Java/
	javac $(RL-GLUE)/Java/Environment/EnvironmentLoader.java -d $(BIN_PATH)/ -cp $(RL-GLUE)/Java/:src/
	javac $(SRC_PATH)/Mines.java -d $(BIN_PATH)/ -cp $(RL-GLUE)/Java/
