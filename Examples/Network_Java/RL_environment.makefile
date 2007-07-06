RL-GLUE = ../../RL-Glue

RL_environment:
	javac $(RL-GLUE)/Java/rlglue/*.java -d Build/ -cp $(RL-GLUE)/Java/
	javac $(RL-GLUE)/Java/rlglue/Network/*.java -d Build/ -cp $(RL-GLUE)/Java/
	javac $(RL-GLUE)/Java/rlglue/Network/Environment/*.java -d Build/ -cp $(RL-GLUE)/Java/ 
	javac $(RL-GLUE)/Java/Environment/EnvironmentMain.java -d Build/ -cp $(RL-GLUE)/Java/:src/
	javac src/DefaultEnvironment.java -d Build/ -cp $(RL-GLUE)/Java/
