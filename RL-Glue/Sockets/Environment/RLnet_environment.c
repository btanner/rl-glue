#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include <RLcommon.h>
#include <RLnet/RLnet.h>

#ifdef NETWORK_DEBUG
#define RLNET_DEBUG(x) x
#else
#define RLNET_DEBUG(x)
#endif

const char* kEnvInit = "init";
const char* kEnvStart= "start";
const char* kEnvStep = "step";
const char* kEnvCleanup = "cleanup";
const char* kEnvSetStateKey = "setsk";
const char* kEnvSetRandomSeed = "setrs";
const char* kEnvGetStateKey = "getsk";
const char* kEnvGetRandomSeed = "getrs";

/* Declare the task spec and length */
unsigned int theTaskSpecLength;
char* theTaskSpecBuffer;

/* Declare observation action and reward */
Action theAction;
int isActionAllocated = 0;

/* Provide forward declaration of environment interface */
extern Task_specification env_init();
extern Observation env_start();
extern Reward_observation env_step(Action a);
extern void env_cleanup();
extern void env_set_state(State_key sk);
extern void env_set_random_seed(Random_seed_key rsk);
extern State_key env_get_state();
extern Random_seed_key env_get_random_seed();

const char* kUnknownMessage = "Unknown Message: %s\n";
const char* kUsage = "Usage: Environment <ip-address> <port>\n";

void print_reward(Reward theReward)
{
  fprintf(stderr, "ENV SENT: theReward %f\n", theReward);
}

void print_action_header(Action theAction)
{
  fprintf(stderr, "ENV RECV: theAction.numInts = %d\n", theAction.numInts);
  fprintf(stderr, "ENV RECV: theAction.numDoubles = %d\n", theAction.numDoubles);
}

void print_observation_header(Observation theObservation)
{
  fprintf(stderr, "ENV SENT: theObservation.numInts = %d\n", theObservation.numInts);
  fprintf(stderr, "ENV SENT: theObservation.numDoubles = %d\n", theObservation.numDoubles);
}

void on_env_init(rlSocket theConnection)
{
  /* Task_specification env_init(); */  
  theTaskSpecBuffer = env_init();
  theTaskSpecLength = strlen(theTaskSpecBuffer) + 1;

  rlSendMessageHeader(theConnection, theTaskSpecLength);
  rlSendMessageBody(theConnection, theTaskSpecBuffer, theTaskSpecLength);
}

void on_env_start(rlSocket theConnection)
{
  /* Observation env_start(); */
  Observation theObservation = env_start();
  rlSendObservation(theConnection, theObservation);

  isActionAllocated = 0;
}

void on_env_step(rlSocket theConnection)
{
  /* Reward_observation env_step(Action a); */
  Reward_observation ro;

  /* need to deal with allocation / dealloction of the action! */
  rlRecvActionHeader(theConnection, &theAction);

  if (isActionAllocated == 0)
  {
    if (theAction.numInts > 0)
      theAction.intArray = (int*)calloc(theAction.numInts, sizeof(int));

    if (theAction.numDoubles > 0)
      theAction.doubleArray = (double*)calloc(theAction.numDoubles, sizeof(double));
    isActionAllocated = 1;
  }

  rlRecvActionBody(theConnection, &theAction);
  
  ro = env_step(theAction);
  rlSendRewardObservation(theConnection, ro);
}

void on_env_cleanup(rlSocket theConnection)
{
  env_cleanup();

  free(theAction.intArray);
  free(theAction.doubleArray);
}

void on_env_set_state(rlSocket theConnection)
{
  /* void env_set_state(State_key sk); */
}

void on_env_set_random_seed(rlSocket theConnection)
{
  /* void env_set_random_seed(Random_seed_key rsk); */
}

void on_env_get_state(rlSocket theConnection)
{
  /* State_key env_get_state(); */
}

void on_env_get_random_seed(rlSocket theConnection)
{
  /* Random_seed_key env_get_random_seed(); */
}

void run_env(rlSocket theConnection)
{
  char theMessage[8] = {0};

  do
  { 
    rlRecvData(theConnection, theMessage, 8);
    RLNET_DEBUG( fprintf(stderr, "ENV RECV: %s\n", theMessage); )
	      
    if (strncmp(theMessage, kEnvInit, 8) == 0)
    {
      on_env_init(theConnection);
    }
    else if (strncmp(theMessage, kEnvStart, 8) == 0)
    {
      on_env_start(theConnection);
    }
    else if (strncmp(theMessage, kEnvStep, 8) == 0)
    {
      on_env_step(theConnection);
    }
    else if (strncmp(theMessage, kEnvCleanup, 8) == 0)
    {
      on_env_cleanup(theConnection);
    }
    else if ( strncmp(theMessage, kEnvSetStateKey, 8) == 0)
    {
      on_env_set_state(theConnection);
    }
    else if ( strncmp(theMessage, kEnvSetRandomSeed, 8) == 0)
    {
      on_env_set_random_seed(theConnection);
    }
    else if ( strncmp(theMessage, kEnvGetStateKey, 8) == 0)
    {
      on_env_get_state(theConnection);
    }
    else if ( strncmp(theMessage, kEnvGetRandomSeed, 8) == 0)
    {
      on_env_get_random_seed(theConnection);
    }
    else
    {
      fprintf(stderr, kUnknownMessage, theMessage);
      break;
    }
  } while (strncmp(theMessage, kEnvCleanup, 8) != 0);
}

int main(int argc, char** argv)
{
  rlSocket theConnection;
  int theResult = 0;
  short thePort = 0;

  if (argc != 3) 
  {
    fprintf(stderr, kUsage);
    return 1;
  }

  sscanf(argv[2], "%hd", &thePort);

  theConnection = rlOpen(thePort);
  theResult = rlIsValidSocket(theConnection);
  assert(theResult != 0);

  theResult = rlConnect(theConnection, argv[1]);
  assert(theResult >= 0);

  run_env(theConnection);

  theResult = rlClose(theConnection);
  assert(theResult == 0);

  return 0;
}
