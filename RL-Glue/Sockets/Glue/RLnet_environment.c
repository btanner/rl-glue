#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <RLcommon.h>
#include <RLnet/RLnet.h>

/* Could be culled, used only for debugging */
#include <stdio.h>

static rlSocket theEnvironmentConnection;
static Observation theObservation;
static Task_specification theTaskSpecBuffer;
static int isAllocated;

const char* kEnvInit = "init";
const char* kEnvStart= "start";
const char* kEnvStep = "step";
const char* kEnvCleanup = "cleanup";
const char* kEnvSetStateKey = "setsk";
const char* kEnvSetRandomSeed = "setrs";
const char* kEnvGetStateKey = "getsk";
const char* kEnvGetRandomSeed = "getrs";

static void send_msg(rlSocket theSocket, const char* theMessage)
{
  char send_buffer[8] = {0};
  strncpy(send_buffer, theMessage, 8);
  rlSendData(theSocket, send_buffer, 8);
}

Task_specification env_init(int argc, char** argv)
{
  int theTaskSpecLength = 0;
  rlSocket theServer;
  int isValidSocket;
  int isListening;

  theServer = rlOpen(4097);
  isValidSocket = rlIsValidSocket(theServer);
  assert(isValidSocket);

  isListening = rlListen(theServer);
  assert(isListening >= 0);

  theEnvironmentConnection = rlAcceptConnection(theServer);
  isValidSocket = rlIsValidSocket(theEnvironmentConnection);
  assert(isValidSocket);

  rlClose(theServer);

  send_msg(theEnvironmentConnection, kEnvInit);

  rlRecvMessageHeader(theEnvironmentConnection, &theTaskSpecLength);

  theTaskSpecBuffer = (char*)calloc(theTaskSpecLength, 1);
  rlRecvMessageBody(theEnvironmentConnection, theTaskSpecBuffer, theTaskSpecLength);

  isAllocated = 0;

  return theTaskSpecBuffer;
}

Observation env_start()
{
  send_msg(theEnvironmentConnection, kEnvStart);
  rlRecvObservationHeader(theEnvironmentConnection, &theObservation);

  if (isAllocated == 0)
  {
    if (theObservation.numInts > 0)
      theObservation.intArray = (int*)calloc(theObservation.numInts, sizeof(int));
    
    if (theObservation.numDoubles > 0)
      theObservation.doubleArray = (double*)calloc(theObservation.numDoubles, sizeof(double));
    
    isAllocated = 1;
  }

  rlRecvObservationBody(theEnvironmentConnection, &theObservation);

  return theObservation;
}

Reward_observation env_step(Action theAction)
{
  Reward_observation theRewardObservation;

  send_msg(theEnvironmentConnection, kEnvStep);  
  rlSendAction(theEnvironmentConnection, theAction);
  rlRecvObservationHeader(theEnvironmentConnection, &theObservation);
  rlRecvReward(theEnvironmentConnection, &theRewardObservation.r);
  rlRecvObservationBody(theEnvironmentConnection, &theObservation);
  rlRecvTerminal(theEnvironmentConnection, &theRewardObservation.terminal);

  theRewardObservation.o = theObservation;

  return theRewardObservation;
}

void env_cleanup()
{
  send_msg(theEnvironmentConnection, kEnvCleanup);
  free(theObservation.intArray);
  free(theObservation.doubleArray);
  free(theTaskSpecBuffer);
}

void env_set_state(State_key sk)
{
}

void env_set_random_seed(Random_seed_key rsk)
{
}

State_key env_get_state()
{
  State_key theStateKey;
  return theStateKey;
}

Random_seed_key env_get_random_seed()
{
  Random_seed_key theRandomSeedKey;
  return theRandomSeedKey;
}
