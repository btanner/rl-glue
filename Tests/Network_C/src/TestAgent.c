#include <stdlib.h>
#include <stdio.h>
#include "TestAgent.h"

Action action;
int stepCount = 0;

void agent_init(const Task_specification task_spec)
{  
  action.numInts     = 1;
  action.intArray    = (int*)calloc(1,sizeof(int));
  action.numDoubles  = 0;
  action.doubleArray = 0;
}

Action agent_start(Observation o)
{
  stepCount = 0;
  action.intArray[0] = stepCount;
  fprintf(stderr, "TestAgt.c: a=%d o=%d\n", action.intArray[0], o.intArray[0]);
  return action;	
}


Action agent_step(Reward r, Observation o)
{
  stepCount += 1;
  action.intArray[0] = stepCount;
  fprintf(stderr, "TestAgt.c: a=%d o=%d\n", action.intArray[0], o.intArray[0]);
  return action;	
}

void agent_end(Reward r)
{ 
}

void agent_cleanup(){
  free(action.intArray);
  action.numInts     = 0;
  action.numDoubles  = 0;
  action.intArray    = 0;
  action.doubleArray = 0;
}

Message agent_message(const Message message)
{
  return 0;
}

void agent_freeze()
{
}
