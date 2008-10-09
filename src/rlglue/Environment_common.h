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

* 
*  $Revision$
*  $Date$
*  $Author$
*  $HeadURL$
* 
*/


#ifndef Environment_common_h
#define Environment_common_h

#ifdef __cplusplus
extern "C" {
#endif

/*	* Environment Interface 
	*
	* This should be included by all C/C++ environments as #include <rlglue/Environment_common.h>
	* Environments must implement all of these functions.
*/
#include <rlglue/RL_common.h>

	/* Environment Interface */
	const char* env_init();
	observation_t env_start();
	reward_observation_t env_step(action_t a);
	void env_cleanup();
	void env_set_state(state_key_t sk);
	void env_set_random_seed(random_seed_key_t rsk);
	state_key_t env_get_state();
	random_seed_key_t env_get_random_seed();
	const char* env_message(const char * message);

#ifdef __cplusplus
}
#endif

#endif
