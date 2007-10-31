/* 
* Copyright (C) 2007, Adam White
* 
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (at your option) any later version.
* 
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* 
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA. */

#include <stdlib.h> /* calloc */
#include <string.h> /* strlen */
#include <assert.h> /* assert */

#include <stdio.h> /* fprintf: debug only */

#include <RL_common.h>
#include <Network/RL_network.h>

static Task_specification theTaskSpec   = 0;
static rlBuffer theBuffer               = {0};
static Observation theObservation       = {0};
static State_key theStateKey            = {0};
static Random_seed_key theRandomSeedKey = {0};
static char* theOutMessage = 0;

extern void rlSetEnvironmentConnection();
extern int rlGetEnvironmentConnection();

Task_specification env_init() {
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

Observation env_start() {
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

Reward_observation env_step(Action theAction) {
  int envState = kEnvStep;
  Reward_observation ro = {0};
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
  offset = rlBufferRead(&theBuffer, offset, &ro.r, 1, sizeof(Reward));
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

void env_set_state(State_key theStateKey) {
  int envState = kEnvSetState;
  unsigned int offset = 0;

  rlBufferClear(&theBuffer);
  offset = rlCopyADTToBuffer(&theStateKey, &theBuffer, offset);
  rlSendBufferData(rlGetEnvironmentConnection(), &theBuffer, envState);

  rlBufferClear(&theBuffer);
  rlRecvBufferData(rlGetEnvironmentConnection(), &theBuffer, &envState);
  assert(envState == kEnvSetState);
}

void env_set_random_seed(Random_seed_key theRandomSeedKey) {
  int envState = kEnvSetRandomSeed;
  unsigned int offset = 0;

  rlBufferClear(&theBuffer);
  offset = rlCopyADTToBuffer(&theRandomSeedKey, &theBuffer, offset);
  rlSendBufferData(rlGetEnvironmentConnection(), &theBuffer, envState);

  rlBufferClear(&theBuffer);
  rlRecvBufferData(rlGetEnvironmentConnection(), &theBuffer, &envState);
  assert(envState == kEnvSetRandomSeed);
}

State_key env_get_state() {
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

Random_seed_key env_get_random_seed() {
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

Message env_message(const Message inMessage) {
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
