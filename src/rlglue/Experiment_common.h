/* 
* Copyright (C) 2007, Adam White

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 */

#ifndef RL_interface_h
#define RL_interface_h

#ifdef __cplusplus
extern "C" {
#endif


#include <rlglue/RL_common.h>

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
void RL_freeze();
void RL_set_state(State_key sk);
void RL_set_random_seed(Random_seed_key rsk);
State_key RL_get_state();
Random_seed_key RL_get_random_seed();

#ifdef __cplusplus
}
#endif

#endif
