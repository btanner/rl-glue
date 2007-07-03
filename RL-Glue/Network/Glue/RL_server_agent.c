#include <string.h> /* strlen */
#include <stdlib.h> /* calloc */
#include <assert.h> /* assert */

#include <RL_common.h>
#include <Network/RL_network.h>

extern int rlConnectSystems();
extern rlSocket theAgentConnection;
static Action theAction = {0};
static rlBuffer theBuffer = {0};

/* Send the task spec to the agent */
void agent_init(const Task_specification theTaskSpec) {
  int agentState = kAgentInit;
  const unsigned int theTaskSpecLength = strlen(theTaskSpec)+1;
  unsigned int offset = 0;
  
  /* Setup the connection */
  if (theAgentConnection == -1) {
    theAgentConnection = 0;
  }

  rlConnectSystems();
  rlBufferCreate(&theBuffer, 4096);

  /* send across agent_init specific data */
  rlBufferClear(&theBuffer);
  offset = 0;
  offset = rlBufferWrite(&theBuffer, offset, &theTaskSpecLength, 1, sizeof(int));
  if (theTaskSpecLength > 0) {
    offset = rlBufferWrite(&theBuffer, offset, theTaskSpec, theTaskSpecLength, sizeof(char));
  }
  rlSendBufferData(theAgentConnection, &theBuffer, agentState);

  rlBufferClear(&theBuffer);
  rlRecvBufferData(theAgentConnection, &theBuffer, &agentState);
  assert(agentState == kAgentInit);
}

/* Send the observation to the agent, receive the action and return it */
Action agent_start(Observation theObservation) {
  int agentState = kAgentStart;
  unsigned int offset = 0;

  rlBufferClear(&theBuffer);
  offset = 0;
  offset = rlCopyADTToBuffer(&theObservation, &theBuffer, offset);
  rlSendBufferData(theAgentConnection, &theBuffer, agentState);

  rlBufferClear(&theBuffer);
  rlRecvBufferData(theAgentConnection, &theBuffer, &agentState);
  assert(agentState == kAgentStart);
    
  offset = 0;
  offset = rlCopyBufferToADT(&theBuffer, offset, &theAction);

  return theAction;
}

/* Send the reward and the observation to the agent, receive the action and return it */
Action agent_step(Reward theReward, Observation theObservation) {
  int agentState = kAgentStep;
  unsigned int offset = 0;

  rlBufferClear(&theBuffer);
  offset = 0;
  offset = rlBufferWrite(&theBuffer, offset, &theReward, 1, sizeof(Reward));
  offset = rlCopyADTToBuffer(&theObservation, &theBuffer, offset);
  rlSendBufferData(theAgentConnection, &theBuffer, agentState);

  rlBufferClear(&theBuffer);
  rlRecvBufferData(theAgentConnection, &theBuffer, &agentState);
  assert(agentState == kAgentStep);

  offset = 0;
  offset = rlCopyBufferToADT(&theBuffer, offset, &theAction);

  return theAction;
}

/* Send the final reward to the agent */
void agent_end(Reward theReward) { 
  int agentState = kAgentEnd;
  unsigned int offset = 0;

  rlBufferClear(&theBuffer);
  offset = rlBufferWrite(&theBuffer, offset, &agentState, 1, sizeof(int));
  offset = rlBufferWrite(&theBuffer, offset, &theReward, 1, sizeof(Reward));
  rlSendBufferData(theAgentConnection, &theBuffer, agentState);

  rlBufferClear(&theBuffer);
  rlRecvBufferData(theAgentConnection, &theBuffer, &agentState);
  assert(agentState == kAgentEnd);
}

/* Tell the agent that we're cleaning up */
void agent_cleanup() {
  int agentState = kAgentCleanup;

  rlBufferClear(&theBuffer);
  rlSendBufferData(theAgentConnection, &theBuffer, agentState);

  rlBufferClear(&theBuffer);
  rlRecvBufferData(theAgentConnection, &theBuffer, &agentState);
  assert(agentState == kAgentCleanup);
 
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
  int agentState = kAgentFreeze;

  rlBufferClear(&theBuffer);
  rlSendBufferData(theAgentConnection, &theBuffer, agentState);

  rlBufferClear(&theBuffer);
  rlRecvBufferData(theAgentConnection, &theBuffer, &agentState);
  assert(agentState == kAgentFreeze);
}

Message agent_message(const Message inMessage) {
  int agentState = kAgentMessage;

  char * theOutMessage = NULL;
  unsigned int theInMessageLength = 0;
  unsigned int theOutMessageLength = 0;
  unsigned int offset = 0;

  if (inMessage != 0) {
    theInMessageLength = strlen(inMessage) + 1;
  }

  rlBufferClear(&theBuffer);
  offset = 0;
  offset = rlBufferWrite(&theBuffer, offset, &theInMessageLength, 1, sizeof(int));
  if (theInMessageLength > 0) {
    offset = rlBufferWrite(&theBuffer, offset, inMessage, theInMessageLength, sizeof(char));
  }
  rlSendBufferData(theAgentConnection, &theBuffer, agentState);

  rlBufferClear(&theBuffer);
  rlRecvBufferData(theAgentConnection, &theBuffer, &agentState);
  assert(agentState == kAgentMessage);

  offset = 0;
  offset = rlBufferRead(&theBuffer, offset, &theOutMessageLength, 1, sizeof(int));
  if (theOutMessageLength > 0) {
    theOutMessage = (char*)calloc(theOutMessageLength, sizeof(char));
    offset = rlBufferRead(&theBuffer, offset, theOutMessage, theOutMessageLength, sizeof(char));
  }

  return theOutMessage;
}
