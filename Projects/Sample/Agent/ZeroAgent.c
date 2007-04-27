#include <stdlib.h>
#include "ZeroAgent.h"

Action theAction;
Observation theObservation;

void agent_init(Task_specification task_spec)
{
  theAction.numInts = 1;
  theAction.intArray = (int*)malloc(sizeof(int) * theAction.numInts);
}

Action agent_start(Observation o)
{
  theObservation = o;
  theAction.intArray[0] = rand();
  return theAction;
}

Action agent_step(Reward r, Observation o)
{
  theObservation = o;
  theAction.intArray[0] = rand();
  return theAction;
}

void agent_end(Reward r)
{
}

void agent_cleanup()
{
}
