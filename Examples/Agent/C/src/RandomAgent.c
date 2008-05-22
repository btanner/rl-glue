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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include "RandomAgent.h"
#include "C_TaskSpec_Parser.h"

Action action;

int freeze = 0;
task_spec_struct tss;					/*declare task_spec_struct*/

void randomify(Action action);

void agent_init(const Task_specification task_spec)
{
  srand(0);/*seed the randomness*/
	srand48(0);
  
  assert (task_spec != 0);
  parse_task_spec(task_spec, &tss);		/*Parsing task_specification*/	

/*allocating memory for one Action*/
  action.numInts     =  1;//tss.num_discrete_action_dims;
  action.intArray    = (int*)malloc(sizeof(int)*action.numInts);
  action.numDoubles  = 0;//tss.num_continuous_action_dims;
  action.doubleArray = 0;
}

Action agent_start(Observation o) {
	randomify(action);
	/*ask(o,action);*/
	return action;
}

Action agent_step(double reward, Observation o) {
	randomify(action);
	return action;
}

void agent_end(double reward) {
}

void agent_cleanup() {
}

void agent_freeze() {
}

Message agent_message(const Message message) {
	return NULL;
}


double getValueForState(Observation theObservation) {
	return 0;
}
	

void randomify(Action action){
	int i;
	for(i=0;i<tss.num_discrete_action_dims;i++){
		action.intArray[i] = rand()%((int)tss.action_maxs[i]+1-(int)tss.action_mins[i]) + (int)tss.action_mins[i];
	}
	for(i=0;i<tss.num_continuous_action_dims;i++){
		action.doubleArray[i] = drand48()*(tss.action_maxs[i]-tss.action_mins[i]) + tss.action_mins[i];
	}
}
