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
void agent_init(const Task_specification task_spec);
Action agent_start(Observation o);
Action agent_step(Reward r, Observation o);
void agent_end(Reward r);  
void agent_cleanup();
Message agent_message(const Message message);

#ifdef __cplusplus
}
#endif
#endif
