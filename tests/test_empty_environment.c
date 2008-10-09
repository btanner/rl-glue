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
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif


#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <rlglue/Environment_common.h>

#include "useful_functions.h"

message_t env_responseMessage=0;
observation_t emptyObservation;
observation_t nonEmptyObservation;


int env_whichEpisode=0;


task_specification_t env_init()
{    
	env_whichEpisode=0;

	clean_abstract_type(&emptyObservation);
	clean_abstract_type(&nonEmptyObservation);

	set_k_ints_in_abstract_type(&nonEmptyObservation,2);
	set_k_doubles_in_abstract_type(&nonEmptyObservation,4);
	set_k_chars_in_abstract_type(&nonEmptyObservation,5);

	return "";
}

observation_t env_start()
{
	env_whichEpisode++;
	
	if(env_whichEpisode%2==0)
		return emptyObservation;

	return nonEmptyObservation;
}

reward_observation_t env_step(action_t a)
{
	reward_observation_t ro={0};

	if(env_whichEpisode%2==0)
		ro.o=emptyObservation;
	else
		ro.o=nonEmptyObservation;
		
	ro.r=0;
	ro.terminal=0;
	
	return ro;
}

void env_cleanup()
{
	clean_abstract_type(&emptyObservation);
	clean_abstract_type(&nonEmptyObservation);
}

void env_set_state(state_key_t sk)
{
}
     
void env_set_random_seed(random_seed_key_t rsk)
{
}

state_key_t env_get_state()
{
	state_key_t theKey={0};
	clean_abstract_type(&theKey);
	return theKey;
}

random_seed_key_t env_get_random_seed()
{
	random_seed_key_t theKey={0};
	clean_abstract_type(&theKey);
	return theKey;
}

message_t env_message(const message_t inMessage) {
	return "";
}
	
