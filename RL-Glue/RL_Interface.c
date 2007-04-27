#include <stdio.h>
#include "RL_Interface.h"

Action last_action;
Observation last_state;
Reward last_reward;

int isTerminal;

int num_steps;
Reward total_reward;
int total_steps;
int num_episodes;

Reward_observation_action_terminal roa;
Observation_action oa;

void RL_init(int argc, char** argv)
{
  Task_specification task_spec;
  task_spec = env_init();
  agent_init(task_spec);
	
  num_episodes = 0;
  total_steps = 0;
}

Observation_action RL_start()
{
  last_state = env_start();
  last_action = agent_start(last_state);

  num_steps = 1;
  total_reward = 0;
  isTerminal = 0;
  
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
    num_steps++; /* increment num_steps if state is not terminal */
    
    roa.a = last_action;
  }
  return roa;
}

/*
void RL_episode()
{
  RL_start();
  while (!isTerminal)
  {
    RL_step();
  }
}
*/

void RL_episode(int num_Steps)
{
  int x = 0;
  RL_start();
  for (x=0; x<num_Steps && !isTerminal; ++x)
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
  return (Reward)(total_reward/(double)num_steps);
}

double RL_average_num_steps()
{
  /* average number of steps per episode (only counts completed episodes (not current))	*/
  return ((double)total_steps)/((double)num_episodes);
}

int RL_num_steps()
{
  /* number of steps of the current or just completed episodes or run */
  return num_steps;
}

int RL_num_episodes()
{
  return num_episodes;
}

void RL_cleanup()
{
  env_cleanup();
  agent_cleanup();
}

State_key RL_get_state()
{
  return env_get_state();
}

Random_seed_key RL_get_random_seed()
{
  return env_get_random_seed();
}

void RL_set_state(State_key sk)
{
  env_set_state(sk);
}

void RL_set_random_seed(Random_seed_key rsk)
{
  env_set_random_seed(rsk);
}
