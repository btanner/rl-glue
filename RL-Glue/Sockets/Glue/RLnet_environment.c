#ifdef NETWORK_DEBUG
#define RLNET_DEBUG(x) x
#else
#define RLNET_DEBUG(x)
#endif

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

static void print_reward(Reward theReward)
{
  fprintf(stderr, "GLUE SENT: theReward %f\n", theReward);
}

static void print_action_header(Action theAction)
{
  fprintf(stderr, "GLUE RECV: theAction.numInts = %d\n", theAction.numInts);
  fprintf(stderr, "GLUE RECV: theAction.numDoubles = %d\n", theAction.numDoubles);
}

static void print_observation_header(Observation theObservation)
{
  fprintf(stderr, "GLUE SENT: theObservation.numInts = %d\n", theObservation.numInts);
  fprintf(stderr, "GLUE SENT: theObservation.numDoubles = %d\n", theObservation.numDoubles);
}

static void send_msg(rlSocket theSocket, const char* theMessage)
{
  char send_buffer[8] = {0};
  strncpy(send_buffer, theMessage, 8);
  rlSendData(theSocket, send_buffer, 8);

  RLNET_DEBUG( fprintf(stderr, "GLUE SENT: %s\n", send_buffer); )
}

Task_specification env_init()
{
  int isValidServerSocket = 0;
  int isValidDataSocket = 0;
  int isListening = 0;
  int theTaskSpecLength = 0;
  rlSocket theServer = rlOpen(4097);

  isValidServerSocket = rlIsValidSocket(theServer);
  assert(rlIsValidSocket(theServer));

  isListening = rlListen(theServer);
  assert(isListening >= 0);

  theEnvironmentConnection = rlAcceptConnection(theServer);
  rlClose(theServer);

  isValidDataSocket = rlIsValidSocket(theEnvironmentConnection);
  assert(isValidDataSocket);

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
  RLNET_DEBUG( print_observation_header(theObservation) );


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
  RLNET_DEBUG( fprintf(stderr, "ENV SENT: %s\n", kEnvStep); )
  
  rlSendAction(theEnvironmentConnection, theAction);
  RLNET_DEBUG( print_action_header(theAction) );

  rlRecvObservationHeader(theEnvironmentConnection, &theObservation);
  RLNET_DEBUG( print_observation_header(theObservation) );

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
