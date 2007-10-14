#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include "Glue_utilities.h"
#include "SarsaAgent.h"

#define NUM_STATES 108
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
  task_spec_struct tss;					/*declare task_spec_struct*/
  srand(0);/*seed the randomness*/
  
  assert (task_spec != 0);
  parse_task_spec(task_spec, &tss);		/*Parsing task_specification*/	

/*allocating memory for one Action*/
  action.numInts     =  tss.num_discrete_action_dims;
  action.intArray    = (int*)malloc(sizeof(int)*action.numInts);
  action.numDoubles  = tss.num_continuous_action_dims;
  action.doubleArray = 0;

/*allocating memory for one Action*/
  previous_action.numInts     = tss.num_discrete_action_dims;
  previous_action.intArray    = (int*)malloc(sizeof(int)*previous_action.numInts);
  previous_action.numDoubles  = tss.num_continuous_action_dims;
  previous_action.doubleArray = 0;
  
/*allocating memory for one Observation*/
  previous_observation.numInts     = tss.num_discrete_obs_dims;
  previous_observation.intArray    = (int*)malloc(sizeof(int)*previous_observation.numInts);
  previous_observation.numDoubles  = tss.num_continuous_obs_dims;
  previous_observation.doubleArray = 0;

  free(tss.obs_types);
  free(tss.obs_mins);
  free(tss.obs_maxs);

  free(tss.action_types);
  free(tss.action_mins);
  free(tss.action_maxs);

  /*reset the value function*/
  memset(value, 0, sizeof(double)*NUM_STATES*NUM_ACTIONS);
}

Action agent_start(Observation o)
{	
  unsigned int i = 0;
    
  /*pick the next action. Note: the policy is frozen internally. We haven't written an egreedy
  *and a greedy method like in sample_agent.c The epsilon is removed within the egreedy method if
  *the policy is frozen*/
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
  
    
  /*pick the next action. Note: the policy is frozen internally. We haven't written an egreedy
  *and a greedy method like in sample_agent.c The epsilon is removed within the egreedy method if
  *the policy is frozen*/
  const int newAction  = egreedy(o);

  action.intArray[0] = newAction;

  oldQ = value[oldState][oldAction];
  newQ = value[newState][newAction];


/*if we haven't frozen the agent, we should improve our value function with each step*/
 if(!freeze) {
   value[oldState][oldAction] = oldQ + sarsa_alpha*(r + sarsa_gamma*newQ - oldQ);

/* We need to copy the action if we want to retain the values in it */
   for(i = 0; i < action.numInts; ++i) {
     previous_action.intArray[i] = action.intArray[i];
   }
   
/* We need to copy the Observation if we want to retain the values in it */   
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
    
    const double oldQ = value[oldState][oldAction];

/*if the agent isn't frozen, do the last update to the value function, in sarsa this
*means the currentQ is zero for this calculation*/
    if(!freeze) {
      value[oldState][oldAction] = oldQ + sarsa_alpha*(r - oldQ);
    }
}

void agent_cleanup(){
/*free all the memory*/
  free(action.intArray);
  free(previous_action.intArray);
  free(previous_observation.intArray);

/*clear all values in the actions*/
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
  /*no messages currently implemented*/
  return "This agent does not respond to any messages.";
}

void agent_freeze(){
  /*sets the agent to freeze mode*/
  freeze = 1;
}

int egreedy(Observation o){
  int state = o.intArray[0];
  int max = 0;
  int i = 1;


/*If NOT frozen, with epsilon = 0.1 probability, act randomly*/
  if((!freeze) && (rand() % 10 == 1)) {
    return rand() % 4;
  }
/*else choose the greedy action*/
  max = 0;
  for(i = 1; i < 4; i++){
    if(value[state][i] > value[state][max]) {
      max = i;
    }
  }

  return max;
}
