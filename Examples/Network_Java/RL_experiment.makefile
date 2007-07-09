RL-GLUE = ../../RL-Glue

RL_experiment:
	javac $(RL-GLUE)/Java/rlglue/*.java -d Build/ -cp $(RL-GLUE)/Java/
	javac src/Experiment.java -d Build/ -cp $(RL-GLUE)/Java/
