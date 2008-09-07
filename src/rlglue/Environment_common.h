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

#ifndef Environment_common_h
#define Environment_common_h

#ifdef __cplusplus
extern "C" {
#endif


#include <rlglue/RL_common.h>

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

#ifdef __cplusplus
}
#endif

#endif
