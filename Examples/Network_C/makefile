all: RL_glue RL_environment RL_experiment RL_agent

include RL_glue.makefile
include RL_agent.makefile
include RL_environment.makefile
include RL_experiment.makefile

clean:
	rm -f RL_glue
	rm -f RL_agent
	rm -f RL_environment
	rm -f RL_experiment
	rm -f Build/*.o
