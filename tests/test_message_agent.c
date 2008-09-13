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
*  $Revision: 809 $
*  $Date: 2008-09-11 22:56:21 -0600 (Thu, 11 Sep 2008) $
*  $Author: brian@tannerpages.com $
*  $HeadURL: https://rl-glue.googlecode.com/svn/trunk/tests/message_test_agent.c $
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
message_t agent_responseMessage=0;



void agent_init(const task_specification_t task_spec){
}

action_t agent_start(observation_t o) {
}

action_t agent_step(reward_t reward, observation_t o) {
}

void agent_end(reward_t reward) {
}

void agent_cleanup() {
}

void agent_freeze() {
}

message_t agent_message(const message_t inMessage) {
	char tmpBuffer[1024];
	
	if(inMessage==0)
		return "null";
	if(strcmp(inMessage,"")==0)
		return "empty";
	if(strcmp(inMessage,"null")==0)
		return 0;
	if(strcmp(inMessage,"empty")==0)
		return "";
	
	sprintf(tmpBuffer,"%s", inMessage);

	if(agent_responseMessage!=0){
		free(agent_responseMessage);
		agent_responseMessage=0;
	}
	agent_responseMessage=(char *)calloc(strlen(tmpBuffer),sizeof(char));
	sprintf(agent_responseMessage,"%s",tmpBuffer);
	return agent_responseMessage;
}
