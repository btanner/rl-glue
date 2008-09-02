/* 
* Copyright (C) 2007, Adam White

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 */

#include <stdlib.h> /* calloc */
#include <string.h> /* strlen */
#include <stdio.h>  /* fprintf */
#include <assert.h> /* assert */

#include <ctype.h> /* isdigit */
#include <netdb.h> /* gethostbyname */
#include <arpa/inet.h> /* inet_ntoa */

#include <RL_common.h>
#include <RL_network.h>

static int theExperimentConnection = 0;

static Observation theObservation       = {0};
static Action theAction                 = {0};
static State_key theStateKey            = {0};
static Random_seed_key theRandomSeedKey = {0};
static rlBuffer theBuffer               = {0};
static Task_specification theTaskSpec = 0;

static char* theMessage = 0;
static unsigned int theMessageCapacity = 0;

void cleanupExperimentAtExit(void)
{
  rlBufferDestroy(&theBuffer);
}

static void forceConnection()
{
  struct hostent *host_ent;

  char* host = kLocalHost;
  short port = kDefaultPort;
  char* envptr = 0;

  if (theExperimentConnection == 0) {
    host = getenv("RLGLUE_HOST");
    if (host == 0) {
      host = kLocalHost;
    }

    envptr = getenv("RLGLUE_PORT");  
    if (envptr != 0) {
      port = strtol(envptr, 0, 10);
      if (port == 0) {
	port = kDefaultPort;
      }
    }
    
    if (isalpha(host[0])) {
      host_ent = gethostbyname(host); 
      host = inet_ntoa(*(struct in_addr*)host_ent->h_addr);
    }

    fprintf(stderr, "Connecting to host=%s on port=%d\n", host, port);
    theExperimentConnection = rlWaitForConnection(host, port, kRetryTimeout);

    /* Send the connection type */
    atexit(cleanupExperimentAtExit);
    rlBufferCreate(&theBuffer, 65536);
    rlBufferClear(&theBuffer);
    rlSendBufferData(theExperimentConnection, &theBuffer, kExperimentConnection);
  }
}

void RL_init() {
  int experimentState = kRLInit;

  forceConnection();

  /* Remote call RL_init */
  rlBufferClear(&theBuffer);
  rlSendBufferData(theExperimentConnection, &theBuffer, experimentState);

  /* Recv back a reply from RL_init */
  rlBufferClear(&theBuffer);
  rlRecvBufferData(theExperimentConnection, &theBuffer, &experimentState);

  assert(experimentState == kRLInit);
}

Observation_action RL_start() {
  int experimentState = kRLStart;
  Observation_action oa = { {0}, {0} };
  unsigned int offset = 0;

  assert(theExperimentConnection != 0);

  rlBufferClear(&theBuffer);
  rlSendBufferData(theExperimentConnection, &theBuffer, experimentState);

  rlBufferClear(&theBuffer);
  rlRecvBufferData(theExperimentConnection, &theBuffer, &experimentState); 
  assert(experimentState == kRLStart);

  offset = rlCopyBufferToADT(&theBuffer, offset, &theObservation);
  offset = rlCopyBufferToADT(&theBuffer, offset, &theAction);

  oa.o = theObservation;
  oa.a = theAction;

  return oa;
}

Reward_observation_action_terminal RL_step() {
  int experimentState = kRLStep;
  Reward_observation_action_terminal roat = {0, {0}, {0}, 0};
  unsigned int offset = 0;
  
  assert(theExperimentConnection != 0);

  rlBufferClear(&theBuffer);
  rlSendBufferData(theExperimentConnection, &theBuffer, experimentState);

  /* Recv Data from Server */
  rlBufferClear(&theBuffer);
  rlRecvBufferData(theExperimentConnection, &theBuffer, &experimentState);
  assert(experimentState == kRLStep);

  offset = rlBufferRead(&theBuffer, offset, &roat.terminal, 1, sizeof(int));
  offset = rlBufferRead(&theBuffer, offset, &roat.r, 1, sizeof(Reward));
  offset = rlCopyBufferToADT(&theBuffer, offset, &theObservation);
  offset = rlCopyBufferToADT(&theBuffer, offset, &theAction);

  roat.o = theObservation;
  roat.a = theAction;

  return roat;
}

void RL_cleanup() {
  int experimentState = kRLCleanup;

  assert(theExperimentConnection != 0);

  rlBufferClear(&theBuffer);
  rlSendBufferData(theExperimentConnection, &theBuffer, experimentState);

  rlBufferClear(&theBuffer);
  rlRecvBufferData(theExperimentConnection, &theBuffer, &experimentState);
  assert(experimentState == kRLCleanup);

  if (theObservation.intArray != 0) {
    free(theObservation.intArray);
    theObservation.intArray = 0;
    theObservation.numInts = 0;
  }

  if (theObservation.doubleArray != 0) {
    free(theObservation.doubleArray);
    theObservation.doubleArray = 0;
    theObservation.numDoubles = 0;
  }

  if (theAction.intArray != 0) {
    free(theAction.intArray);
    theAction.intArray = 0;
    theAction.numInts = 0;
  }

  if (theAction.doubleArray != 0) {
    free(theAction.doubleArray);
    theAction.doubleArray = 0;
    theAction.numDoubles = 0;
  }

  if (theStateKey.intArray != 0) {
    free(theStateKey.intArray);
    theStateKey.intArray = 0;
    theStateKey.numInts = 0;
  }

  if (theStateKey.doubleArray != 0) {
    free(theStateKey.doubleArray);
    theStateKey.doubleArray = 0;
    theStateKey.numDoubles = 0;
  }

  if (theRandomSeedKey.intArray != 0) {
    free(theRandomSeedKey.intArray);
    theStateKey.intArray = 0;
    theStateKey.numInts = 0;
  }

  if (theRandomSeedKey.doubleArray != 0) {
    free(theRandomSeedKey.doubleArray);
    theRandomSeedKey.doubleArray = 0;
    theRandomSeedKey.numDoubles = 0;
  }

  if (theTaskSpec != 0) {
    free(theTaskSpec);
    theTaskSpec = 0;
  }

  if (theMessage != 0) {
    free(theMessage);
    theMessage = 0;
  }

  theMessageCapacity = 0;
}

Reward RL_return() {
  int experimentState = kRLReturn;
  Reward theReward = 0;
  unsigned int offset = 0;

  assert(theExperimentConnection != 0);

  rlBufferClear(&theBuffer);
  rlSendBufferData(theExperimentConnection, &theBuffer, experimentState);

  rlBufferClear(&theBuffer);
  rlRecvBufferData(theExperimentConnection, &theBuffer, &experimentState);
  assert(experimentState == kRLReturn);

  offset = rlBufferRead(&theBuffer, offset, &theReward, 1, sizeof(Reward));

  return theReward;
}

int RL_num_steps() {
  int experimentState = kRLNumSteps;
  int numSteps = 0;
  unsigned int offset = 0;

  assert(theExperimentConnection != 0);

  rlBufferClear(&theBuffer);
  rlSendBufferData(theExperimentConnection, &theBuffer, experimentState);

  rlBufferClear(&theBuffer);
  rlRecvBufferData(theExperimentConnection, &theBuffer, &experimentState);
  assert(experimentState == kRLNumSteps);

  offset = rlBufferRead(&theBuffer, offset, &numSteps, 1, sizeof(int));

  return numSteps;
}


Message RL_agent_message(const Message message) {
  int experimentState = kRLAgentMessage;
  unsigned int messageLength = 0;
  unsigned int offset = 0;

  if (message != 0)
    messageLength = strlen(message);

  forceConnection();

  rlBufferClear(&theBuffer);
  offset = 0;
  offset = rlBufferWrite(&theBuffer, offset, &messageLength, 1, sizeof(int));
  if (messageLength > 0) {
    offset = rlBufferWrite(&theBuffer, offset, message, messageLength, sizeof(char));
  }
  rlSendBufferData(theExperimentConnection, &theBuffer, experimentState);
  
  offset = 0;
  rlBufferClear(&theBuffer);
  rlRecvBufferData(theExperimentConnection, &theBuffer, &experimentState);
  assert(experimentState == kRLAgentMessage);

  offset = rlBufferRead(&theBuffer, offset, &messageLength, 1, sizeof(int));
  if (messageLength > theMessageCapacity) {
    free(theMessage);
    theMessage = (char*)calloc(messageLength+1, sizeof(char));
    theMessageCapacity = messageLength;
  }

  if (messageLength > 0) {
    offset = rlBufferRead(&theBuffer, offset, theMessage, messageLength, sizeof(char));
    theMessage[messageLength] = '\0';
  }

  return theMessage;
}


Message RL_env_message(const Message message) {
  int experimentState = kRLEnvMessage;
  unsigned int messageLength = 0;
  unsigned int offset = 0;

  if (message != 0)
    messageLength = strlen(message);

  forceConnection();

  rlBufferClear(&theBuffer);
  offset = 0;
  offset = rlBufferWrite(&theBuffer, offset, &messageLength, 1, sizeof(int));
  if (messageLength > 0) {
    offset = rlBufferWrite(&theBuffer, offset, message, messageLength, sizeof(char));
  }
  rlSendBufferData(theExperimentConnection, &theBuffer, experimentState);
  
  rlBufferClear(&theBuffer);
  rlRecvBufferData(theExperimentConnection, &theBuffer, &experimentState);
  assert(experimentState == kRLEnvMessage);

  offset = 0;
  offset = rlBufferRead(&theBuffer, offset, &messageLength, 1, sizeof(int));
  if (messageLength > theMessageCapacity) {
    free(theMessage);
    theMessage = (char*)calloc(messageLength+1, sizeof(char));
    theMessageCapacity = messageLength;
  }

  if (messageLength > 0) {
    offset = rlBufferRead(&theBuffer, offset, theMessage, messageLength, sizeof(char));
    theMessage[messageLength] = '\0';
  }

  return theMessage;
}

int RL_num_episodes() {
  int experimentState = kRLNumEpisodes;
  int numEpisodes = 0;
  unsigned int offset = 0;

  assert(theExperimentConnection != 0);

  rlBufferClear(&theBuffer);
  rlSendBufferData(theExperimentConnection, &theBuffer, experimentState);

  rlBufferClear(&theBuffer);
  rlRecvBufferData(theExperimentConnection, &theBuffer, &experimentState);
  assert(experimentState == kRLNumEpisodes);

  offset = rlBufferRead(&theBuffer, offset, &numEpisodes, 1, sizeof(int));

  return numEpisodes;
}

void RL_episode(unsigned int numSteps) {
  unsigned int offset = 0;
  int experimentState = kRLEpisode;

  assert(theExperimentConnection != 0);

  rlBufferClear(&theBuffer);
  offset = 0;
  offset = rlBufferWrite(&theBuffer, offset, &numSteps, 1, sizeof(int));
  rlSendBufferData(theExperimentConnection, &theBuffer, experimentState);

  rlBufferClear(&theBuffer);
  rlRecvBufferData(theExperimentConnection, &theBuffer, &experimentState);

  assert(experimentState == kRLEpisode);
}

void RL_freeze() {
  int experimentState = kRLFreeze;

  assert(theExperimentConnection != 0);

  rlBufferClear(&theBuffer);
  rlSendBufferData(theExperimentConnection, &theBuffer, experimentState);

  rlBufferClear(&theBuffer);
  rlRecvBufferData(theExperimentConnection, &theBuffer, &experimentState);
  assert(experimentState == kRLFreeze);
}

void RL_set_state(State_key theStateKey) {
  int experimentState = kRLSetState;
  unsigned int offset = 0;

  assert(theExperimentConnection != 0);

  rlBufferClear(&theBuffer);
  offset = rlCopyADTToBuffer(&theStateKey, &theBuffer, offset);
  rlSendBufferData(theExperimentConnection, &theBuffer, experimentState);

  rlBufferClear(&theBuffer);
  rlRecvBufferData(theExperimentConnection, &theBuffer, &experimentState);
  assert(experimentState == kRLSetState);
}

void RL_set_random_seed(Random_seed_key theRandomSeedKey) {
  int experimentState = kRLSetRandomSeed;
  unsigned int offset = 0;

  assert(theExperimentConnection != 0);

  rlBufferClear(&theBuffer);
  offset = rlCopyADTToBuffer(&theRandomSeedKey, &theBuffer, offset);
  rlSendBufferData(theExperimentConnection, &theBuffer, experimentState);

  rlBufferClear(&theBuffer);
  rlRecvBufferData(theExperimentConnection, &theBuffer, &experimentState);
  assert(experimentState == kRLSetRandomSeed);
}

State_key RL_get_state() {
  int experimentState = kRLGetState;
  unsigned int offset = 0;

  assert(theExperimentConnection != 0);

  rlBufferClear(&theBuffer);
  rlSendBufferData(theExperimentConnection, &theBuffer, experimentState);

  rlBufferClear(&theBuffer);
  rlRecvBufferData(theExperimentConnection, &theBuffer, &experimentState);
  assert(experimentState == kRLGetState);

  offset = rlCopyBufferToADT(&theBuffer, offset, &theStateKey);

  return theStateKey;
}

Random_seed_key RL_get_random_seed() {
  int experimentState = kRLGetRandomSeed;
  unsigned int offset = 0;

  assert(theExperimentConnection != 0);

  rlBufferClear(&theBuffer);
  rlSendBufferData(theExperimentConnection, &theBuffer, experimentState);

  rlBufferClear(&theBuffer);
  rlRecvBufferData(theExperimentConnection, &theBuffer, &experimentState);
  assert(experimentState == kRLGetRandomSeed);

  offset = rlCopyBufferToADT(&theBuffer, offset, &theRandomSeedKey);

  return theRandomSeedKey;
}
