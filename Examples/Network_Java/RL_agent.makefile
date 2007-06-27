RL-GLUE = ../../RL-Glue

RL_agent:
#	javac $(RL-GLUE)/Java/rlglue/*.java -d Build/ -cp $(RL-GLUE)/Java/
#	javac $(RL-GLUE)/Java/rlglue/Network/*.java -d Build/ -cp $(RL-GLUE)/Java/
	javac $(RL-GLUE)/Java/rlglue/Network/Agent/*.java -d Build/ -cp $(RL-GLUE)/Java/:src/
#	javac $(RL-GLUE)/Java/rlglue/Network/Environment/*.java -d Build/ -cp $(RL-GLUE)/Java/
#	javac $(RL-GLUE)/Java/rlglue/Network/Experiment/*.java -d Build/ -cp $(RL-GLUE)/Java/
