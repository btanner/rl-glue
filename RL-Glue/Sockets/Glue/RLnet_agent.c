#ifdef NETWORK_DEBUG
#define RLNET_DEBUG(x) x
#else
#define RLNET_DEBUG(x)
#endif

#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <RLcommon.h>
#include <RLnet/RLnet.h>

/* Could be culled, used only for debugging */
#include <stdio.h>

static rlSocket theAgentConnection;
static Action theAction;

const char* kAgentInit = "init";
const char* kAgentStart= "start";
const char* kAgentStep = "step";
const char* kAgentEnd  = "end";
const char* kAgentCleanup = "cleanup";

static void print_reward(Reward theReward)
{
  fprintf(stderr, "GLUE SENT: theReward %f\n", theReward);
}

static void print_action_header(Action theAction)
{
  fprintf(stderr, "GLUE RECV: theAction.numInts = %d\n", theAction.numInts);
  fprintf(stderr, "GLUE RECV: theAction.numDoubles = %d\n", theAction.numDoubles);
}

static void print_observation_header(Observation theObservation)
{
  fprintf(stderr, "GLUE SENT: theObservation.numInts = %d\n", theObservation.numInts);
  fprintf(stderr, "GLUE SENT: theObservation.numDoubles = %d\n", theObservation.numDoubles);
}

static void send_msg(rlSocket theSocket, const char* theMessage)
{
  char send_buffer[8] = {0};
  strncpy(send_buffer, theMessage, 8);
  rlSendData(theSocket, send_buffer, 8);

  RLNET_DEBUG( fprintf(stderr, "GLUE SENT: %s\n", send_buffer); )
}

void agent_init(Task_specification theTaskSpecBuffer)
{
  int theResult = 0;
  int theTaskSpecLength = 0;

  rlSocket theServer = rlOpen(4096);
  theResult = rlIsValidSocket(theServer);
  assert(theResult != 0);

  theResult = rlListen(theServer);
  assert(theResult >= 0);

  theAgentConnection = rlAcceptConnection(theServer);
  theResult = rlIsValidSocket(theAgentConnection);
  assert(theResult != 0);

  theResult = rlClose(theServer);
  assert(theResult == 0);

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
  RLNET_DEBUG( print_observation_header(theObservation); )

  rlRecvActionHeader(theAgentConnection, &theAction);
  RLNET_DEBUG( print_action_header(theAction); )

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
  RLNET_DEBUG( print_reward(theReward); )

  rlSendObservation(theAgentConnection, theObservation);
  RLNET_DEBUG( print_observation_header(theObservation); )

  rlRecvAction(theAgentConnection, &theAction);
  RLNET_DEBUG( print_action_header(theAction); )

  return theAction;
}

void agent_end(Reward theReward)
{
  send_msg(theAgentConnection, kAgentEnd);

  rlSendReward(theAgentConnection, theReward);
  RLNET_DEBUG( print_reward(theReward); )

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
