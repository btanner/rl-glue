all: RL_glue RL_agent RL_environment
include RL_glue.makefile
include RL_agent.makefile
include RL_environment.makefile
include RL_netlib.makefile

clean:
	rm -f RL_glue
	rm -f RL_agent
	rm -f RL_environment
	rm -f Build/*.o
