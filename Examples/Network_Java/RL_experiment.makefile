RL-GLUE = ../../RL-Glue
SRC_PATH = src
BIN_PATH = bin

RL_experiment:
	javac $(RL-GLUE)/Java/rlglue/*.java -d $(BIN_PATH)/ -cp $(RL-GLUE)/Java/
	javac $(SRC_PATH)/Experiment.java -d $(BIN_PATH)/ -cp $(RL-GLUE)/Java/
