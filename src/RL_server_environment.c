/* 
* Copyright (C) 2007, Andrew Butcher

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.

* 
*  $Revision$
*  $Date$
*  $Author$
*  $HeadURL$
* 
*/


#include <stdlib.h> /* calloc */
#include <string.h> /* strlen */
#include <assert.h> /* assert */

#include <stdio.h> /* fprintf: debug only */

#include <rlglue/Environment_common.h>
#include <rlglue/network/RL_network.h>

static task_specification_t theTaskSpec   = 0;
static rlBuffer theBuffer               = {0};
static observation_t theObservation = {0};
static state_key_t theStateKey            = {0};
static random_seed_key_t theRandomSeedKey = {0};
static char* theOutMessage = 0;

extern void rlSetEnvironmentConnection();
extern int rlGetEnvironmentConnection();

task_specification_t env_init() {
  /* Setup the connection */
  int envState = kEnvInit;
  unsigned int theTaskSpecLength = 0;
  unsigned int offset = 0;

  if (theBuffer.capacity == 0)
    rlBufferCreate(&theBuffer, 65536);

  /* env init-specific data */
  rlBufferClear(&theBuffer);
  rlSendBufferData(rlGetEnvironmentConnection(), &theBuffer, envState);

  rlBufferClear(&theBuffer);
  rlRecvBufferData(rlGetEnvironmentConnection(), &theBuffer, &envState);
  assert(envState == kEnvInit);

  offset = 0;
  offset = rlBufferRead(&theBuffer, offset, &theTaskSpecLength, 1, sizeof(int));  
  if (theTaskSpecLength > 0) {

    if (theTaskSpec != 0) {
      free(theTaskSpec);
      theTaskSpec = 0;
    }

    theTaskSpec = (char*)calloc(theTaskSpecLength+1, sizeof(char));
    offset = rlBufferRead(&theBuffer, offset, theTaskSpec, theTaskSpecLength, sizeof(char));
    theTaskSpec[theTaskSpecLength] = '\0';
  }

  return theTaskSpec;
}

observation_t env_start() {
  int envState = kEnvStart;
  unsigned int offset = 0;

  rlBufferClear(&theBuffer);
  rlSendBufferData(rlGetEnvironmentConnection(), &theBuffer, envState);

  rlBufferClear(&theBuffer);
  rlRecvBufferData(rlGetEnvironmentConnection(), &theBuffer, &envState);
  assert(envState == kEnvStart);

  offset = rlCopyBufferToADT(&theBuffer, offset, &theObservation);
  return theObservation;
}

reward_observation_t env_step(action_t theAction) {
  int envState = kEnvStep;
  reward_observation_t ro = {0};
  unsigned int offset = 0;

  rlBufferClear(&theBuffer);
  offset = 0;
  offset = rlCopyADTToBuffer(&theAction, &theBuffer, offset);
  rlSendBufferData(rlGetEnvironmentConnection(), &theBuffer, envState);

  rlBufferClear(&theBuffer);
  rlRecvBufferData(rlGetEnvironmentConnection(), &theBuffer, &envState);
  assert(envState == kEnvStep);

  offset = 0;
  offset = rlBufferRead(&theBuffer, offset, &ro.terminal, 1, sizeof(int));
  offset = rlBufferRead(&theBuffer, offset, &ro.r, 1, sizeof(reward_t));
  offset = rlCopyBufferToADT(&theBuffer, offset, &theObservation);

  ro.o = theObservation;
  return ro;
}

void env_cleanup() {
  int envState = kEnvCleanup;

  rlBufferClear(&theBuffer);
  rlSendBufferData(rlGetEnvironmentConnection(), &theBuffer, envState);

  rlBufferClear(&theBuffer);
  rlRecvBufferData(rlGetEnvironmentConnection(), &theBuffer, &envState);
  assert(envState == kEnvCleanup);

  rlBufferDestroy(&theBuffer);

  if (theTaskSpec != 0) {
    free(theTaskSpec);
    theTaskSpec = 0;
  }

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
    theRandomSeedKey.intArray = 0;
    theRandomSeedKey.numInts = 0;
  }

  if (theRandomSeedKey.doubleArray != 0) {
    free(theRandomSeedKey.doubleArray);
    theRandomSeedKey.doubleArray = 0;
    theRandomSeedKey.numDoubles = 0;
  }

  if (theOutMessage != 0) {
    free(theOutMessage);
    theOutMessage = 0;
  }
}

void env_set_state(state_key_t theStateKey) {
  int envState = kEnvSetState;
  unsigned int offset = 0;

  rlBufferClear(&theBuffer);
  offset = rlCopyADTToBuffer(&theStateKey, &theBuffer, offset);
  rlSendBufferData(rlGetEnvironmentConnection(), &theBuffer, envState);

  rlBufferClear(&theBuffer);
  rlRecvBufferData(rlGetEnvironmentConnection(), &theBuffer, &envState);
  assert(envState == kEnvSetState);
}

void env_set_random_seed(random_seed_key_t theRandomSeedKey) {
  int envState = kEnvSetRandomSeed;
  unsigned int offset = 0;

  rlBufferClear(&theBuffer);
  offset = rlCopyADTToBuffer(&theRandomSeedKey, &theBuffer, offset);
  rlSendBufferData(rlGetEnvironmentConnection(), &theBuffer, envState);

  rlBufferClear(&theBuffer);
  rlRecvBufferData(rlGetEnvironmentConnection(), &theBuffer, &envState);
  assert(envState == kEnvSetRandomSeed);
}

state_key_t env_get_state() {
  int envState = kEnvGetState;
  unsigned int offset = 0;

  rlBufferClear(&theBuffer);
  rlSendBufferData(rlGetEnvironmentConnection(), &theBuffer, envState);

  rlBufferClear(&theBuffer);
  rlRecvBufferData(rlGetEnvironmentConnection(), &theBuffer, &envState);
  assert(envState == kEnvGetState);

  offset = rlCopyBufferToADT(&theBuffer, offset, &theStateKey);

  return theStateKey;
}

random_seed_key_t env_get_random_seed() {
  int envState = kEnvGetRandomSeed;
  unsigned int offset = 0;

  rlBufferClear(&theBuffer);
  rlSendBufferData(rlGetEnvironmentConnection(), &theBuffer, envState);

  rlBufferClear(&theBuffer);
  rlRecvBufferData(rlGetEnvironmentConnection(), &theBuffer, &envState);
  assert(envState == kEnvGetRandomSeed);

  offset = rlCopyBufferToADT(&theBuffer, offset, &theRandomSeedKey);

  return theRandomSeedKey;
}

message_t env_message(const message_t inMessage) {
  int envState = kEnvMessage;
  unsigned int theInMessageLength = 0;
  unsigned int theOutMessageLength = 0;
  unsigned int offset = 0;

  if (inMessage != NULL) {
    theInMessageLength = strlen(inMessage);
  }

  if (theBuffer.capacity == 0)
    rlBufferCreate(&theBuffer, 65356);

  rlBufferClear(&theBuffer);
  offset = 0;
  offset = rlBufferWrite(&theBuffer, offset, &theInMessageLength, 1, sizeof(int));
  if (theInMessageLength > 0) {
    offset = rlBufferWrite(&theBuffer, offset, inMessage, theInMessageLength, sizeof(char));
  }
  rlSendBufferData(rlGetEnvironmentConnection(), &theBuffer, envState);

  rlBufferClear(&theBuffer);
  rlRecvBufferData(rlGetEnvironmentConnection(), &theBuffer, &envState);
  assert(envState == kEnvMessage);

  offset = 0;
  offset = rlBufferRead(&theBuffer, offset, &theOutMessageLength, 1, sizeof(int));
/*Free and point the old message to null */
    if (theOutMessage != 0) {
      free(theOutMessage);
      theOutMessage = 0;
    }
/* Allocated memory for the new message, maybe just 1 byte for the terminator */
    theOutMessage = (char*)calloc(theOutMessageLength+1, sizeof(char));

/* Fill up the string from the buffer */
if (theOutMessageLength > 0) {
    offset = rlBufferRead(&theBuffer, offset, theOutMessage, theOutMessageLength, sizeof(char));
  }
/* Set the terminator */
    theOutMessage[theOutMessageLength] = '\0';
  return theOutMessage;
}
