/* 
* Copyright (C) 2007, Brian Tanner

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


/**
This agent is used for testing.  It will mostly just return whatever it receives.

This agent doesn't implement all the methods.. isn't that bad?
**/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <rlglue/Agent_common.h>

#include "useful_functions.h"


message_t agent_responseMessage=0;
action_t emptyAction;
action_t nonEmptyAction;
int agent_whichEpisode=0;

void agent_init(const task_specification_t task_spec){
	clean_abstract_type(&emptyAction);
	clean_abstract_type(&nonEmptyAction);
	
	set_k_ints_in_abstract_type(&nonEmptyAction,7);
	set_k_doubles_in_abstract_type(&nonEmptyAction,3);
	set_k_chars_in_abstract_type(&nonEmptyAction,1);

	agent_whichEpisode=0;
}

action_t agent_start(observation_t o) {
	agent_whichEpisode++;
	
	if(agent_whichEpisode%2==0)
		return emptyAction;
	
	return nonEmptyAction;
}

action_t agent_step(reward_t reward, observation_t o) {
	if(agent_whichEpisode%2==0)
		return emptyAction;
	
	return nonEmptyAction;
}

void agent_end(reward_t reward) {
}

void agent_cleanup() {
	clean_abstract_type(&emptyAction);
	clean_abstract_type(&nonEmptyAction);
}

void agent_freeze() {
}

message_t agent_message(const message_t inMessage) {
	return "";
}
