#include <stdio.h> /* fprintf */
#include <assert.h> /* assert */
#include <signal.h> /* handle ctrl-C */
#include <stdlib.h> /* exit */
#include <string.h> /* strlen, strncmp */

#include <RL_common.h>
#include <Network/RL_network.h>

const char* kUnknownMessage = "Unknown Message: %s\n";

extern int rlConnectSystems();

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

void onRLCleanup(rlSocket theConnection);

extern rlSocket theExperimentConnection;
State_key theStateKey = {0};
Random_seed_key theRandomSeedKey = {0};
rlBuffer theBuffer = {0};

void termination_handler(int signum) {
  onRLCleanup(theExperimentConnection);
  if (theExperimentConnection != 0) {
    rlClose(theExperimentConnection);
  }
  rlBufferDestroy(&theBuffer);
  exit(0);
}

void onRLInit(rlSocket theConnection) {
  RL_init();
  rlBufferClear(&theBuffer);
}

void onRLStart(rlSocket theConnection) {
  unsigned int offset = 0;
  Observation_action obsAct = RL_start();

  rlBufferClear(&theBuffer);
  offset = 0;
  offset = rlCopyADTToBuffer(&obsAct.o, &theBuffer, offset);
  offset = rlCopyADTToBuffer(&obsAct.a, &theBuffer, offset);
}

void onRLStep(rlSocket theConnection) {
  Reward_observation_action_terminal roat = RL_step();
  unsigned int offset = 0;

  rlBufferClear(&theBuffer);
  offset = 0;
  offset = rlBufferWrite(&theBuffer, offset, &roat.terminal, 1, sizeof(int));
  offset = rlBufferWrite(&theBuffer, offset, &roat.r, 1, sizeof(Reward));
  offset = rlCopyADTToBuffer(&roat.o, &theBuffer, offset);
  offset = rlCopyADTToBuffer(&roat.a, &theBuffer, offset);
}

void onRLReturn(rlSocket theConnection) {
  Reward theReward = RL_return();
  unsigned int offset = 0;

  rlBufferClear(&theBuffer);
  offset = rlBufferWrite(&theBuffer, offset, &theReward, 1, sizeof(Reward));
}

void onRLNumSteps(rlSocket theConnection) {
  int numSteps = RL_num_steps();
  unsigned int offset = 0;

  rlBufferClear(&theBuffer);
  offset = rlBufferWrite(&theBuffer, offset, &numSteps, 1, sizeof(int));
}

void onRLNumEpisodes(rlSocket theConnection) {
  int numEpisodes = RL_num_episodes();
  unsigned int offset = 0;

  rlBufferClear(&theBuffer);
  offset = rlBufferWrite(&theBuffer, offset, &numEpisodes, 1, sizeof(int));
}

void onRLEpisode(rlSocket theConnection) {
  unsigned int numSteps = 0;
  unsigned int offset = 0;
  offset = rlBufferRead(&theBuffer, offset, &numSteps, 1, sizeof(unsigned int));

  RL_episode(numSteps);

  rlBufferClear(&theBuffer);
}

void onRLSetState(rlSocket theConnection) {
  unsigned int offset = 0;
  offset = rlCopyBufferToADT(&theBuffer, offset, &theStateKey);

  RL_set_state(theStateKey);
  
  rlBufferClear(&theBuffer);
}

void onRLSetRandomSeed(rlSocket theConnection) {
  unsigned int offset = 0;
  offset = rlCopyBufferToADT(&theBuffer, offset, &theRandomSeedKey);
  
  RL_set_random_seed(theRandomSeedKey);

  rlBufferClear(&theBuffer);
}

void onRLGetState(rlSocket theConnection) {
  unsigned int offset = 0;
  State_key theStateKey = RL_get_state();

  rlBufferClear(&theBuffer);
  offset = rlCopyADTToBuffer(&theStateKey, &theBuffer, offset);
}

void onRLGetRandomSeed(rlSocket theConnection) {
  unsigned int offset = 0;
  Random_seed_key theRandomSeedKey = RL_get_random_seed();

  rlBufferClear(&theBuffer);
  offset = rlCopyADTToBuffer(&theRandomSeedKey, &theBuffer, offset);
}

void onRLCleanup(rlSocket theConnection) {
  RL_cleanup();

  rlBufferClear(&theBuffer);
  
  free(theStateKey.intArray);
  free(theStateKey.doubleArray);
  free(theRandomSeedKey.intArray);
  free(theRandomSeedKey.doubleArray);

  theStateKey.intArray = 0;
  theStateKey.doubleArray = 0;

  theRandomSeedKey.intArray = 0;
  theRandomSeedKey.doubleArray = 0;
  
  theStateKey.numInts = 0;
  theStateKey.numDoubles = 0;

  theRandomSeedKey.numInts = 0;
  theRandomSeedKey.numDoubles = 0;
}

void onRLFreeze(rlSocket theConnection) {
  RL_freeze();

  rlBufferClear(&theBuffer);
}

void onRLAgentMessage(rlSocket theConnection) {
  char* inMessage = 0;
  char* outMessage = 0;
  unsigned int messageLength = 0;
  unsigned int offset = 0;

  offset = 0;
  offset = rlBufferRead(&theBuffer, offset, &messageLength, 1, sizeof(int));

  if (messageLength > 0) {
    inMessage = (char*)calloc(messageLength+1, sizeof(char));
    offset = rlBufferRead(&theBuffer, offset, inMessage, messageLength, sizeof(char));
  }
  
  outMessage = RL_agent_message(inMessage);
  if (outMessage != 0) {
    messageLength = strlen(outMessage);
  }

  offset = 0;
  rlBufferClear(&theBuffer);
  offset = rlBufferWrite(&theBuffer, offset, &messageLength, 1, sizeof(int));
  if (messageLength > 0) {
    offset = rlBufferWrite(&theBuffer, offset, outMessage, messageLength, sizeof(char));
  } 

  free(inMessage);
  inMessage = 0;
}

void onRLEnvMessage(rlSocket theConnection) {
  char* inMessage = 0;
  char* outMessage = 0;
  unsigned int messageLength = 0;
  unsigned int offset = 0;

  offset = 0;
  offset = rlBufferRead(&theBuffer, offset, &messageLength, 1, sizeof(int));

  if (messageLength > 0) {
    inMessage = (char*)calloc(messageLength+1, sizeof(char));
    offset = rlBufferRead(&theBuffer, offset, inMessage, messageLength, sizeof(char));
  }
  
  outMessage = RL_env_message(inMessage);
  if (outMessage != 0) {
    messageLength = strlen(outMessage);
  }

  rlBufferClear(&theBuffer);
  offset = 0;
  offset = rlBufferWrite(&theBuffer, offset, &messageLength, 1, sizeof(int));
  if (messageLength > 0) {
    offset = rlBufferWrite(&theBuffer, offset, outMessage, messageLength, sizeof(char));
  } 

  free(inMessage);
}

void runGlueEventLoop(rlSocket theConnection) {
  int glueState = 0;

  do { 
    rlBufferClear(&theBuffer);
    rlRecvBufferData(theConnection, &theBuffer, &glueState);

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

    default:
      fprintf(stderr, kUnknownMessage, glueState);
      break;
    };

    rlSendBufferData(theConnection, &theBuffer, glueState);
  } while (glueState != kRLCleanup);
}

int main(int argc, char** argv) {
  rlSocket theConnection = 0;
  int autoReconnect = 0;
  char* envptr = 0;

  envptr = getenv("RLGLUE_AUTORECONNECT");
  if (envptr != 0) {
    autoReconnect = strtol(envptr, 0, 10);
  }
  fprintf(stderr, "autoreconnect=%d\n", autoReconnect);

  rlBufferCreate(&theBuffer, 4096);

  do {
    if (theExperimentConnection == -1) {
      theExperimentConnection = 0;
    }
    theConnection = rlConnectSystems();
    assert(rlIsValidSocket(theConnection));
    runGlueEventLoop(theConnection);
    rlClose(theConnection);
    theExperimentConnection = 0;
  } while (autoReconnect);

  rlBufferDestroy(&theBuffer);

  return 0;
}
