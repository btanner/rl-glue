#include <assert.h> /* assert  */
#include <stdlib.h> /* malloc, exit */
#include <stdio.h>  /* fprintf */
#include <unistd.h> /* sleep   */
#include <string.h> /* strlen */

#include <ctype.h> /* isdigit */
#include <netdb.h> /* gethostbyname */
#include <arpa/inet.h> /* inet_ntoa */

#include <RL_common.h>
#include <Network/RL_network.h>

/* Provide forward declaration of agent interface */
extern void agent_init(const Task_specification task_spec);
extern Action agent_start(Observation o);
extern Action agent_step(Reward r, Observation o);
extern void agent_end(Reward r);
extern void agent_cleanup();
extern void agent_freeze();
extern Message agent_message(const Message inMessage);

static const char* kUnknownMessage = "Unknown Message: %d\n";

static char* theTaskSpec = 0;
static Observation theObservation = {0};
static rlBuffer theBuffer = {0};
static Message theInMessage = 0;
static unsigned int theInMessageCapacity = 0;

static void onAgentInit(rlSocket theConnection) {
  unsigned int theTaskSpecLength = 0;
  unsigned int offset = 0;

  /* Read the data in the buffer (data from server) */
  offset = rlBufferRead(&theBuffer, offset, &theTaskSpecLength, 1, sizeof(int));
  if (theTaskSpecLength > 0) {
    theTaskSpec = (char*)calloc(theTaskSpecLength+1, sizeof(char));
    offset = rlBufferRead(&theBuffer, offset, theTaskSpec, theTaskSpecLength, sizeof(char));
  }

  /* Call RL method on the recv'd data */
  agent_init(theTaskSpec);

  /* Prepare the buffer for sending data back to the server */
  rlBufferClear(&theBuffer);
}

static void onAgentStart(rlSocket theConnection) {
  Action theAction = {0};
  unsigned int offset = 0;

  /* Read the data in the buffer (data from server) */
  offset = rlCopyBufferToADT(&theBuffer, offset, &theObservation);

  /* Call RL method on the recv'd data */
  theAction = agent_start(theObservation);

  /* Prepare the buffer for sending data back to the server */
  rlBufferClear(&theBuffer);
  offset = 0;
  offset = rlCopyADTToBuffer(&theAction, &theBuffer, offset);
}

static void onAgentStep(rlSocket theConnection) {
  Reward theReward = 0;
  Action theAction = {0};
  unsigned int offset = 0;

  /* Read the data in the buffer (data from server) */
  offset = rlBufferRead(&theBuffer, offset, &theReward, 1, sizeof(theReward));
  offset = rlCopyBufferToADT(&theBuffer, offset, &theObservation);

  /* Call RL method on the recv'd data */
  theAction = agent_step(theReward, theObservation);

  /* Prepare the buffer for sending data back to the server */
  rlBufferClear(&theBuffer);
  offset = 0;
  rlCopyADTToBuffer(&theAction, &theBuffer, offset);
}

static void onAgentEnd(rlSocket theConnection) {
  Reward theReward = 0;
  unsigned int offset = 0;

  /* Read the data in the buffer (data from server) */
  offset = rlBufferRead(&theBuffer, offset, &theReward, 1, sizeof(Reward));

  /* Call RL method on the recv'd data */
  agent_end(theReward);

  /* Prepare the buffer for sending data back to the server */
  rlBufferClear(&theBuffer);
}

static void onAgentCleanup(rlSocket theConnection) {
  /* Read the data in the buffer (data from server) */
  /* No data sent for agent cleanup */

  /* Call RL method on the recv'd data */
  agent_cleanup();

  /* Prepare the buffer for sending data back to the server */
  rlBufferClear(&theBuffer);

  /* Cleanup our resources */
  free(theObservation.intArray);
  free(theObservation.doubleArray);
  free(theTaskSpec);
  free(theInMessage);
  
  theObservation.numInts    = 0;
  theObservation.numDoubles = 0;
  theObservation.intArray   = 0;
  theObservation.doubleArray= 0;
  theTaskSpec = 0;
  theInMessage = 0;
  theInMessageCapacity = 0;
}

static void onAgentFreeze(rlSocket theConnection) {
  /* Read the data in the buffer (data from server) */
  /* No data sent for agent cleanup */

  /* Call RL method on the recv'd data */
  agent_freeze();

  /* Prepare the buffer for sending data back to the server */
  rlBufferClear(&theBuffer);
}

static void onAgentMessage(rlSocket theConnection) {
  unsigned int inMessageLength = 0;
  unsigned int outMessageLength = 0;
  Message inMessage = 0;
  Message outMessage = 0;
  unsigned int offset = 0;

  /* Read the data in the buffer (data from server) */
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

  /* Call RL method on the recv'd data */
  outMessage = agent_message(theInMessage);
  if (outMessage != NULL) {
   outMessageLength = strlen(outMessage);
  }
  
  /* Prepare the buffer for sending data back to the server */
  /* we want to start sending, so we're going to reset the offset to 0 so we write to the beginning of the buffer */
  rlBufferClear(&theBuffer);
  offset = 0;
  offset = rlBufferWrite(&theBuffer, offset, &outMessageLength, 1, sizeof(int)); 
  if (outMessageLength > 0) {
    offset = rlBufferWrite(&theBuffer, offset, outMessage, outMessageLength, sizeof(char));
  }
}

static void runAgentEventLoop(rlSocket theConnection) {
  int agentState = 0;

  do {
    rlBufferClear(&theBuffer);
    rlRecvBufferData(theConnection, &theBuffer, &agentState);

    switch(agentState) {
    case kAgentInit:
      onAgentInit(theConnection);
      break;

    case kAgentStart:
      onAgentStart(theConnection);
      break;

    case kAgentStep:
      onAgentStep(theConnection);
      break;

    case kAgentEnd:
      onAgentEnd(theConnection);
      break;

    case kAgentCleanup:
      onAgentCleanup(theConnection);
      break;

    case kAgentFreeze:
      onAgentFreeze(theConnection);
      break;

    case kAgentMessage:
      onAgentMessage(theConnection);
	  break;
    
    default:
      fprintf(stderr, kUnknownMessage, agentState);
      exit(0);
      break;
    };

    rlSendBufferData(theConnection, &theBuffer, agentState);
  } while (agentState != kAgentCleanup);
}

int main(int argc, char** argv) {
  rlSocket theConnection = 0;

  struct hostent *host_ent;

  char* host = kLocalHost;
  short port = kDefaultPort;
  int autoReconnect = 0;

  char* envptr = 0;

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

  fprintf(stderr, "Connecting to host=%s on port=%d with autoreconnect=%d\n", host, port, autoReconnect);

  /* Allocate what should be plenty of space for the buffer - it will dynamically resize if it is too small */
  rlBufferCreate(&theBuffer, 4096);
  
  do {
    theConnection = rlWaitForConnection(host, port, kRetryTimeout);
    rlBufferClear(&theBuffer);
    rlSendBufferData(theConnection, &theBuffer, kAgentConnection);
    runAgentEventLoop(theConnection);
    rlClose(theConnection);
  } while(autoReconnect);

  rlBufferDestroy(&theBuffer);

  return 0;
}
