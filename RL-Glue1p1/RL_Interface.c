#include "RL_Interface.h"

#ifndef RL_USE_ALL

#ifndef ENV_STATE
State_key env_get_state()
{
	printf("\nError:: env_get_state must be defined in your enviornment c file before you can call RL_get_state\n\n");
        exit(1);
	State_key sk;
	return sk;

}

void env_set_state(State_key sk)
{
	printf("\nError:: env_set_state must be defined in your environment c file before you can call RL_set_state\n\n");
        exit(1);
}
#endif

#ifndef ENV_RANDOM
Random_seed_key env_get_random_seed()
{
	printf("\nError:: env_get_random_seed must be defined in your environment c file before you can call RL_get_random_seed\n\n");
	exit(1);        
	Random_seed_key rsk;
	return rsk;
}

void env_set_random_seed(Random_seed_key rsk)
{
	printf("\nError:: env_set_random must be defined in your environment c file before you can call RL_set_random_seed\n\n");
	exit(1);
}
#endif

#ifndef ENV_CLEANUP
void env_cleanup()
{}	
#endif

#ifndef AGENT_CLEANUP
void agent_cleanup()
{}
#endif

#ifndef AGENT_INIT
void agent_init(char* ts)
{}
#endif
	
#ifndef ENV_INIT
char* env_init()
{
	return NULL;
}
#endif

#endif

Action last_action;
Observation last_state;
Reward last_reward;

bool isTerminal;


int num_steps;
Reward total_reward;
int total_steps;
int num_episodes;

char* RLts;

Reward_observation_action_terminal roa;
Observation_action oa;

#ifndef PIPES
void RL_init()
{
	Task_specification task_spec;
	task_spec = env_init();
	agent_init(task_spec);
	
	num_episodes = 0;
	total_steps = 0;
	RLts = task_spec;
}
#else
void RL_init(char* inpipe, char* outpipe)
{
	Task_specification task_spec;

	task_spec = env_init(inpipe,outpipe);
	agent_init(task_spec,inpipe,outpipe);
	
	num_episodes = 0;
	total_steps = 0;
	RLts = task_spec;

}
void RL_init()
{
	Task_specification task_spec;

	task_spec = env_init();
	agent_init(task_spec);
	
	num_episodes = 0;
	total_steps = 0;
	RLts = task_spec;

}
#endif

Observation_action RL_start()
{
	last_state = env_start();
	last_action = agent_start(last_state);
	num_steps = 1;
	total_reward = 0;
	isTerminal = false;
	
	oa.o = last_state;
	oa.a = last_action;
	
	return oa;
}

Reward_observation_action_terminal RL_step()
{
	Reward_observation ro;
	ro = env_step(last_action);
	last_reward = ro.r;
	last_state = ro.o;
	
	roa.r = ro.r;
	roa.o = ro.o;
	roa.terminal = ro.terminal;
	
	isTerminal = ro.terminal;
	total_reward = total_reward + last_reward;
	if (ro.terminal)
	{
		num_episodes++;
		total_steps += num_steps;
		agent_end(last_reward);
	}
	else
	{
		last_action = agent_step(last_reward,last_state);
        num_steps++; //increment num_steps if state is not terminal
    
		roa.a = last_action;
	}
	return roa;
}

State_key RL_get_state()
{
	return env_get_state();
}

Random_seed_key RL_get_random_seed()
{
	return env_get_random_seed();
}

#ifdef PIPES
void RL_set_state(char* sk)
{
	env_set_state(sk);
}
void RL_set_random_seed(char* rsk)
{
	env_set_random_seed(rsk);
}

void RL_set_state(int sk)
{
	char buf[10000];
	buf[0] = '\0';
	sprintf(buf," %d ",sk);
	RL_set_state(buf);
}

void RL_set_state(float sk)
{
	char buf[10000];
	buf[0] = '\0';
	sprintf(buf," %lf ",sk);
	RL_set_state(buf);
}

void RL_set_state(std::vector <int> &sk)
{
	char buf[10000];
	char top[10000];
	top[0] = '\0';	
	for(int i=0; i<sk.size(); i++)
	{
		buf[0] = '\0';	
		sprintf(buf," %d ",sk[i]);
		strcat(top,buf);
	}
	RL_set_state(buf);
}

void RL_set_state(std::vector <float> &sk)
{
	char buf[10000];
	char top[10000];
	top[0] = '\0';	
	for(int i=0; i<sk.size(); i++)
	{
		buf[0] = '\0';	
		sprintf(buf," %lf ",sk[i]);
		strcat(top,buf);
	}
	RL_set_state(buf);
}

void RL_set_random_seed(int rsk)
{
	char buf[10000];
	buf[0] = '\0';
	sprintf(buf," %d ",rsk);
	RL_set_random_seed(buf);
}

void RL_set_random_seed(float rsk)
{
	char buf[10000];
	buf[0] = '\0';
	sprintf(buf," %lf ",rsk);
	RL_set_random_seed(buf);
}

void RL_set_random_seed(std::vector <int> &rsk)
{
	char buf[10000];
	char top[10000];
	top[0] = '\0';	
	for(int i=0; i<rsk.size(); i++)
	{
		buf[0] = '\0';	
		sprintf(buf," %d ",rsk[i]);
		strcat(top,buf);
	}
	RL_set_state(buf);
}

void RL_set_random_seed(std::vector <float> &rsk)
{
	char buf[10000];
	char top[10000];
	top[0] = '\0';	
	for(int i=0; i<rsk.size(); i++)
	{
		buf[0] = '\0';	
		sprintf(buf," %lf ",rsk[i]);
		strcat(top,buf);
	}
	RL_set_state(buf);
}

#else

void RL_set_state(State_key sk)
{
	env_set_state(sk);
}
void RL_set_random_seed(Random_seed_key rsk)
{
	env_set_random_seed(rsk);
}
#endif

void RL_episode()
{
	RL_start();
	while (!isTerminal)
	{
		RL_step();
	}
}

void RL_episode(int num_Steps)
{
	RL_start();
	for (int x=0; x<num_Steps && !isTerminal; ++x)
	{
		RL_step();
	}
}

Reward RL_return()
{
	return total_reward;
}

Reward RL_average_reward()
{
    return (Reward)(total_reward/double(num_steps));
}

double RL_average_num_steps()
{
	//average number of steps per episode (only counts completed episodes (not current))
	
	return ((double)total_steps)/((double)num_episodes);
}

int RL_num_steps()
{

    //number of steps of the current or just completed episodes or run
    return num_steps;
}

int RL_num_episodes()
{
	return num_episodes;
}

char* RL_get_spec()
{
	return RLts;
}

void RL_cleanup()
{
	env_cleanup();
	agent_cleanup();
}
