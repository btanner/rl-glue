#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "TestEnvironment.h"

Observation o;
Reward_observation ro;
int stepCount = 0;

Task_specification env_init()
{    

  o.numInts    = 1;
  o.numDoubles = 0;
  o.intArray    = (int*)calloc(1,sizeof(int));
  o.doubleArray = 0;

  return 0;
}

Observation env_start()
{ 
  stepCount = 0;
  o.intArray[0] = stepCount;
  return o;
}

Reward_observation env_step(Action a)
{  
  stepCount += 1;
  o.intArray[0] = stepCount;
  ro.r = stepCount;
  ro.o = o;

  if (a.intArray[0] == 100)
    ro.terminal = 1;
  else
    ro.terminal = 0;

  fprintf(stderr, "TestEnv.c: a=%d o=%d t=%d\n", a.intArray[0], o.intArray[0], ro.terminal);


  return ro;
}

void env_cleanup()
{
/*deallocate all memory and initialize values to 0*/
  free(o.intArray);
  free(o.doubleArray);

  o.intArray    = 0;
  o.doubleArray = 0;
}

void env_set_state(State_key sk)
{
}
     
void env_set_random_seed(Random_seed_key rsk)
{
}

State_key env_get_state()
{
  State_key theKey = {0};
  return theKey;
}

Random_seed_key env_get_random_seed()
{
  Random_seed_key theKey = {0};
  return theKey;
}

Message env_message(const Message inMessage) {
  return 0;
}
