RL-GLUE = ../../RL-Glue
SRC_PATH = src
BIN_PATH = bin

RL_environment:	
	javac $(SRC_PATH)/Mines.java -d $(BIN_PATH)/ -cp $(RL-GLUE)/Java/RL-Glue.jar
