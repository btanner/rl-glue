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

#include <stdio.h> /* fprintf */
#include <assert.h> /* assert */
#include <signal.h> /* handle ctrl-C */
#include <stdlib.h> /* exit */
#include <string.h> /* strlen, strncmp */

#include <RL_common.h>
#include <RL_network.h>

const char* kUnknownMessage = "Unknown Message: %s\n";

extern int rlConnectSystems();
extern void rlDisconnectSystems();

extern Task_specification RL_init();
extern Observation_action RL_start();
extern Reward_observation_action_terminal RL_step();
extern Reward RL_return();
extern int RL_num_steps();
extern int RL_num_episodes();
extern void RL_episode(unsigned int num_steps);
extern void RL_set_state(State_key sk);
extern void RL_set_random_seed(Random_seed_key rsk);
extern State_key RL_get_state();
extern Random_seed_key RL_get_random_seed();
extern void RL_cleanup();
extern void RL_freeze();
extern Message RL_agent_message(const Message message);
extern Message RL_env_message(const Message message);

void onRLCleanup(int theConnection);

State_key theStateKey = {0};
Random_seed_key theRandomSeedKey = {0};
rlBuffer theBuffer = {0};
int theConnection = 0;

/* Code added by Brian Tanner Oct 13/2007 to address double cleanup problem */
unsigned short initNoCleanUp=0;

void termination_handler(int signum) {
  fprintf(stderr, "Signal: %d has killed this process. Cleaning Up And Exiting....\n", signum);

/* Code added by Brian Tanner Oct 13/2007 to address double cleanup problem */
  if(initNoCleanUp==1){
  	onRLCleanup(theConnection);
	initNoCleanUp=0;
  }
  if (theConnection != 0) {
    rlClose(theConnection);
  }
  rlBufferDestroy(&theBuffer);
  exit(0);
}

void onRLInit(int theConnection) {
  RL_init();
  rlBufferClear(&theBuffer);
/* Code added by Brian Tanner Oct 13/2007 to address double cleanup problem */
  initNoCleanUp=1;
}

void onRLStart(int theConnection) {
  unsigned int offset = 0;
  Observation_action obsAct = RL_start();

  rlBufferClear(&theBuffer);
  offset = 0;
  offset = rlCopyADTToBuffer(&obsAct.o, &theBuffer, offset);
  offset = rlCopyADTToBuffer(&obsAct.a, &theBuffer, offset);
}

void onRLStep(int theConnection) {
  Reward_observation_action_terminal roat = RL_step();
  unsigned int offset = 0;

  rlBufferClear(&theBuffer);
  offset = 0;
  offset = rlBufferWrite(&theBuffer, offset, &roat.terminal, 1, sizeof(int));
  offset = rlBufferWrite(&theBuffer, offset, &roat.r, 1, sizeof(Reward));
  offset = rlCopyADTToBuffer(&roat.o, &theBuffer, offset);
  offset = rlCopyADTToBuffer(&roat.a, &theBuffer, offset);
}

void onRLReturn(int theConnection) {
  Reward theReward = RL_return();
  unsigned int offset = 0;

  rlBufferClear(&theBuffer);
  offset = rlBufferWrite(&theBuffer, offset, &theReward, 1, sizeof(Reward));
}

void onRLNumSteps(int theConnection) {
  int numSteps = RL_num_steps();
  unsigned int offset = 0;

  rlBufferClear(&theBuffer);
  offset = rlBufferWrite(&theBuffer, offset, &numSteps, 1, sizeof(int));
}

void onRLNumEpisodes(int theConnection) {
  int numEpisodes = RL_num_episodes();
  unsigned int offset = 0;

  rlBufferClear(&theBuffer);
  offset = rlBufferWrite(&theBuffer, offset, &numEpisodes, 1, sizeof(int));
}

void onRLEpisode(int theConnection) {
  unsigned int numSteps = 0;
  unsigned int offset = 0;
  offset = rlBufferRead(&theBuffer, offset, &numSteps, 1, sizeof(unsigned int));

  RL_episode(numSteps);

  rlBufferClear(&theBuffer);
}

void onRLSetState(int theConnection) {
  unsigned int offset = 0;
  offset = rlCopyBufferToADT(&theBuffer, offset, &theStateKey);

  RL_set_state(theStateKey);
  
  rlBufferClear(&theBuffer);
}

void onRLSetRandomSeed(int theConnection) {
  unsigned int offset = 0;
  offset = rlCopyBufferToADT(&theBuffer, offset, &theRandomSeedKey);
  
  RL_set_random_seed(theRandomSeedKey);

  rlBufferClear(&theBuffer);
}

void onRLGetState(int theConnection) {
  unsigned int offset = 0;
  State_key theStateKey = RL_get_state();

  rlBufferClear(&theBuffer);
  offset = rlCopyADTToBuffer(&theStateKey, &theBuffer, offset);
}

void onRLGetRandomSeed(int theConnection) {
  unsigned int offset = 0;
  Random_seed_key theRandomSeedKey = RL_get_random_seed();

  rlBufferClear(&theBuffer);
  offset = rlCopyADTToBuffer(&theRandomSeedKey, &theBuffer, offset);
}

void onRLCleanup(int theConnection) {
  RL_cleanup();

  rlBufferClear(&theBuffer);
  
  /* Cleanup the state key */
  if (theStateKey.intArray != 0) {
    free(theStateKey.intArray);
    theStateKey.intArray = 0;

/* Code added by Brian Tanner Oct 13/2007 to address double cleanup problem */
  initNoCleanUp=0;
  }

  if (theStateKey.doubleArray != 0) {
    free(theStateKey.doubleArray);
    theStateKey.doubleArray = 0;
  }

  theStateKey.numInts = 0;
  theStateKey.numDoubles = 0;

  /* Cleanup the random seed key */
  if (theRandomSeedKey.intArray != 0) {
    free(theRandomSeedKey.intArray);
    theRandomSeedKey.intArray = 0;
  }

  if (theRandomSeedKey.doubleArray != 0) {
    free(theRandomSeedKey.doubleArray);
    theRandomSeedKey.doubleArray = 0;
  }

  theRandomSeedKey.numInts = 0;
  theRandomSeedKey.numDoubles = 0;
}

void onRLFreeze(int theConnection) {
  RL_freeze();

  rlBufferClear(&theBuffer);
}

void onRLAgentMessage(int theConnection) {
  char* inMessage = 0;
  char* outMessage = 0;
  unsigned int inMessageLength = 0;
  unsigned int outMessageLength = 0;
  unsigned int offset = 0;

  offset = 0;
  offset = rlBufferRead(&theBuffer, offset, &inMessageLength, 1, sizeof(int));

  if (inMessageLength > 0) {
    inMessage = (char*)calloc(inMessageLength+1, sizeof(char));
    offset = rlBufferRead(&theBuffer, offset, inMessage, inMessageLength, sizeof(char));
    inMessage[inMessageLength] = '\0';
  }
  
  outMessage = RL_agent_message(inMessage);    

  if (outMessage != 0) {
    outMessageLength = strlen(outMessage);
  }

  offset = 0;
  rlBufferClear(&theBuffer);

  offset = rlBufferWrite(&theBuffer, offset, &outMessageLength, 1, sizeof(int));
  if (outMessageLength > 0) {
    offset = rlBufferWrite(&theBuffer, offset, outMessage, outMessageLength, sizeof(char));
  } 

  free(inMessage);
  inMessage = 0;
}

void onRLEnvMessage(int theConnection) {
  char* inMessage = 0;
  char* outMessage = 0;
  unsigned int inMessageLength = 0;
  unsigned int outMessageLength = 0;
  unsigned int offset = 0;

  offset = 0;
  offset = rlBufferRead(&theBuffer, offset, &inMessageLength, 1, sizeof(int));

  if (inMessageLength > 0) {
    inMessage = (char*)calloc(inMessageLength+1, sizeof(char));
    offset = rlBufferRead(&theBuffer, offset, inMessage, inMessageLength, sizeof(char));
    inMessage[inMessageLength] = '\0';
  }
  
  outMessage = RL_env_message(inMessage);
  if (outMessage != 0) {
    outMessageLength = strlen(outMessage);
  }

  rlBufferClear(&theBuffer);
  offset = 0;
  offset = rlBufferWrite(&theBuffer, offset, &outMessageLength, 1, sizeof(int));
  if (outMessageLength > 0) {
    offset = rlBufferWrite(&theBuffer, offset, outMessage, outMessageLength, sizeof(char));
  }

  free(inMessage);
}

void runGlueEventLoop(int theConnection) {
  int glueState = 0;
  
  do { 
    rlBufferClear(&theBuffer);
    if (rlRecvBufferData(theConnection, &theBuffer, &glueState) == 0)
      break;

    switch(glueState) {
    case kRLInit:
      onRLInit(theConnection);
      break;
      
    case kRLStart:
      onRLStart(theConnection);
      break;
      
    case kRLStep:
      onRLStep(theConnection);
      break;
      
    case kRLReturn:
      onRLReturn(theConnection);
      break;
      
    case kRLCleanup:
      onRLCleanup(theConnection);
      break;
      
    case kRLNumSteps:
      onRLNumSteps(theConnection);
      break;
      
    case kRLNumEpisodes:
      onRLNumEpisodes(theConnection);
      break;
      
    case kRLEpisode:
      onRLEpisode(theConnection);
      break;
      
    case kRLSetState:
      onRLSetState(theConnection);
      break;
      
    case kRLSetRandomSeed:
      onRLSetRandomSeed(theConnection);
      break;
      
    case kRLGetState:
      onRLGetState(theConnection);
      break;
      
    case kRLGetRandomSeed:
      onRLGetRandomSeed(theConnection);
      break;
      
    case kRLFreeze:
      onRLFreeze(theConnection);
      break;

    case kRLAgentMessage:
      onRLAgentMessage(theConnection);
      break;

    case kRLEnvMessage:
      onRLEnvMessage(theConnection);
      break;

    case kRLTerm:
      break;

    default:
      fprintf(stderr, kUnknownMessage, glueState);
      break;
    };

    rlSendBufferData(theConnection, &theBuffer, glueState);
  } while (glueState != kRLTerm);
}

int main(int argc, char** argv) {
  int autoReconnect = 0;
  char* envptr = 0;

  const char *usage = "The following environment variables are used by the glue to control its function:\n"
    "RLGLUE_AUTORECONNECT  : If set glue will continue to run after an experiment has finished\n";

  if (argc > 1) {
    fprintf(stderr, usage);
    exit(1);
  }

  envptr = getenv("RLGLUE_AUTORECONNECT");
  if (envptr != 0) {
    autoReconnect = strtol(envptr, 0, 10);
  }

  rlBufferCreate(&theBuffer, 65536);

  do {
    theConnection = rlConnectSystems();
    assert(rlIsValidSocket(theConnection));
    runGlueEventLoop(theConnection);
    rlDisconnectSystems();
  } while (autoReconnect);

  rlBufferDestroy(&theBuffer);

  return 0;
}
