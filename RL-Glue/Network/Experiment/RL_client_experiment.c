#include <stdlib.h> /* calloc */
#include <string.h> /* strlen */

#include <RL_common.h>
#include <Network/RL_netlib.h>

static rlSocket theExperimentConnection = 0;

static Observation theObservation       = {0};
static Action theAction                 = {0};
static State_key theStateKey            = {0};
static Random_seed_key theRandomSeedKey = {0};
static rlBuffer theBuffer               = {0};
static Task_specification theTaskSpec = 0;

static char* theMessage = 0;
static int theMessageCapacity = 0;

void RL_init() {
  const int theConnectionType = kExperimentConnection;
  const int experimentState = kRLInit;

  rlBufferCreate(&theBuffer, 4096);

  theExperimentConnection = rlWaitForConnection(kLocalHost, kDefaultPort, kRetryTimeout);

  rlBufferClear(&theBuffer);
  rlBufferWrite(&theBuffer, 0, &theConnectionType, 1, sizeof(int));
  rlSendBufferData(theExperimentConnection, &theBuffer);

  rlBufferClear(&theBuffer);
  rlBufferWrite(&theBuffer, 0, &experimentState, 1, sizeof(int));
  rlSendBufferData(theExperimentConnection, &theBuffer);
}

Observation_action RL_start() {
  const int experimentState = kRLStart;
  Observation_action oa = { {0}, {0} };

  rlBufferClear(&theBuffer);
  rlBufferWrite(&theBuffer, 0, &experimentState, 1, sizeof(int));
  rlSendBufferData(theExperimentConnection, &theBuffer);

  rlBufferClear(&theBuffer);
  rlRecvBufferData(theExperimentConnection, &theBuffer);
  rlCopyBufferToADT(&theBuffer, &theObservation);

  rlBufferClear(&theBuffer);
  rlRecvBufferData(theExperimentConnection, &theBuffer);
  rlCopyBufferToADT(&theBuffer, &theAction);

  oa.o = theObservation;
  oa.a = theAction;

  return oa;
}

Reward_observation_action_terminal RL_step() {
  const int experimentState = kRLStep;
  Reward_observation_action_terminal roat = {0, {0}, {0}, 0};
  int offset = 0;
  
  rlBufferClear(&theBuffer);
  rlBufferWrite(&theBuffer, 0, &experimentState, 1, sizeof(int));
  rlSendBufferData(theExperimentConnection, &theBuffer);

  /* Recv Terminal & Reward */
  rlBufferClear(&theBuffer);
  rlRecvBufferData(theExperimentConnection, &theBuffer);
  offset = rlBufferRead(&theBuffer, offset, &roat.terminal, 1, sizeof(int));
  offset = rlBufferRead(&theBuffer, offset, &roat.r, 1, sizeof(Reward));

  /* Recv Observation */
  rlBufferClear(&theBuffer);
  rlRecvBufferData(theExperimentConnection, &theBuffer);
  rlCopyBufferToADT(&theBuffer, &theObservation);

  /* Recv Action */
  rlBufferClear(&theBuffer);
  rlRecvBufferData(theExperimentConnection, &theBuffer);
  rlCopyBufferToADT(&theBuffer, &theAction);

  roat.o = theObservation;
  roat.a = theAction;

  return roat;
}

void RL_cleanup() {
  const int experimentState = kRLCleanup;

  rlBufferClear(&theBuffer);
  rlBufferWrite(&theBuffer, 0, &experimentState, 1, sizeof(int));
  rlSendBufferData(theExperimentConnection, &theBuffer);

  rlClose(theExperimentConnection);
  theExperimentConnection = 0;

  free(theObservation.intArray);
  free(theObservation.doubleArray);
  free(theAction.intArray);
  free(theAction.doubleArray);
  free(theStateKey.intArray);
  free(theStateKey.doubleArray);
  free(theRandomSeedKey.intArray);
  free(theRandomSeedKey.doubleArray);
  free(theTaskSpec);
  free(theMessage);

  theObservation.numInts = 0;
  theObservation.numDoubles = 0;
  theAction.numInts = 0;
  theAction.numDoubles = 0;
  theStateKey.numInts = 0;
  theStateKey.numDoubles = 0;
  theRandomSeedKey.numInts = 0;
  theRandomSeedKey.numDoubles = 0;
  theTaskSpec = 0;
  theMessage = 0;
  theMessageCapacity = 0;
}

Reward RL_return() {
  const int experimentState = kRLReturn;
  Reward theReward = 0;

  rlBufferClear(&theBuffer);
  rlBufferWrite(&theBuffer, 0, &experimentState, 1, sizeof(int));
  rlSendBufferData(theExperimentConnection, &theBuffer);

  rlBufferClear(&theBuffer);
  rlRecvBufferData(theExperimentConnection, &theBuffer);
  rlBufferRead(&theBuffer, 0, &theReward, 1, sizeof(Reward));

  return theReward;
}

int RL_num_steps() {
  const int experimentState = kRLNumSteps;
  int numSteps = 0;

  rlBufferClear(&theBuffer);
  rlBufferWrite(&theBuffer, 0, &experimentState, 1, sizeof(int));
  rlSendBufferData(theExperimentConnection, &theBuffer);

  rlBufferClear(&theBuffer);
  rlRecvBufferData(theExperimentConnection, &theBuffer);
  rlBufferRead(&theBuffer, 0, &numSteps, 1, sizeof(int));

  return numSteps;
}


char* RL_agent_message(const char* message) {
  const int experimentState = kRLAgentMessage;
  int messageLength = strlen(message) + 1;
  int offset = 0;

  rlBufferClear(&theBuffer);
  rlBufferWrite(&theBuffer, 0, &experimentState, 1, sizeof(int));
  rlSendBufferData(theExperimentConnection, &theBuffer);

  offset = 0;
  rlBufferClear(&theBuffer);
  offset = rlBufferWrite(&theBuffer, offset, &messageLength, 1, sizeof(int));
  if (messageLength > 0) {
    offset = rlBufferWrite(&theBuffer, offset, message, messageLength, sizeof(char));
  }
  rlSendBufferData(theExperimentConnection, &theBuffer);
  
  offset = 0;
  rlBufferClear(&theBuffer);
  offset = rlBufferRead(&theBuffer, offset, &messageLength, 1, sizeof(int));

  if (messageLength > theMessageCapacity) {
    free(theMessage);
    theMessage = (char*)calloc(messageLength, sizeof(char));
    theMessageCapacity = messageLength;
  }

  if (messageLength > 0) {
    offset = rlBufferRead(&theBuffer, offset, theMessage, messageLength, sizeof(char));
  }

  return theMessage;
}


char* RL_env_message(const char* message) {
  const int experimentState = kRLEnvMessage;
  int messageLength = strlen(message) + 1;
  int offset = 0;

  rlBufferClear(&theBuffer);
  rlBufferWrite(&theBuffer, 0, &experimentState, 1, sizeof(int));
  rlSendBufferData(theExperimentConnection, &theBuffer);

  offset = 0;
  rlBufferClear(&theBuffer);
  offset = rlBufferWrite(&theBuffer, offset, &messageLength, 1, sizeof(int));
  if (messageLength > 0) {
    offset = rlBufferWrite(&theBuffer, offset, message, messageLength, sizeof(char));
  }
  rlSendBufferData(theExperimentConnection, &theBuffer);
  
  offset = 0;
  rlBufferClear(&theBuffer);
  offset = rlBufferRead(&theBuffer, offset, &messageLength, 1, sizeof(int));

  if (messageLength > theMessageCapacity) {
    free(theMessage);
    theMessage = (char*)calloc(messageLength, sizeof(char));
    theMessageCapacity = messageLength;
  }

  if (messageLength > 0) {
    offset = rlBufferRead(&theBuffer, offset, theMessage, messageLength, sizeof(char));
  }

  return theMessage;
}


int RL_num_episodes() {
  const int experimentState = kRLNumEpisodes;
  int numEpisodes = 0;

  rlBufferClear(&theBuffer);
  rlBufferWrite(&theBuffer, 0, &experimentState, 1, sizeof(int));
  rlSendBufferData(theExperimentConnection, &theBuffer);

  rlBufferClear(&theBuffer);
  rlRecvBufferData(theExperimentConnection, &theBuffer);
  rlBufferRead(&theBuffer, 0, &numEpisodes, 1, sizeof(int));

  return numEpisodes;
}

void RL_episode(int numSteps) {
  const int experimentState = kRLEpisode;

  rlBufferClear(&theBuffer);
  rlBufferWrite(&theBuffer, 0, &experimentState, 1, sizeof(int));
  rlSendBufferData(theExperimentConnection, &theBuffer);

  rlBufferClear(&theBuffer);
  rlBufferWrite(&theBuffer, 0, &numSteps, 1, sizeof(int));
  rlSendBufferData(theExperimentConnection, &theBuffer);
}

void RL_freeze() {
  const int experimentState = kRLFreeze;

  rlBufferClear(&theBuffer);
  rlBufferWrite(&theBuffer, 0, &experimentState, 1, sizeof(int));
  rlSendBufferData(theExperimentConnection, &theBuffer);
}

void RL_set_state(State_key theStateKey) {
  const int experimentState = kRLSetState;

  rlBufferClear(&theBuffer);
  rlBufferWrite(&theBuffer, 0, &experimentState, 1, sizeof(int));
  rlSendBufferData(theExperimentConnection, &theBuffer);
  
  rlBufferClear(&theBuffer);
  rlCopyADTToBuffer(&theStateKey, &theBuffer);
  rlSendBufferData(theExperimentConnection, &theBuffer);
}

void RL_set_random_seed(Random_seed_key theRandomSeedKey) {
  const int experimentState = kRLSetRandomSeed;

  rlBufferClear(&theBuffer);
  rlBufferWrite(&theBuffer, 0, &experimentState, 1, sizeof(int));
  rlSendBufferData(theExperimentConnection, &theBuffer);
  
  rlBufferClear(&theBuffer);
  rlCopyADTToBuffer(&theRandomSeedKey, &theBuffer);
  rlSendBufferData(theExperimentConnection, &theBuffer);
}

State_key RL_get_state() {
  const int experimentState = kRLGetState;

  rlBufferClear(&theBuffer);
  rlBufferWrite(&theBuffer, 0, &experimentState, 1, sizeof(int));
  rlSendBufferData(theExperimentConnection, &theBuffer);

  rlBufferClear(&theBuffer);
  rlRecvBufferData(theExperimentConnection, &theBuffer);
  rlCopyBufferToADT(&theBuffer, &theStateKey);

  return theStateKey;
}

Random_seed_key RL_get_random_seed() {
  const int experimentState = kRLGetRandomSeed;

  rlBufferClear(&theBuffer);
  rlBufferWrite(&theBuffer, 0, &experimentState, 1, sizeof(int));
  rlSendBufferData(theExperimentConnection, &theBuffer);

  rlBufferClear(&theBuffer);
  rlRecvBufferData(theExperimentConnection, &theBuffer);
  rlCopyBufferToADT(&theBuffer, &theRandomSeedKey);

  return theRandomSeedKey;
}
