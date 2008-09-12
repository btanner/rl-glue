/* 
* Copyright (C) 2008, Brian Tanner

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

#ifndef legacy_types_h
#define legacy_types_h
#include <rlglue/RL_common.h>

/*
This file defines all of the types that were present in RL_Glue 2.x.

You should really be using the new ones (lower case and suffixed with _t) from 
RL_common.h, but if you don't want to change all your code, you can include this file
and your old code will work.
*/

#ifdef __cplusplus
extern "C" {
#endif

	typedef task_specification_t Task_specification;
	typedef reward_t Reward;
	typedef terminal_t Terminal;
	typedef rl_abstract_type_t RL_abstract_type;
	typedef observation_t Observation;
	typedef action_t Action;
	typedef random_seed_key_t Random_seed_key;
	typedef state_key_t State_key;
	typedef message_t Message;
	typedef observation_action_t Observation_action;
	typedef reward_observation_t Reward_observation;
	typedef reward_observation_action_terminal_t Reward_observation_action_terminal;


#ifdef __cplusplus
}
#endif

#endif