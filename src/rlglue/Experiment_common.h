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

* 
*  $Revision$
*  $Date$
*  $Author$
*  $HeadURL$
* 
*/


#ifndef RL_interface_h
#define RL_interface_h

#ifdef __cplusplus
extern "C" {
#endif


#include <rlglue/RL_common.h>

/* Glue */
task_specification_t RL_init();
observation_action_t RL_start();
reward_observation_action_terminal_t RL_step();
void RL_cleanup();

message_t RL_agent_message(message_t message);
message_t RL_env_message(message_t message);

reward_t RL_return();
int RL_num_steps();
int RL_num_episodes();
terminal_t RL_episode(unsigned int num_steps);
void RL_set_state(state_key_t sk);
void RL_set_random_seed(random_seed_key_t rsk);
state_key_t RL_get_state();
random_seed_key_t RL_get_random_seed();

#ifdef __cplusplus
}
#endif

#endif
