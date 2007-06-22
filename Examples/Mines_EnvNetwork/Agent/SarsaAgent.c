#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "SarsaAgent.h"
#include "Glue_utilities.h"

#define NUM_STATES 114
#define NUM_ACTIONS 4

Action action;
Action previous_action;
Observation previous_observation;
double value[NUM_STATES][NUM_ACTIONS];
double sarsa_alpha = 0.1;
double sarsa_gamma = 0.9;
int freeze = 0;

void agent_init(const Task_specification task_spec)
{
  task_spec_struct tss;

  srand(time(NULL));

	printf("task_spec is: %s\n", task_spec);
  
  parse_task_spec(task_spec, &tss);

  action.numInts     =  tss.num_discrete_action_dims;
  action.intArray    = (int*)malloc(sizeof(int)*action.numInts);
  action.numDoubles  = tss.num_continuous_action_dims;
  action.doubleArray = 0;

  previous_action.numInts     = tss.num_discrete_action_dims;
  previous_action.intArray    = (int*)malloc(sizeof(int)*previous_action.numInts);
  previous_action.numDoubles  = tss.num_continuous_action_dims;
  previous_action.doubleArray = 0;

  previous_observation.numInts     = tss.num_discrete_obs_dims;
  previous_observation.intArray    = (int*)malloc(sizeof(int)*previous_observation.numInts);
  previous_observation.numDoubles  = tss.num_continuous_obs_dims;
  previous_observation.doubleArray = 0;

  memset(value, 0, sizeof(double)*114*4);
}

Action agent_start(Observation o)
{	
  unsigned int i = 0;
  action.intArray[0] = egreedy(o);

  /* We need to copy the action if we want to retain the values in it */
  for(i = 0; i < action.numInts; ++i) { 
    previous_action.intArray[i] = action.intArray[i];
  }

  /* We need to copy the observation if we want to retain the values in it */
  for(i=0; i< o.numInts; ++i) {
    previous_observation.intArray[i] = o.intArray[i];
  }

  return action;	
}


Action agent_step(Reward r, Observation o)
{
  unsigned int i = 0;
  double oldQ = 0; 
  double newQ = 0; 

  const int newState = o.intArray[0];
  const int oldState = previous_observation.intArray[0];

  const int oldAction  = previous_action.intArray[0];
  const int newAction  = egreedy(o);

  action.intArray[0] = newAction;

  oldQ = value[oldState][oldAction];
  newQ = value[newState][newAction];

 if(!freeze) {
   value[oldState][oldAction] = oldQ + sarsa_alpha*(r + sarsa_gamma*newQ - oldQ);

   for(i = 0; i < action.numInts; ++i) {
     previous_action.intArray[i] = action.intArray[i];
   }
   
   for(i = 0; i < o.numInts; ++i) {
     previous_observation.intArray[i] = o.intArray[i];
   }
 }

 return action;	
}


void agent_end(Reward r)
{ 
  const int oldState = previous_observation.intArray[0];
  const int oldAction = previous_action.intArray[0];
  
  double oldQ = value[oldState][oldAction];
  value[oldState][oldAction] = oldQ + sarsa_alpha*(r - oldQ);
}


int egreedy(Observation o){
  int state = o.intArray[0];
  int max = 0;
  int i = 1;

  if((freeze == 0) && (rand() % 10 == 1)) {
    return rand() % 4;	
  }

  max = 0;
  for(i = 1; i < 4; i++){
    if(value[state][i] > value[state][max]) {
      max = i;
    }
  }

  return max;
}

void agent_cleanup(){
  free(action.intArray);
  free(previous_action.intArray);
  free(previous_observation.intArray);

  action.numInts     = 0;
  action.numDoubles  = 0;
  action.intArray    = 0;
  action.doubleArray = 0;

  previous_action.numInts     = 0;
  previous_action.numDoubles  = 0;
  previous_action.intArray    = 0;
  previous_action.doubleArray = 0;
}

Message agent_message(const Message message){
  return "SarsaAgent.c does not respond to any messages.";
}

void agent_freeze(){
  freeze = 1;
}
