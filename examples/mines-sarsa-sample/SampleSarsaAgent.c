/* 
	Copyright (C) 2008, Brian Tanner

	Licensed under the Apache License, Version 2.0 (the "License");
	you may not use this file except in compliance with the License.
	You may obtain a copy of the License at

	    http://www.apache.org/licenses/LICENSE-2.0

	Unless required by applicable law or agreed to in writing, software
	distributed under the License is distributed on an "AS IS" BASIS,
	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
	See the License for the specific language governing permissions and
	limitations under the License.

	
	*  $Revision: 821 $
	*  $Date: 2008-09-14 14:21:41 -0600 (Sun, 14 Sep 2008) $
	*  $Author: brian@tannerpages.com $
	*  $HeadURL: https://rl-glue.googlecode.com/svn/trunk/src/RL_direct_agent.c $
	
*/

/* 	
	This is a very simple Sarsa agent for discrete-action, discrete-state
	environments.  It uses epsilon-greedy exploration.
	
	We've made a decision to store the previous action and observation in 
	their raw form, as structures.  This code could be simplified and you
	could store them just as ints.
*/


#include <stdio.h>  /* for printf */
#include <string.h> /* for strcmp */
#include <assert.h>
#include <rlglue/Agent_common.h> /* agent_ functions and RL-Glue types */
#include <rlglue/utils/C/RLStruct_util.h> /* helpful functions for structs */
#include <rlglue/utils/C/TaskSpec_Parser.h> /* task spec parser */


action_t this_action;
action_t last_action;

observation_t *last_observation=0;

double* value_function=0;
double sarsa_stepsize = 0.1;
double sarsa_epsilon = 0.1;
double sarsa_gamma = 1.0;
int numActions=0;
int numStates=0;

int policy_frozen=0;
int exploring_frozen=0;

/* Returns a random integer in [0,max] */
int randInRange(int max);
int egreedy(int theState);
int calculateArrayIndex(int theState, int theAction);
void save_value_function(const char *fileName);
void load_value_function(const char *fileName);

void agent_init(const char* task_spec)
{
	/*Struct to hold the parsed task spec*/
	taskspec_t ts;
	int decode_result = dec_taskspec( &ts, task_spec );
	assert(decode_result==0);
	
	/* Lots of assertions to make sure that we can handle this problem.  */
	assert(ts.num_int_obs==1);
	assert(ts.num_double_obs==0);
	assert(ts.int_obs[0].special_min==RV_NOTSPECIAL);
	assert(ts.int_obs[0].special_max==RV_NOTSPECIAL);
	numStates=ts.int_obs[0].max+1;

	assert(ts.num_int_act==1);
	assert(ts.num_double_act==0);
	assert(ts.int_act[0].special_min==RV_NOTSPECIAL);
	assert(ts.int_act[0].special_max==RV_NOTSPECIAL);
	numActions=ts.int_act[0].max+1;
	
	/*Here is where you might allocate storage for parameters (value function or policy, last action, last observation, etc)*/
	
	/*Here you would parse the task spec if you felt like it*/
	
	/*Allocate memory for a one-dimensional integer action using utility functions from RLStruct_util*/
	allocateRLStruct(&this_action,1,0,0);
	allocateRLStruct(&last_action,1,0,0);
	/* That is equivalent to:
			 this_action.numInts     =  1;
			 this_action.intArray    = (int*)calloc(1,sizeof(int));
			 this_action.numDoubles  = 0;
			 this_action.doubleArray = 0;
			 this_action.numChars    = 0;
			 this_action.charArray   = 0;
	*/

	/*Allocate memory for a one-dimensional integer observation using utility functions from RLStruct_util*/
	last_observation=allocateRLStructPointer(1,0,0);
	
	/*Later we will parse this from the task spec, but for now*/
	value_function=(double *)calloc(numActions*numStates,sizeof(double));
	
/*	{
		int s=0;
		int a=0;
		
		for(s=0;s<108;s++){
			for(a=0;a<4;a++){
				printf("%d %d %d\n",s,a,calculateArrayIndex(s,a));
			}
		}
		exit(1);
	}
	*/
}

const action_t *agent_start(const observation_t *this_observation) {
	int theIntAction=egreedy(this_observation->intArray[0]);
	this_action.intArray[0]=theIntAction;

	/* In a real action you might want to store the last observation and last action*/
	replaceRLStruct(&this_action, &last_action);
	replaceRLStruct(this_observation, last_observation);
	
	return &this_action;
}

const action_t *agent_step(double reward, const observation_t *this_observation) {
	int newState=this_observation->intArray[0];
	int lastState=last_observation->intArray[0];
	int lastAction=last_action.intArray[0];
	
	int newAction=egreedy(newState);
	
	double Q_sa=value_function[calculateArrayIndex(lastState,lastAction)];
	double Q_sprime_aprime=value_function[calculateArrayIndex(newState,newAction)];
	
	double new_Q_sa=Q_sa + sarsa_stepsize * (reward + sarsa_gamma * Q_sprime_aprime - Q_sa);
	/*	Only update the value function if the policy is not frozen */
	if(!policy_frozen){
		value_function[calculateArrayIndex(lastState,lastAction)]=new_Q_sa;
	}
	this_action.intArray[0]=newAction;
	
	replaceRLStruct(&this_action, &last_action);
	replaceRLStruct(this_observation, last_observation);
	
	return &this_action;
}

void agent_end(double reward) {
	int lastState=last_observation->intArray[0];
	int lastAction=last_action.intArray[0];
	
	double Q_sa=value_function[calculateArrayIndex(lastState,lastAction)];
	double new_Q_sa=Q_sa + sarsa_stepsize * (reward - Q_sa);

	/*	Only update the value function if the policy is not frozen */
	if(!policy_frozen){
		value_function[calculateArrayIndex(lastState,lastAction)]=new_Q_sa;
	}
	clearRLStruct(&last_action);
	clearRLStruct(last_observation);
}

void agent_cleanup() {
	clearRLStruct(&this_action);
	clearRLStruct(&last_action);
	freeRLStructPointer(last_observation);
	
	if(value_function!=0){
		free(value_function);
		value_function=0;
	}
}

const char* agent_message(const char* inMessage) {
	static char buffer[128];
	
	if(strcmp(inMessage,"freeze learning")==0){
		policy_frozen=1;
		return "message understood, policy frozen";
	}
	if(strcmp(inMessage,"unfreeze learning")==0){
		policy_frozen=0;
		return "message understood, policy unfrozen";
	}
	if(strcmp(inMessage,"freeze exploring")==0){
		exploring_frozen=1;
		return "message understood, exploring frozen";
	}
	if(strcmp(inMessage,"unfreeze exploring")==0){
		exploring_frozen=0;
		return "message understood, exploring unfrozen";
	}
	if(strncmp(inMessage,"save_policy",11)==0){
		strlcpy(buffer,inMessage+12,sizeof(buffer));
		printf("Saving value function...");
		save_value_function(buffer);
		printf("Saved.\n");
		return "message understood, saving policy";
	}
	if(strncmp(inMessage,"load_policy",11)==0){
		strlcpy(buffer,inMessage+12,sizeof(buffer));
		printf("Loading value function...");
		load_value_function(buffer);
		printf("Loaded.\n");
		return "message understood, loading policy";
	}

	
	return "SampleSarsaAgent does not understand your message.";
			
}

void save_value_function(const char *fileName){
	FILE *fp;
	fp=fopen(fileName, "wb");
	
	fwrite(value_function,sizeof(double),numStates*numActions,fp);
	fclose(fp);
}
void load_value_function(const char *fileName){
	FILE *fp;
	fp=fopen(fileName, "rb");
	
	fread(value_function,sizeof(double),numStates*numActions,fp);
	fclose(fp);
}

int egreedy(int state){
	int max = 0;
	int i = 1;
	int randFrequency=(int)(1.0f/sarsa_epsilon);

	if(!exploring_frozen){
  		if((rand() % randFrequency == 1)) {
    		return randInRange(numActions-1);
  		}
	}

/*otherwise choose the greedy action*/
  max = 0;
  for(i = 1; i < numActions; i++){
    if(value_function[calculateArrayIndex(state,i)] > value_function[calculateArrayIndex(state,max)]) {
      max = i;
    }
  }

  return max;
}

int randInRange(int max){
	double r, x;
	r = ((double)rand() / ((double)(RAND_MAX)+(double)(1)));
   	x = (r * (max+1));

	return (int)x;
}

int calculateArrayIndex(int theState, int theAction){
	assert(theState<numStates);
	assert(theAction<numActions);

	return theState*numActions+theAction;
}
