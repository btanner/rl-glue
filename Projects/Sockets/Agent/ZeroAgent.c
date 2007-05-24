#include <stdlib.h>
#include <stdio.h>
#include "ZeroAgent.h"

Action theAction;
Observation theObservation;

void agent_init(int argc, char** argv, Task_specification task_spec)
{
  int i = 0;
  fprintf(stderr, "agent_init called with parameters: ");
  for (i = 0; i < argc; ++i) fprintf(stderr, "%s ", argv[i]);
  fprintf(stderr, "\n");

  theAction.numInts = 10;
  theAction.numDoubles = 10;
  theAction.intArray = (int*)calloc(theAction.numInts, sizeof(int));
  theAction.doubleArray = (double*)calloc(theAction.numDoubles, sizeof(double));
}

Action agent_start(Observation o)
{
  int i = 0;
  theObservation = o;

  for (i = 0; i < 10; ++i)
  {
    theAction.intArray[i] = i;
    theAction.doubleArray[i] = i + 10;
  }

  return theAction;
}

Action agent_step(Reward r, Observation o)
{
  int i = 0;

  theObservation = o;

  for (i = 0; i < 10; ++i)
  {
    theAction.intArray[i] = i;
    theAction.doubleArray[i] = i + 10;
  }

  return theAction;
}

void agent_end(Reward r)
{
}

void agent_cleanup()
{
  free (theAction.intArray);
  free (theAction.doubleArray);
}
