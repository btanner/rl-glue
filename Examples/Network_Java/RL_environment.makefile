RL-GLUE = ../../RL-Glue

RL_environment:
	javac $(RL-GLUE)/Java/rlglue/*.java -d Build/ -cp $(RL-GLUE)/Java/
	javac $(RL-GLUE)/Java/Environment/EnvironmentLoader.java -d Build/ -cp $(RL-GLUE)/Java/:src/
	javac src/DefaultEnvironment.java -d Build/ -cp $(RL-GLUE)/Java/
