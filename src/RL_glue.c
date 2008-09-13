/* 
* Copyright (C) 2007, Andrew Butcher

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

#include <rlglue/Experiment_common.h>
#include <rlglue/Agent_common.h>
#include <rlglue/Environment_common.h>

#include <assert.h>

/*Brian, Sept 8, 2008 :
---------------------
I'm a little worried that some of my changes in this file 
(using stack variables for structs and returning them)
 might go terribly wrong*/

static action_t last_action  = {0};
static reward_t total_reward = 0;
static int num_steps       = 0;
static int num_episodes    = 0;


int __rlglue_check_abstract_type(const rl_abstract_type_t *theStruct){
	if(theStruct->numInts>1000){printf("abstract type integrity error: numInts = %d\n",theStruct->numInts);return 1;}
	if(theStruct->numDoubles>1000)return 2;
	if(theStruct->numChars>1000)return 3;

	if(theStruct->numInts>0 && theStruct->intArray==0)return 4;
	if(theStruct->numDoubles>0 && theStruct->doubleArray==0)return 5;
	if(theStruct->numChars>0 && theStruct->charArray==0)return 6;

	if(theStruct->numInts==0 && theStruct->intArray!=0)return 7;
	if(theStruct->numDoubles==0 && theStruct->doubleArray!=0)return 8;
	if(theStruct->numChars==0 && theStruct->charArray!=0)return 9;
	
	return 0;
}

/* Take ths out later */
void __rlglue_print_abstract_type(const rl_abstract_type_t *theStruct){
	int i;
		printf("Printing Abstract Type\n-----------------\n");
	printf("\t Ints: %d \t Doubles: %d\t Chars: %d\n",theStruct->numInts, theStruct->numDoubles, theStruct->numChars);
	if(theStruct->numInts>0)assert(theStruct->intArray!=0);
	if(theStruct->numDoubles>0)assert(theStruct->doubleArray!=0);
	if(theStruct->numChars>0)assert(theStruct->charArray!=0);
	
	if(theStruct->numInts<100){
		printf("Ints: ");
		for(i=0;i<theStruct->numInts;i++)
			printf("\t%d",theStruct->intArray[i]);
	}
	printf("\n");
	if(theStruct->numDoubles<100){
		printf("Doubles: ");
		for(i=0;i<theStruct->numDoubles;i++)
			printf("\t%f",theStruct->doubleArray[i]);
	}
	printf("\n");
	if(theStruct->numChars<100){
		printf("Chars: ");
		for(i=0;i<theStruct->numChars;i++)
			printf("\t%c",theStruct->charArray[i]);
	}
	printf("\n");
}



task_specification_t RL_init() {
  task_specification_t task_spec;
  task_spec = env_init();
  agent_init(task_spec);

  total_reward=0;
  num_steps=0;
  num_episodes=0;
/* **WORRYSOME** */
	return task_spec;
}

observation_action_t RL_start() {
	observation_t last_state;
	observation_action_t oa;

	total_reward=0;
	num_steps=1;

	last_state = env_start();
	last_action = agent_start(last_state);

	oa.o = last_state;
	oa.a = last_action;

/* **WORRYSOME** */
	return oa;
}

reward_observation_action_terminal_t RL_step() {
	reward_observation_action_terminal_t roa={0};
	reward_observation_t ro={0};
  	reward_t this_reward=0;
	observation_t last_state={0};

	__RL_CHECK_STRUCT(&last_action)
  	ro = env_step(last_action);
	__RL_CHECK_STRUCT(&ro.o)
  	this_reward = ro.r;
  	last_state = ro.o;
  
  	roa.r = ro.r;
  	roa.o = ro.o;
  	roa.terminal = ro.terminal;
  
  	total_reward += this_reward;

    num_steps += 1; /* increment num_steps if state is not terminal <-- why is that (Sept 11/08 changed to increment always)*/
	 if (ro.terminal == 1) {
	   num_episodes += 1;
	   agent_end(this_reward);
	 }
	 else {
	   last_action = agent_step(this_reward,last_state);
	   __RL_CHECK_STRUCT(&last_action)
	   roa.a = last_action;
	 }

/* **WORRYSOME** */
	 return roa;
}

void RL_cleanup() {
  env_cleanup();
  agent_cleanup();
}

message_t RL_agent_message(const message_t message) {
	char *theAgentResponse;
	char *messageToSend=message;
	
	/* I think this is safe because nobody will try to free stuff that is sent to them */
  if(messageToSend==0)
	 messageToSend="";
	
  theAgentResponse=agent_message(messageToSend);
  if(theAgentResponse==0)
	theAgentResponse="";
	
  return theAgentResponse;
}

message_t RL_env_message(const message_t message) {
	char *theEnvResponse;
	char *messageToSend=message;

	/* I think this is safe because nobody will try to free stuff that is sent to them */
  if(messageToSend==0)
	 messageToSend="";

  theEnvResponse=env_message(messageToSend);
  if(theEnvResponse==0)
	theEnvResponse="";

  return theEnvResponse;
}

terminal_t RL_episode(unsigned int maxStepsThisEpisode) {
	reward_observation_action_terminal_t rl_step_result={0};
	rl_step_result.terminal=0;	
  	unsigned int x = 0;
  	RL_start();
/* RL_start sets current step to 1, so we should start x at 1 */
  	for ( x = 1; !rl_step_result.terminal && (maxStepsThisEpisode == 0 ? 1 : x < maxStepsThisEpisode); ++x ) {
    	rl_step_result=RL_step();
  	}

	/*Return the value of terminal to tell the caller whether the episode ended naturally or was cut off*/
	return rl_step_result.terminal;
}

reward_t RL_return() {
  return total_reward;
}

int RL_num_steps() {
  /* number of steps of the current or just completed episodes of run */
  return num_steps;
}

int RL_num_episodes() {
  return num_episodes;
}

state_key_t RL_get_state() {
  return env_get_state();
}

void RL_set_state(state_key_t sk) {
  env_set_state(sk);
}

void RL_set_random_seed(random_seed_key_t rsk) {
  env_set_random_seed(rsk);
}

random_seed_key_t RL_get_random_seed() {
  return env_get_random_seed();
}
