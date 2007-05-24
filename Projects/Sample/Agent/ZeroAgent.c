#include <stdio.h>
#include <stdlib.h>
#include "ZeroAgent.h"

Action theAction;
Observation theObservation;

void agent_init(int argc, char** argv, Task_specification task_spec)
{
  int i = 0;
  fprintf(stderr, "agent_init called with parameters: ");
  for (i = 0; i < argc; ++i) fprintf(stderr, "%s ", argv[i]);
  fprintf(stderr, "\n");

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
