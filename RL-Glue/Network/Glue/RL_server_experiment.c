#include <stdio.h> /* fprintf */
#include <assert.h> /* assert */
#include <signal.h> /* handle ctrl-C */
#include <stdlib.h> /* exit */
#include <string.h> /* strlen, strncmp */
#include <unistd.h> /* getopt */
#include <ctype.h> /* isdigit */

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>


#include <RL_common.h>
#include <Network/RL_netlib.h>

const char* kUnknownMessage = "Unknown Message: %s\n";

extern int rlConnectSystems();

extern Task_specification RL_init();
extern Observation_action RL_start();
extern Reward_observation_action_terminal RL_step();
extern Reward RL_return();
extern int RL_num_steps();
extern int RL_num_episodes();
extern void RL_episode(int num_steps);
extern void RL_set_state(State_key sk);
extern void RL_set_random_seed(Random_seed_key rsk);
extern State_key RL_get_state();
extern Random_seed_key RL_get_random_seed();
extern void RL_cleanup();
extern void RL_freeze();
extern char* RL_agent_message(const char* message);
extern char* RL_env_message(const char* message);

extern short rlConnectPort;

void onRLCleanup(rlSocket theConnection);

int theExperimentConnection = 0;
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

void rlSetExperimentConnection(int theConnection) {
  if (theExperimentConnection) {
    rlClose(theExperimentConnection);
  }

  theExperimentConnection = theConnection;
}

int rlIsExperimentConnected() {
  return theExperimentConnection != 0;
}


void onRLInit(rlSocket theConnection) {
  RL_init();
}

void onRLStart(rlSocket theConnection) {
  Observation_action obsAct = RL_start();

  rlBufferClear(&theBuffer);
  rlCopyADTToBuffer(&obsAct.o, &theBuffer);
  rlSendBufferData(theConnection, &theBuffer);

  rlBufferClear(&theBuffer);
  rlCopyADTToBuffer(&obsAct.a, &theBuffer);
  rlSendBufferData(theConnection, &theBuffer);
}

void onRLStep(rlSocket theConnection) {
  Reward_observation_action_terminal roat = RL_step();
  int offset = 0;

  offset = 0;
  rlBufferClear(&theBuffer);
  rlRecvBufferData(theConnection, &theBuffer);
  offset = rlBufferRead(&theBuffer, offset, &roat.terminal, 1, sizeof(int));
  offset = rlBufferRead(&theBuffer, offset, &roat.r, 1, sizeof(Reward));
  rlSendBufferData(theConnection, &theBuffer);

  rlBufferClear(&theBuffer);
  rlCopyADTToBuffer(&roat.o, &theBuffer);
  rlSendBufferData(theConnection, &theBuffer);

  rlBufferClear(&theBuffer);
  rlCopyADTToBuffer(&roat.a, &theBuffer);
  rlSendBufferData(theConnection, &theBuffer);
}

void onRLReturn(rlSocket theConnection) {
  Reward theReward = RL_return();
  
  rlBufferClear(&theBuffer);
  rlBufferWrite(&theBuffer, 0, &theReward, 1, sizeof(Reward));
  rlSendBufferData(theConnection, &theBuffer);
}

void onRLNumSteps(rlSocket theConnection) {
  int numSteps = RL_num_steps();

  rlBufferClear(&theBuffer);
  rlBufferWrite(&theBuffer, 0, &numSteps, 1, sizeof(int));
  rlSendBufferData(theConnection, &theBuffer);
}

void onRLNumEpisodes(rlSocket theConnection) {
  int numEpisodes = RL_num_episodes();

  rlBufferClear(&theBuffer);
  rlBufferWrite(&theBuffer, 0, &numEpisodes, 1, sizeof(int));
  rlSendBufferData(theConnection, &theBuffer);
}

void onRLEpisode(rlSocket theConnection) {
  int numEpisodes = 0;
  
  rlBufferClear(&theBuffer);
  rlRecvBufferData(theConnection, &theBuffer);
  rlBufferRead(&theBuffer, 0, &numEpisodes, 1, sizeof(int));

  RL_episode(numEpisodes);
}

void onRLSetState(rlSocket theConnection) {
  rlBufferClear(&theBuffer);
  rlRecvBufferData(theConnection, &theBuffer);
  rlCopyBufferToADT(&theBuffer, &theStateKey);
  
  RL_set_state(theStateKey);
}

void onRLSetRandomSeed(rlSocket theConnection) {
  rlBufferClear(&theBuffer);
  rlRecvBufferData(theConnection, &theBuffer);
  rlCopyBufferToADT(&theBuffer, &theRandomSeedKey);
  
  RL_set_random_seed(theRandomSeedKey);
}

void onRLGetState(rlSocket theConnection) {
  State_key theStateKey = RL_get_state();

  rlBufferClear(&theBuffer);
  rlCopyADTToBuffer(&theStateKey, &theBuffer);
  rlSendBufferData(theConnection, &theBuffer);
}

void onRLGetRandomSeed(rlSocket theConnection) {
  Random_seed_key theRandomSeedKey = RL_get_random_seed();

  rlBufferClear(&theBuffer);
  rlCopyADTToBuffer(&theRandomSeedKey, &theBuffer);
  rlSendBufferData(theConnection, &theBuffer);
}

void onRLCleanup(rlSocket theConnection) {
  RL_cleanup();
  
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
}

void onRLAgentMessage(rlSocket theConnection) {
  char* inMessage = 0;
  char* outMessage = 0;
  int messageLength = 0;
  int offset = 0;

  rlBufferClear(&theBuffer);
  rlRecvBufferData(theConnection, &theBuffer);
  
  offset = 0;
  offset = rlBufferRead(&theBuffer, offset, &messageLength, 1, sizeof(int));

  if (messageLength > 0) {
    inMessage = (char*)calloc(messageLength, sizeof(char));
    offset = rlBufferRead(&theBuffer, offset, inMessage, messageLength, sizeof(char));
  }
  
  outMessage = RL_agent_message(inMessage);
  if (outMessage != 0) {
    messageLength = strlen(outMessage) + 1;
  }

  offset = 0;
  rlBufferClear(&theBuffer);
  offset = rlBufferWrite(&theBuffer, offset, &messageLength, 1, sizeof(int));
  if (messageLength > 0) {
    offset = rlBufferWrite(&theBuffer, offset, outMessage, messageLength, sizeof(char));
  } 
  rlSendBufferData(theConnection, &theBuffer);

  free(inMessage);
}

void onRLEnvMessage(rlSocket theConnection) {
  char* inMessage = 0;
  char* outMessage = 0;
  int messageLength = 0;
  int offset = 0;

  rlBufferClear(&theBuffer);
  rlRecvBufferData(theConnection, &theBuffer);
  
  offset = 0;
  offset = rlBufferRead(&theBuffer, offset, &messageLength, 1, sizeof(int));

  if (messageLength > 0) {
    inMessage = (char*)calloc(messageLength, sizeof(char));
    offset = rlBufferRead(&theBuffer, offset, inMessage, messageLength, sizeof(char));
  }
  
  outMessage = RL_env_message(inMessage);
  if (outMessage != 0) {
    messageLength = strlen(outMessage) + 1;
  }

  offset = 0;
  rlBufferClear(&theBuffer);
  offset = rlBufferWrite(&theBuffer, offset, &messageLength, 1, sizeof(int));
  if (messageLength > 0) {
    offset = rlBufferWrite(&theBuffer, offset, outMessage, messageLength, sizeof(char));
  } 
  rlSendBufferData(theConnection, &theBuffer);

  free(inMessage);
}

void runGlueEventLoop(rlSocket theConnection) {
  int glueState = 0;

  do { 
    rlBufferClear(&theBuffer);
    rlRecvBufferData(theConnection, &theBuffer);
    rlBufferRead(&theBuffer, 0, &glueState, 1, sizeof(int));

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
  } while (glueState != kRLCleanup);
}

int main(int argc, char** argv) {
  rlSocket theConnection = 0;

  char* host = kLocalHost;
  short port = kDefaultPort;
  int autoReconnect = 0;

  host = getenv("RLGLUE_HOST");
  if (host == 0) {
    host = kLocalHost;
  }

  port = strtol(getenv("RLGLUE_PORT"), 0, 10);
  if (port == 0) {
    port = kDefaultPort;
  }

  autoReconnect = strtol(getenv("RLGLUE_AUTORECONNECT"), 0, 10);

  rlBufferCreate(&theBuffer, 4096);

  do {
    theConnection = rlConnectSystems();
    assert(rlIsValidSocket(theConnection));
    runGlueEventLoop(theConnection);
    rlClose(theConnection);
    theExperimentConnection = 0;
  } while (autoReconnect);

  rlBufferDestroy(&theBuffer);

  return 0;
}
