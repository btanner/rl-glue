#include <string.h> /* strlen */
#include <stdlib.h> /* calloc */
#include <stdio.h> /* for fprintf: debug only */

#include <RL_common.h>
#include <Network/RL_netlib.h>

extern int rlConnectSystems();
static rlSocket theAgentConnection = 0;
static Action theAction = {0};
static rlBuffer theBuffer = {0};

void rlSetAgentConnection(int theConnection) {
  if (theAgentConnection) {
    rlClose(theAgentConnection);
  }

  theAgentConnection = theConnection;
}

int rlIsAgentConnected() {
  return theAgentConnection != 0;
}

/* Send the task spec to the agent */
void agent_init(Task_specification theTaskSpec) {
  const int agentState = kAgentInit;
  const int theTaskSpecLength = strlen(theTaskSpec)+1;
  int offset = 0;

  rlConnectSystems();

  rlBufferCreate(&theBuffer, 4096);
  rlBufferClear(&theBuffer);
  rlBufferWrite(&theBuffer, 0, &agentState, 1, sizeof(int));
  rlSendBufferData(theAgentConnection, &theBuffer);

  rlBufferClear(&theBuffer);
  offset = rlBufferWrite(&theBuffer, offset, &theTaskSpecLength, 1, sizeof(int));
  if (theTaskSpecLength > 0) {
    offset = rlBufferWrite(&theBuffer, offset, theTaskSpec, theTaskSpecLength, sizeof(char));
  }
  rlSendBufferData(theAgentConnection, &theBuffer);
}

/* Send the observation to the agent, receive the action and return it */
Action agent_start(Observation theObservation) {
  const int agentState = kAgentStart;

  rlBufferClear(&theBuffer);
  rlBufferWrite(&theBuffer, 0, &agentState, 1, sizeof(int));
  rlSendBufferData(theAgentConnection, &theBuffer);

  rlBufferClear(&theBuffer);
  rlCopyADTToBuffer(&theObservation, &theBuffer);
  rlSendBufferData(theAgentConnection, &theBuffer);

  rlBufferClear(&theBuffer);
  rlRecvBufferData(theAgentConnection, &theBuffer);
  rlCopyBufferToADT(&theBuffer, &theAction);

  return theAction;
}

/* Send the reward and the observation to the agent, receive the action and return it */
Action agent_step(Reward theReward, Observation theObservation) {
  const int agentState = kAgentStep;

  rlBufferClear(&theBuffer);
  rlBufferWrite(&theBuffer, 0, &agentState, 1, sizeof(int));
  rlSendBufferData(theAgentConnection, &theBuffer);

  rlBufferClear(&theBuffer);
  rlBufferWrite(&theBuffer, 0, &theReward, 1, sizeof(Reward));
  rlSendBufferData(theAgentConnection, &theBuffer);

  rlBufferClear(&theBuffer);
  rlCopyADTToBuffer(&theObservation, &theBuffer);
  rlSendBufferData(theAgentConnection, &theBuffer);

  rlBufferClear(&theBuffer);
  rlRecvBufferData(theAgentConnection, &theBuffer);
  rlCopyBufferToADT(&theBuffer, &theAction);

  return theAction;
}

/* Send the final reward to the agent */
void agent_end(Reward theReward) { 
  const int agentState = kAgentEnd;

  rlBufferClear(&theBuffer);
  rlBufferWrite(&theBuffer, 0, &agentState, 1, sizeof(int));
  rlSendBufferData(theAgentConnection, &theBuffer);

  rlBufferClear(&theBuffer);
  rlBufferWrite(&theBuffer, 0, &theReward, 1, sizeof(Reward));
  rlSendBufferData(theAgentConnection, &theBuffer);
}

/* Tell the agent that we're cleaning up */
void agent_cleanup() {
  const int agentState = kAgentCleanup;

  rlBufferClear(&theBuffer);
  rlBufferWrite(&theBuffer, 0, &agentState, 1, sizeof(int));
  rlSendBufferData(theAgentConnection, &theBuffer);

  rlClose(theAgentConnection);
  theAgentConnection = 0;

  free(theAction.intArray);
  free(theAction.doubleArray);
  theAction.numInts     = 0;
  theAction.numDoubles  = 0;
  theAction.intArray    = 0;
  theAction.doubleArray = 0;

  rlBufferDestroy(&theBuffer);
}

void agent_freeze() {
  const int agentState = kAgentFreeze;

  rlBufferClear(&theBuffer);
  rlBufferWrite(&theBuffer, 0, &agentState, 1, sizeof(int));
  rlSendBufferData(theAgentConnection, &theBuffer);
}

char* agent_message(const char* inMessage) {
  int theInMessageLength = 0;
  int theOutMessageLength = 0;
  char * theOutMessage = NULL;

  const int agentState = kAgentMessage;
  int offset = 0;

  rlBufferClear(&theBuffer);
  rlBufferWrite(&theBuffer, 0, &agentState, 1, sizeof(int));
  rlSendBufferData(theAgentConnection, &theBuffer);

  if (inMessage != NULL) {
    theInMessageLength = strlen(inMessage) + 1;
  }

  rlBufferClear(&theBuffer);
  offset = rlBufferWrite(&theBuffer, offset, &theInMessageLength, 1, sizeof(int));
  if (theInMessageLength > 0) {
    offset = rlBufferWrite(&theBuffer, offset, inMessage, theInMessageLength, sizeof(char));
  }
  rlSendBufferData(theAgentConnection, &theBuffer);

  offset = 0;
  rlBufferClear(&theBuffer);
  rlRecvBufferData(theAgentConnection, &theBuffer);
  offset = rlBufferRead(&theBuffer, offset, &theOutMessageLength, 1, sizeof(int));
  if (theOutMessageLength > 0) {
    theOutMessage = (char*)calloc(theOutMessageLength, sizeof(char));
    offset = rlBufferRead(&theBuffer, offset, theOutMessage, theOutMessageLength, sizeof(char));
  }

  return theOutMessage;
}
