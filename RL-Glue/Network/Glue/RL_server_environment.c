#include <stdlib.h> /* calloc */
#include <string.h> /* strlen */

#include <stdio.h> /* fprintf : Debug only! */

#include <RL_common.h>
#include <Network/RL_netlib.h>

extern int rlConnectSystems();

static rlSocket theEnvironmentConnection = 0;
static Task_specification theTaskSpec = 0;
static Observation theObservation       = {0};
static State_key theStateKey            = {0};
static Random_seed_key theRandomSeedKey = {0};
static rlBuffer theBuffer = {0};

void rlSetEnvironmentConnection(int theConnection) {
  if (theEnvironmentConnection) {
    rlClose(theEnvironmentConnection);
  }

  theEnvironmentConnection = theConnection;
}

int rlIsEnvironmentConnected() {
  return theEnvironmentConnection != 0;
}

Task_specification env_init() {
  const int envState = kEnvInit;
  int theTaskSpecLength = 0;
  int offset = 0;

  rlConnectSystems();

  rlBufferCreate(&theBuffer, 4096);

  rlBufferClear(&theBuffer);
  rlBufferWrite(&theBuffer, 0, &envState, 1, sizeof(int));
  rlSendBufferData(theEnvironmentConnection, &theBuffer);

  rlBufferClear(&theBuffer);
  rlRecvBufferData(theEnvironmentConnection, &theBuffer);
  offset = rlBufferRead(&theBuffer, offset, &theTaskSpecLength, 1, sizeof(int));  
  if (theTaskSpecLength > 0) {
    theTaskSpec = (char*)calloc(theTaskSpecLength, sizeof(char));
    offset = rlBufferRead(&theBuffer, offset, theTaskSpec, theTaskSpecLength, sizeof(char));
  }
  return theTaskSpec;
}

Observation env_start() {
  const int envState = kEnvStart;

  rlBufferClear(&theBuffer);
  rlBufferWrite(&theBuffer, 0, &envState, 1, sizeof(int));
  rlSendBufferData(theEnvironmentConnection, &theBuffer);

  rlBufferClear(&theBuffer);
  rlRecvBufferData(theEnvironmentConnection, &theBuffer);
  rlCopyBufferToADT(&theBuffer, &theObservation);
  return theObservation;
}

Reward_observation env_step(Action theAction) {
  const int envState = kEnvStep;
  Reward_observation ro = {0};
  int offset = 0;

  rlBufferClear(&theBuffer);
  rlBufferWrite(&theBuffer, 0, &envState, 1, sizeof(int));
  rlSendBufferData(theEnvironmentConnection, &theBuffer);

  rlBufferClear(&theBuffer);
  rlCopyADTToBuffer(&theAction, &theBuffer);
  rlSendBufferData(theEnvironmentConnection, &theBuffer);

  rlBufferClear(&theBuffer);
  rlRecvBufferData(theEnvironmentConnection, &theBuffer);
  offset = rlBufferRead(&theBuffer, offset, &ro.terminal, 1, sizeof(int));
  offset = rlBufferRead(&theBuffer, offset, &ro.r, 1, sizeof(Reward));

  rlBufferClear(&theBuffer);
  rlRecvBufferData(theEnvironmentConnection, &theBuffer);
  rlCopyBufferToADT(&theBuffer, &theObservation);

  ro.o = theObservation;
  return ro;
}

void env_cleanup() {
  const int envState = kEnvCleanup;

  rlBufferClear(&theBuffer);
  rlBufferWrite(&theBuffer, 0, &envState, 1, sizeof(int));
  rlSendBufferData(theEnvironmentConnection, &theBuffer);

  rlClose(theEnvironmentConnection);
  theEnvironmentConnection = 0;

  free(theObservation.intArray);
  free(theObservation.doubleArray);
  free(theStateKey.intArray);
  free(theStateKey.doubleArray);
  free(theRandomSeedKey.intArray);
  free(theRandomSeedKey.doubleArray);

  theObservation.intArray = 0;
  theObservation.doubleArray = 0;
  theStateKey.intArray = 0;
  theStateKey.doubleArray = 0;
  theRandomSeedKey.intArray = 0;
  theRandomSeedKey.doubleArray = 0;

  theObservation.numInts = 0;
  theObservation.numDoubles = 0;
  theStateKey.numInts = 0;
  theStateKey.numDoubles = 0;
  theRandomSeedKey.numInts = 0;
  theRandomSeedKey.numDoubles = 0;
}

void env_set_state(State_key theStateKey) {
  const int envState = kEnvSetState;

  rlBufferClear(&theBuffer);
  rlBufferWrite(&theBuffer, 0, &envState, 1, sizeof(int));
  rlSendBufferData(theEnvironmentConnection, &theBuffer);

  rlBufferClear(&theBuffer);
  rlCopyADTToBuffer(&theStateKey, &theBuffer);
  rlSendBufferData(theEnvironmentConnection, &theBuffer);
}

void env_set_random_seed(Random_seed_key theRandomSeedKey) {
  const int envState = kEnvSetRandomSeed;

  rlBufferClear(&theBuffer);
  rlBufferWrite(&theBuffer, 0, &envState, 1, sizeof(int));
  rlSendBufferData(theEnvironmentConnection, &theBuffer);

  rlBufferClear(&theBuffer);
  rlCopyADTToBuffer(&theRandomSeedKey, &theBuffer);
  rlSendBufferData(theEnvironmentConnection, &theBuffer);
}

State_key env_get_state() {
  const int envState = kEnvGetState;

  rlBufferClear(&theBuffer);
  rlBufferWrite(&theBuffer, 0, &envState, 1, sizeof(int));
  rlSendBufferData(theEnvironmentConnection, &theBuffer);

  rlBufferClear(&theBuffer);
  rlRecvBufferData(theEnvironmentConnection, &theBuffer);
  rlCopyBufferToADT(&theBuffer, &theStateKey);

  return theStateKey;
}

Random_seed_key env_get_random_seed() {
  const int envState = kEnvGetRandomSeed;

  rlBufferClear(&theBuffer);
  rlBufferWrite(&theBuffer, 0, &envState, 1, sizeof(int));
  rlSendBufferData(theEnvironmentConnection, &theBuffer);

  rlBufferClear(&theBuffer);
  rlRecvBufferData(theEnvironmentConnection, &theBuffer);
  rlCopyBufferToADT(&theBuffer, &theRandomSeedKey);

  return theRandomSeedKey;
}

char* env_message(const char* inMessage) {
  int theInMessageLength = 0;
  int theOutMessageLength = 0;
  char * theOutMessage = NULL;

  const int envState = kEnvMessage;
  int offset = 0;

  rlBufferClear(&theBuffer);
  rlBufferWrite(&theBuffer, 0, &envState, 1, sizeof(int));
  rlSendBufferData(theEnvironmentConnection, &theBuffer);

  if (inMessage != NULL) {
    theInMessageLength = strlen(inMessage) + 1;
  }

  offset = 0;
  rlBufferClear(&theBuffer);
  offset = rlBufferWrite(&theBuffer, offset, &theInMessageLength, 1, sizeof(int));
  if (theInMessageLength > 0) {
    offset = rlBufferWrite(&theBuffer, offset, inMessage, theInMessageLength, sizeof(char));
    rlSendBufferData(theEnvironmentConnection, &theBuffer);
  }

  offset = 0;
  rlBufferClear(&theBuffer);
  rlRecvBufferData(theEnvironmentConnection, &theBuffer);
  offset = rlBufferRead(&theBuffer, offset, &theOutMessageLength, 1, sizeof(int));
  if (theOutMessageLength > 0) {
    theOutMessage = (char*)calloc(theOutMessageLength, sizeof(char));
    offset = rlBufferRead(&theBuffer, offset, theOutMessage, theOutMessageLength, sizeof(char));
  }
  return theOutMessage;
}
