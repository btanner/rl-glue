RL-GLUE = ../../RL-Glue
SRC_PATH = src
BIN_PATH = bin

RL_experiment:
	javac $(SRC_PATH)/Experiment.java -d $(BIN_PATH)/ -cp $(RL-GLUE)/Java/RL-Glue.jar
