#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include <RLcommon.h>
#include <RLnet/RLnet.h>

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
extern Task_specification env_init(int argc, char** argv);
extern Observation env_start();
extern Reward_observation env_step(Action a);
extern void env_cleanup();
extern void env_set_state(State_key sk);
extern void env_set_random_seed(Random_seed_key rsk);
extern State_key env_get_state();
extern Random_seed_key env_get_random_seed();

const char* kUnknownMessage = "Unknown Message: %s\n";
const char* kUsage = "Usage: Environment <ip-address> <port>\n";

void on_env_init(int argc, char** argv, rlSocket theConnection)
{
  theTaskSpecBuffer = env_init(argc, argv);
  theTaskSpecLength = strlen(theTaskSpecBuffer) + 1;

  rlSendMessageHeader(theConnection, theTaskSpecLength);
  rlSendMessageBody(theConnection, theTaskSpecBuffer, theTaskSpecLength);
}

void on_env_start(rlSocket theConnection)
{
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

void run_environment(int argc, char** argv, rlSocket theConnection)
{
  char theMessage[8] = {0};

  do
  { 
    rlRecvData(theConnection, theMessage, 8);
	      
    if (strncmp(theMessage, kEnvInit, 8) == 0)
    {
      on_env_init(argc, argv, theConnection);
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

  int isValidSocket = 0;
  int isConnected = -1;
  int isClosed = 0;

  short thePort = 0;

  if (argc != 3) 
  {
    fprintf(stderr, kUsage);
    return 1;
  }

  sscanf(argv[2], "%hd", &thePort);

  while(1)
  {
    while(isConnected < 0)
    {
      theConnection = rlOpen(thePort);
      isValidSocket = rlIsValidSocket(theConnection);
      assert(isValidSocket);
      
      isConnected = rlConnect(theConnection, argv[1]);
      if (isConnected < 0) rlClose(theConnection); /* We need to try again */
    }

    run_environment(argc, argv, theConnection);
    
    isClosed = rlClose(theConnection);
    isConnected = -1;
    /* assert(isClosed >= 0); */
  }

  return 0;
}
