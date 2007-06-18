#ifndef RL_interface_h
#define RL_interface_h

#include <RL_common.h>

/* Glue */
void RL_init();
Observation_action RL_start();
Reward_observation_action_terminal RL_step();
void RL_cleanup();

Message RL_agent_message(Message message);
Message RL_env_message(Message message);

Reward RL_return();
int RL_num_steps();
int RL_num_episodes();
void RL_episode(unsigned int num_steps);
/* void RL_episode(); */
void RL_freeze();
void RL_set_state(State_key sk);
void RL_set_random_seed(Random_seed_key rsk);
State_key RL_get_state();
Random_seed_key RL_get_random_seed();

/* Agent Interface */
void agent_init(Task_specification task_spec);
Action agent_start(Observation o);
Action agent_step(Reward r, Observation o);
void agent_end(Reward r);  
void agent_cleanup();
void agent_freeze();
Message agent_message(const Message message);

/* Environment Interface */
Task_specification env_init();
Observation env_start();
Reward_observation env_step(Action a);
void env_cleanup();
void env_set_state(State_key sk);
void env_set_random_seed(Random_seed_key rsk);
State_key env_get_state();
Random_seed_key env_get_random_seed();
Message env_message(const Message message);

#endif
