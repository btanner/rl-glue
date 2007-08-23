RL-GLUE-CLASSPATH = /Users/andrewbutcher/workspace
SRC_PATH = src
BIN_PATH = bin

RL_agent:
	javac $(SRC_PATH)/SarsaAgent.java -d $(BIN_PATH)/ -cp $(RL-GLUE-CLASSPATH)/Java/bin/
