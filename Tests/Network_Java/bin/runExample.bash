./RL_Glue &
java -cp ../../../RL-Glue/Java/RL-Glue.jar:./ rlglue.agent.AgentLoader SarsaAgent &
java -cp ../../../RL-Glue/Java/RL-Glue.jar:./ rlglue.environment.EnvironmentLoader Mines &
java -cp ../../../RL-Glue/Java/RL-Glue.jar:./ Experiment
