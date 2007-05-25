#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <RLcommon.h>
#include <RLnet/RLnet.h>
#include <unistd.h>

/* Could be culled, used only for debugging */
#include <stdio.h>

static rlSocket theAgentConnection;
static Action theAction;

const char* kAgentInit = "init";
const char* kAgentStart= "start";
const char* kAgentStep = "step";
const char* kAgentEnd  = "end";
const char* kAgentCleanup = "cleanup";

static const short kDefaultPort = 4697;

static void send_msg(rlSocket theSocket, const char* theMessage)
{
  char send_buffer[8] = {0};
  strncpy(send_buffer, theMessage, 8);
  rlSendData(theSocket, send_buffer, 8);
}

static void parse_command_line(int argc, char** argv, short *thePort) {
  int c = 0;

  while((c = getopt(argc, argv, "p:")) != -1) {
    switch(c) {
    case 'p':
      sscanf(optarg, "%hd", thePort);
      break;
    };
  }
}

void agent_init(int argc, char** argv, Task_specification theTaskSpecBuffer)
{
  int theTaskSpecLength = 0;

  rlSocket theServer;
  int isValidSocket = 0;
  int isListening = 0;

  short port = kDefaultPort;
  parse_command_line(argc, argv, &port);

  theServer = rlOpen(port);  
  isValidSocket = rlIsValidSocket(theServer);
  assert(isValidSocket);

  isListening = rlListen(theServer);
  assert(isListening >= 0);

  theAgentConnection = rlAcceptConnection(theServer);
  isValidSocket = rlIsValidSocket(theAgentConnection);
  assert(isValidSocket);

  rlClose(theServer);

  send_msg(theAgentConnection, kAgentInit);

  /* Warning, strlen, could be used as buffer-overrun exploit */
  theTaskSpecLength = strlen(theTaskSpecBuffer) + 1;

  rlSendMessageHeader(theAgentConnection, theTaskSpecLength);
  rlSendMessageBody(theAgentConnection, theTaskSpecBuffer, theTaskSpecLength);
}

Action agent_start(Observation theObservation)
{
  send_msg(theAgentConnection, kAgentStart);

  rlSendObservation(theAgentConnection, theObservation);
  rlRecvActionHeader(theAgentConnection, &theAction);

  if (theAction.numInts > 0)
    theAction.intArray = (int*)calloc(theAction.numInts, sizeof(int));

  if (theAction.numDoubles > 0)
    theAction.doubleArray = (double*)calloc(theAction.numDoubles, sizeof(double));

  rlRecvActionBody(theAgentConnection, &theAction);
  return theAction;
}

Action agent_step(Reward theReward, Observation theObservation)
{
  send_msg(theAgentConnection, kAgentStep);
  
  rlSendReward(theAgentConnection, theReward);
  rlSendObservation(theAgentConnection, theObservation);
  rlRecvAction(theAgentConnection, &theAction);

  return theAction;
}

void agent_end(Reward theReward)
{
  send_msg(theAgentConnection, kAgentEnd);

  rlSendReward(theAgentConnection, theReward);

  free(theAction.intArray);
  free(theAction.doubleArray);

  theAction.intArray = 0;
  theAction.doubleArray = 0;
}

void agent_cleanup()
{
  send_msg(theAgentConnection, kAgentCleanup);
  rlClose(theAgentConnection);
}
