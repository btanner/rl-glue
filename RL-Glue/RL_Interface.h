#ifndef RL_Interface_h
#define RL_Interface_h

#include <stdarg.h>
#include "RLcommon.h"

/* Glue */
void RL_init(int argc, char** argv);
Observation_action RL_start();
Reward_observation_action_terminal RL_step();
Reward RL_return();
Reward RL_average_reward();
double RL_average_num_steps();
int RL_num_steps();
int RL_num_episodes();

/* void RL_episode(); */
void RL_episode(int num_steps);
void RL_set_state(State_key sk);
void RL_set_random_seed(Random_seed_key rsk);
State_key RL_get_state();
Random_seed_key RL_get_random_seed();
void RL_cleanup();

/* Agent Interface */
void agent_init(int argc, char** argv, Task_specification task_spec);
Action agent_start(Observation o);
Action agent_step(Reward r, Observation o);
void agent_end(Reward r);  
void agent_cleanup();

/* Environment Interface */
Task_specification env_init(int argc, char** argv);
Observation env_start();
Reward_observation env_step(Action a);
void env_cleanup();
void env_set_state(State_key sk);
void env_set_random_seed(Random_seed_key rsk);
State_key env_get_state();
Random_seed_key env_get_random_seed();

#endif
