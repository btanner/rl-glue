./RL_Glue &
java -cp ~/workspace/Java/bin:./ rlglue.agent.AgentLoader SarsaAgent &
java -cp ~/workspace/Java/bin:./ rlglue.environment.EnvironmentLoader Mines &
java -cp ~/workspace/Java/bin:./ Experiment
