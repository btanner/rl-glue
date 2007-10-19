/* 
* Copyright (C) 2007, Brian Tanner
* 
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (at your option) any later version.
* 
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* 
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include "Glue_utilities.h"
#include "RandomAgent.h"

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
  action.numInts     =  tss.num_discrete_action_dims;
  action.intArray    = (int*)malloc(sizeof(int)*action.numInts);
  action.numDoubles  = tss.num_continuous_action_dims;
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
