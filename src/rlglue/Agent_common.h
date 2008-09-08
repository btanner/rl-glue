/* 
* Copyright (C) 2007, Matt Radkie

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

#ifndef Agent_common_h
#define Agent_common_h

#include <rlglue/RL_common.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Agent Interface */
void agent_init(const task_specification_t task_spec);
action_t agent_start(observation_t o);
action_t agent_step(reward_t r, observation_t o);
void agent_end(reward_t r);  
void agent_cleanup();
message_t agent_message(const message_t message);

#ifdef __cplusplus
}
#endif
#endif
