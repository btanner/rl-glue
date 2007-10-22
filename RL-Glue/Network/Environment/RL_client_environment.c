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

#include <assert.h> /* assert  */
#include <unistd.h> /* sleep   */
#include <string.h> /* strlen */ /* I'm sorry about using strlen. */
#include <stdio.h>  /* fprintf */
#include <stdlib.h> /* calloc, getenv, exit */

#include <ctype.h> /* isdigit */
#include <netdb.h> /* gethostbyname */
#include <arpa/inet.h> /* inet_ntoa */

#include <RL_common.h>
#include <Network/RL_network.h>

/* Provide forward declaration of environment interface */
extern Task_specification env_init();
extern Observation env_start();
extern Reward_observation env_step(Action a);
extern void env_cleanup();
extern void env_set_state(State_key sk);
extern void env_set_random_seed(Random_seed_key rsk);
extern State_key env_get_state();
extern Random_seed_key env_get_random_seed();
extern Message env_message(const Message inMessage);

static const char* kUnknownMessage = "Unknown Message: %s\n";

static Action theAction                 = {0};
static State_key theStateKey            = {0};
static Random_seed_key theRandomSeedKey = {0};
static rlBuffer theBuffer               = {0};
static Message theInMessage = 0;
static unsigned int theInMessageCapacity = 0;

static void onEnvInit(int theConnection) {
  Task_specification theTaskSpec = 0;
  unsigned int theTaskSpecLength = 0;
  unsigned int offset = 0;

  /* Read the data in the buffer (data from server) */
  /* No data to read .... */

  /* Call RL method on the recv'd data */
  theTaskSpec = env_init();
  if (theTaskSpec != NULL) {
    theTaskSpecLength = strlen(theTaskSpec);
  }

  /* Prepare the buffer for sending data back to the server */
  rlBufferClear(&theBuffer);
  offset = rlBufferWrite(&theBuffer, offset, &theTaskSpecLength, 1, sizeof(int));
  if (theTaskSpecLength > 0) {
    offset = rlBufferWrite(&theBuffer, offset, theTaskSpec, theTaskSpecLength, sizeof(char));
  }
}

static void onEnvStart(int theConnection) {
  Observation theObservation = env_start();
  unsigned int offset = 0;

  rlBufferClear(&theBuffer);
  offset = rlCopyADTToBuffer(&theObservation, &theBuffer, offset);
}

static void onEnvStep(int theConnection) {
  Reward_observation ro = {0};
  unsigned int offset = 0;

  offset = rlCopyBufferToADT(&theBuffer, offset, &theAction);
  ro = env_step(theAction);

  rlBufferClear(&theBuffer);
  offset = 0;
  offset = rlBufferWrite(&theBuffer, offset, &ro.terminal, 1, sizeof(int));
  offset = rlBufferWrite(&theBuffer, offset, &ro.r, 1, sizeof(Reward));
  offset = rlCopyADTToBuffer(&ro.o, &theBuffer, offset);
}

static void onEnvCleanup(int theConnection) {
  env_cleanup();
  rlBufferClear(&theBuffer);

  free(theAction.intArray);
  free(theAction.doubleArray);
  free(theRandomSeedKey.intArray);
  free(theRandomSeedKey.doubleArray);
  free(theStateKey.intArray);
  free(theStateKey.doubleArray);
  free(theInMessage);

  theAction.intArray           = 0;
  theAction.doubleArray        = 0;
  theRandomSeedKey.intArray    = 0;
  theRandomSeedKey.doubleArray = 0;
  theStateKey.intArray         = 0;
  theStateKey.doubleArray      = 0;

  theAction.numInts = 0;
  theAction.numDoubles = 0;
  theRandomSeedKey.numInts = 0;
  theRandomSeedKey.numDoubles = 0;
  theStateKey.numInts = 0;
  theStateKey.numDoubles = 0;

  theInMessage = 0;
  theInMessageCapacity = 0;
}

static void onEnvSetState(int theConnection) {
  unsigned int offset = 0;

  offset = rlCopyBufferToADT(&theBuffer, offset, &theStateKey);
  env_set_state(theStateKey);

  rlBufferClear(&theBuffer);
}

static void onEnvSetRandomSeed(int theConnection) {
  unsigned int offset = 0;

  offset = rlCopyBufferToADT(&theBuffer, offset, &theRandomSeedKey);  
  env_set_random_seed(theRandomSeedKey);

  rlBufferClear(&theBuffer);
}

static void onEnvGetState(int theConnection) {
  State_key key = env_get_state();
  unsigned int offset = 0;

  rlBufferClear(&theBuffer);
  offset = rlCopyADTToBuffer(&key, &theBuffer, offset);
}

static void onEnvGetRandomSeed(int theConnection) {
  Random_seed_key key = env_get_random_seed();
  unsigned int offset = 0;

  rlBufferClear(&theBuffer);
  rlCopyADTToBuffer(&key, &theBuffer, offset);
}

static void onEnvMessage(int theConnection) {
  unsigned int inMessageLength = 0;
  unsigned int outMessageLength = 0;
  Message inMessage = 0;
  Message outMessage = 0;
  unsigned int offset = 0;

  offset = 0;
  offset = rlBufferRead(&theBuffer, offset, &inMessageLength, 1, sizeof(int));
  if (inMessageLength > theInMessageCapacity) {
    inMessage = (Message)calloc(inMessageLength+1, sizeof(char));
    free(theInMessage);

    theInMessage = inMessage;
    theInMessageCapacity = inMessageLength;
  }

  if (inMessageLength > 0) {
    offset = rlBufferRead(&theBuffer, offset, theInMessage, inMessageLength, sizeof(char));
  }

  outMessage = env_message(theInMessage);

  if (outMessage != NULL) {
   outMessageLength = strlen(outMessage);
  }
  
  /* we want to start sending, so we're going to reset the offset to 0 so we write the the beginning of the buffer */
  rlBufferClear(&theBuffer);
  offset = 0;
  offset = rlBufferWrite(&theBuffer, offset, &outMessageLength, 1, sizeof(int));
  if (outMessageLength > 0) {
    offset = rlBufferWrite(&theBuffer, offset, outMessage, outMessageLength, sizeof(char));
  }
}

static void runEnvironmentEventLoop(int theConnection) {
  int envState = 0;

  do { 
    rlBufferClear(&theBuffer);
    rlRecvBufferData(theConnection, &theBuffer, &envState);

    switch(envState) {
    case kEnvInit:
      onEnvInit(theConnection);
      break;

    case kEnvStart:
      onEnvStart(theConnection);
      break;

    case kEnvStep:
      onEnvStep(theConnection);
      break;

    case kEnvCleanup:
      onEnvCleanup(theConnection);
      break;

    case kEnvSetState:
      onEnvSetState(theConnection);
      break;

    case kEnvSetRandomSeed:
      onEnvSetRandomSeed(theConnection);
      break;

    case kEnvGetState:
      onEnvGetState(theConnection);
      break;

    case kEnvGetRandomSeed:
      onEnvGetRandomSeed(theConnection);
      break;

    case kEnvMessage:
      onEnvMessage(theConnection);
      break;

    case kRLTerm:
      break;

    default:
      fprintf(stderr, kUnknownMessage, envState);
      exit(0);
      break;
    };

    rlSendBufferData(theConnection, &theBuffer, envState);
  } while (envState != kRLTerm);
}

int main(int argc, char** argv) {
  int theConnection = 0;

  const char *usage = "The following environment variables are used by the environment to control its function:\n"
    "RLGLUE_HOST  : If set the environment will use this ip or hostname to connect to rather than %s\n"
    "RLGLUE_PORT  : If set the environment will use this port to connect on rather than %d\n"
    "RLGLUE_AUTORECONNECT  : If set the environment will reconnect to the glue after an experiment has finished\n";
  
  struct hostent *host_ent;

  char* host = kLocalHost;
  short port = kDefaultPort;
  int autoReconnect = 0;

  char* envptr = 0;

  if (argc > 1) {
    fprintf(stderr, usage, kLocalHost, kDefaultPort);
    exit(1);
  }

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

  envptr = getenv("RLGLUE_AUTORECONNECT");
  if (envptr != 0) {
    autoReconnect = strtol(envptr, 0, 10);
  }

  if (isalpha(host[0])) {
    host_ent = gethostbyname(host); 
    host = inet_ntoa(*(struct in_addr*)host_ent->h_addr);
  }

  fprintf(stderr, "Connecting to host=%s on port=%d...", host, port);
	fflush(stderr);


  /* Allocate what should be plenty of space for the buffer - it will dynamically resize if it is too small */
  rlBufferCreate(&theBuffer, 4096);
  
  do {
    theConnection = rlWaitForConnection(host, port, kRetryTimeout);
		fprintf(stderr, "Connected\n");
    rlBufferClear(&theBuffer);
    rlSendBufferData(theConnection, &theBuffer, kEnvironmentConnection);
    runEnvironmentEventLoop(theConnection);
    rlClose(theConnection);
  } while(autoReconnect);

  rlBufferDestroy(&theBuffer);

  return 0;
}
