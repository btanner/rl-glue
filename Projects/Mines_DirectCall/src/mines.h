#ifndef mines_h

#include <RL_common.h>

/* RL-Glue interface */
Task_specification env_init();
Observation env_start();
Reward_observation env_step(Action a);
void env_cleanup();
void env_set_state(State_key sk);
void env_set_random_seed(Random_seed_key rsk);
State_key env_get_State();
Random_seed_key env_get_random_seek();
Message env_message(const Message inMessage);

/* Mines utility functions */
void env_print(const char* header, RL_abstract_type* data);
int getPosition();
void getNextPosition(Action a);
Reward getReward();

#endif
