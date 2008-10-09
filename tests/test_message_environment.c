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

task_specification_t env_init()
{    
	return "";
}

observation_t env_start()
{
	observation_t o={0};
	clean_abstract_type(&o);
	return o;
}

reward_observation_t env_step(action_t a)
{
	reward_observation_t ro={0};
	clean_abstract_type(&ro.o);
	return ro;
}

void env_cleanup()
{

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
	char tmpBuffer[1024];
	
	if(inMessage==0)
		return "null";
	if(strcmp(inMessage,"")==0)
		return "empty";
	if(strcmp(inMessage,"null")==0)
		return 0;
	if(strcmp(inMessage,"empty")==0){
		return "";
		}
	sprintf(tmpBuffer,"%s", inMessage);

	if(env_responseMessage!=0){
		free(env_responseMessage);
		env_responseMessage=0;
	}
	env_responseMessage=(char *)calloc(strlen(tmpBuffer)+1,sizeof(char));
	sprintf(env_responseMessage,"%s",tmpBuffer);
	return env_responseMessage;
}
	
