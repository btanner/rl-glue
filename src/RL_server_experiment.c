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
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif


#include <stdio.h> /* fprintf */
#include <assert.h> /* assert */
#include <signal.h> /* handle ctrl-C */
#include <stdlib.h> /* exit */
#include <string.h> /* strlen, strncmp */

#include <rlglue/Experiment_common.h>
#include <rlglue/network/RL_network.h>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "RL_network_glue.h"

const char* kUnknownMessage = "Unknown message: %s\n";

char* svn_version(){
	return SVN_VERSION;
}

void onRLCleanup(int theConnection);

// #ifdef DEBUG_GLUE_NETWORK
int debug_glue_network=0;
// #else
// int debug_glue_network=0;
// #endif

state_key_t theStateKey = {0};
random_seed_key_t theRandomSeedKey = {0};
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
	unsigned int TS_length=0;
	unsigned int offset=0;
  task_specification_t TS=RL_init();
  rlBufferClear(&theBuffer);
/* Code added by Brian Tanner Oct 13/2007 to address double cleanup problem */
  initNoCleanUp=1;

/* Code added by Brian Tanner Sept 8/2008 to solve issue 35, RL_init should return the task spec */
  if (TS != NULL) {
   TS_length = strlen(TS);
  }
  
  /* we want to start sending, so we're going to reset the offset to 0 so we write the the beginning of the buffer */
  offset = rlBufferWrite(&theBuffer, offset, &TS_length, 1, sizeof(unsigned int));
  if (TS_length > 0) {
    offset = rlBufferWrite(&theBuffer, offset, TS, TS_length, sizeof(char));
  }
}

void onRLStart(int theConnection) {
  unsigned int offset = 0;
  observation_action_t obsAct = RL_start();
	__RL_CHECK_STRUCT(&obsAct.o)
	__RL_CHECK_STRUCT(&obsAct.a)

  rlBufferClear(&theBuffer);
  offset = 0;
  offset = rlCopyADTToBuffer(&obsAct.o, &theBuffer, offset);
  offset = rlCopyADTToBuffer(&obsAct.a, &theBuffer, offset);
}


void onRLStep(int theConnection) {
   reward_observation_action_terminal_t roat = RL_step();
	__RL_CHECK_STRUCT(&roat.o)
	__RL_CHECK_STRUCT(&roat.a)
  unsigned int offset = 0;

  rlBufferClear(&theBuffer);
  offset = 0;
  offset = rlBufferWrite(&theBuffer, offset, &roat.terminal, 1, sizeof(terminal_t));
  offset = rlBufferWrite(&theBuffer, offset, &roat.r, 1, sizeof(reward_t));
  offset = rlCopyADTToBuffer(&roat.o, &theBuffer, offset);
  offset = rlCopyADTToBuffer(&roat.a, &theBuffer, offset);
}

void onRLReturn(int theConnection) {
  reward_t theReward = RL_return();
  unsigned int offset = 0;

  rlBufferClear(&theBuffer);
  offset = rlBufferWrite(&theBuffer, offset, &theReward, 1, sizeof(reward_t));
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
	terminal_t terminal = 0;

	offset = rlBufferRead(&theBuffer, offset, &numSteps, 1, sizeof(unsigned int));

	terminal=RL_episode(numSteps);

	rlBufferClear(&theBuffer);
	/*Brian Sept 8 2008 :: Not really sure if I should be resetting offset to 0 here.  Seems to work*/
	offset=0;
	offset = rlBufferWrite(&theBuffer, offset, &terminal, 1, sizeof(terminal_t));
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
  state_key_t theStateKey = RL_get_state();

  rlBufferClear(&theBuffer);
  offset = rlCopyADTToBuffer(&theStateKey, &theBuffer, offset);
}

void onRLGetRandomSeed(int theConnection) {
  unsigned int offset = 0;
  random_seed_key_t theRandomSeedKey = RL_get_random_seed();

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

void onRLAgentMessage(int theConnection) {
  char* inMessage = 0;
  char* outMessage = 0;
  unsigned int inMessageLength = 0;
  unsigned int outMessageLength = 0;
  unsigned int offset = 0;

  offset = 0;
  offset = rlBufferRead(&theBuffer, offset, &inMessageLength, 1, sizeof(int));

  inMessage = (char*)calloc(inMessageLength+1, sizeof(char));
  if (inMessageLength > 0) {
    offset = rlBufferRead(&theBuffer, offset, inMessage, inMessageLength, sizeof(char));
  }
  /* Sept 12 2008 moved out of if to make sure it is null terminated if empty message*/
  inMessage[inMessageLength] = '\0';
  
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

  /* make a buffer to handle the message received from the experiment (maybe of size 1 if its an empty message)*/
  inMessage = (char*)calloc(inMessageLength+1, sizeof(char));
  if (inMessageLength > 0) {
    offset = rlBufferRead(&theBuffer, offset, inMessage, inMessageLength, sizeof(char));
  }
  /* Sept 12 2008 moved out of if to make sure it is null terminated if empty message*/
  inMessage[inMessageLength] = '\0';

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
	if(debug_glue_network)printf("\tDEBUG: kRLInit\n");
      onRLInit(theConnection);
      break;
      
    case kRLStart:
	if(debug_glue_network)printf("\tDEBUG: kRLStart\n");
      onRLStart(theConnection);
      break;
      
    case kRLStep:
	if(debug_glue_network)printf("\tDEBUG: kRLStep\n");
      onRLStep(theConnection);
      break;
      
    case kRLReturn:
	if(debug_glue_network)printf("\tDEBUG: kRLReturn\n");
      onRLReturn(theConnection);
      break;
      
    case kRLCleanup:
	if(debug_glue_network)printf("\tDEBUG: kRLCleanup\n");
      onRLCleanup(theConnection);
      break;
      
    case kRLNumSteps:
	if(debug_glue_network)printf("\tDEBUG: kRLNumSteps\n");
      onRLNumSteps(theConnection);
      break;
      
    case kRLNumEpisodes:
	if(debug_glue_network)printf("\tDEBUG: kRLNumEpisodes\n");
      onRLNumEpisodes(theConnection);
      break;
      
    case kRLEpisode:
	if(debug_glue_network)printf("\tDEBUG: kRLEpisode\n");
      onRLEpisode(theConnection);
      break;
      
    case kRLSetState:
	if(debug_glue_network)printf("\tDEBUG: kRLSetState\n");
      onRLSetState(theConnection);
      break;
      
    case kRLSetRandomSeed:
	if(debug_glue_network)printf("\tDEBUG: kRLSetRandomSeed\n");
      onRLSetRandomSeed(theConnection);
      break;
      
    case kRLGetState:
	if(debug_glue_network)printf("\tDEBUG: kRLGetState\n");
      onRLGetState(theConnection);
      break;
      
    case kRLGetRandomSeed:
	if(debug_glue_network)printf("\tDEBUG: kRLGetRandomSeed\n");
      onRLGetRandomSeed(theConnection);
      break;

    case kRLAgentMessage:
	if(debug_glue_network)printf("\tDEBUG: kRLAgentMessage\n");
      onRLAgentMessage(theConnection);
      break;

    case kRLEnvMessage:
	if(debug_glue_network)printf("\tDEBUG: kRLEnvMessage\n");
      onRLEnvMessage(theConnection);
      break;

    case kRLTerm:
	if(debug_glue_network)printf("\tDEBUG: kRLTerm\n");
      break;

    default:
	if(debug_glue_network)printf("\tDEBUG: kUnknownMessage\n");
      fprintf(stderr, kUnknownMessage, glueState);
      break;
    };

    rlSendBufferData(theConnection, &theBuffer, glueState);
  } while (glueState != kRLTerm);
}

int main(int argc, char** argv) {
	char usageBuffer[1024];
	
	fprintf(stdout, "RL-Glue Version %s, Build %s\n", VERSION,SVN_VERSION);
	fflush(stdout);
  
  snprintf(usageBuffer,1024,"\n\trl_glue version\t=\t%s\n\tbuild number\t=\t%s\n\nUsage: $:>rl_glue\n\n  By default rl_glue listens on port 4096.\n  To choose a different port, set environment variable RLGLUE_PORT.\n\n",VERSION,SVN_VERSION);

  if (argc > 1) {
    fprintf(stdout, usageBuffer);
    exit(1);
  }

  rlBufferCreate(&theBuffer, 65536);

  theConnection = rlConnectSystems();
  assert(rlIsValidSocket(theConnection));
  runGlueEventLoop(theConnection);
  rlDisconnectSystems();

  rlBufferDestroy(&theBuffer);

  return 0;
}
