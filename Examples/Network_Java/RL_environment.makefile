RL-GLUE = ../../RL-Glue
SRC_PATH = src
BIN_PATH = bin

RL_environment:	
	# The Environment we want to compile
	javac $(SRC_PATH)/Mines.java -d $(BIN_PATH)/ -cp $(RL-GLUE)/Java/

	# RL-Glue
	javac $(RL-GLUE)/Java/Environment/EnvironmentLoader.java -d $(BIN_PATH)/ -cp $(RL-GLUE)/Java/:src/
	javac $(RL-GLUE)/Java/rlglue/Action.java -d $(BIN_PATH)/ -cp $(RL-GLUE)/Java/
	javac $(RL-GLUE)/Java/rlglue/ClientEnvironment.java -d $(BIN_PATH)/ -cp $(RL-GLUE)/Java/
	javac $(RL-GLUE)/Java/rlglue/Environment.java -d $(BIN_PATH)/ -cp $(RL-GLUE)/Java/
	javac $(RL-GLUE)/Java/rlglue/Network.java -d $(BIN_PATH)/ -cp $(RL-GLUE)/Java/
	javac $(RL-GLUE)/Java/rlglue/Observation.java -d $(BIN_PATH)/ -cp $(RL-GLUE)/Java/
