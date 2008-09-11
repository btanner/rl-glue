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
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <rlglue/Environment_common.h>
#include "useful_functions.h"


observation_t o={0};
reward_observation_t ro={0};
message_t env_responseMessage=0;
int stepCount=0;

task_specification_t env_init()
{    
	return "sample task spec";
}

observation_t env_start()
{
	stepCount=0;
	clean_abstract_type(&o);
	makeKInts(&o,1);
	return o;
}

reward_observation_t env_step(action_t a)
{
	clean_abstract_type(&o);
	makeKInts(&o,1);
	
	ro.o=o;
	ro.r=1.0;
  
	stepCount++;
	
	ro.terminal=stepCount==5;
  return ro;
}

void env_cleanup()
{
  clean_abstract_type(&o);
}

void env_set_state(state_key_t sk)
{
}
     
void env_set_random_seed(random_seed_key_t rsk)
{
}

state_key_t env_get_state()
{
  state_key_t theKey;
  return theKey;
}

random_seed_key_t env_get_random_seed()
{
  random_seed_key_t theKey;
  return theKey;
}

message_t env_message(const message_t inMessage) {
	int timesToPrint=stepCount%3;
	int i;
	char tmpBuffer[1024];
	
	sprintf(tmpBuffer,"%s|",inMessage);
	for(i=0;i<timesToPrint;i++){
		sprintf(tmpBuffer,"%s%d.", tmpBuffer,stepCount);
	}
	sprintf(tmpBuffer,"%s|%s",tmpBuffer,inMessage);
	
	if(env_responseMessage!=0){
		free(env_responseMessage);
		env_responseMessage=0;
	}
	env_responseMessage=(char *)calloc(strlen(tmpBuffer),sizeof(char));
	sprintf(env_responseMessage,"%s",tmpBuffer);
	return env_responseMessage;
}
