RL-GLUE = ../../RL-Glue

RL_agent:
	javac $(RL-GLUE)/Java/rlglue/*.java -d Build/ -cp $(RL-GLUE)/Java/
	javac $(RL-GLUE)/Java/Agent/AgentLoader.java -d Build/ -cp $(RL-GLUE)/Java/:src/
	javac src/SarsaAgent.java -d Build/ -cp $(RL-GLUE)/Java/
