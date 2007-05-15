#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include <RLcommon.h>
#include <RLnet/RLnet.h>

const char* kAgentInit = "init";
const char* kAgentStart= "start";
const char* kAgentStep = "step";
const char* kAgentEnd  = "end";
const char* kAgentCleanup = "cleanup";

/* Declare the task spec and length */
int theTaskSpecLength;
char* theTaskSpecBuffer;

/* Declare observation action and reward */
Action theAction;
Observation theObservation;
Reward theReward;

/* Provide forward declaration of agent interface */
extern void agent_init(Task_specification task_spec);
extern Action agent_start(Observation o);
extern Action agent_step(Reward r, Observation o);
extern void agent_end(Reward r);
extern void agent_cleanup();

const char* kUnknownMessage = "Unknown Message: %s\n";
const char* kUsage = "Usage: Agent <ip-address> <port>\n";

void on_agent_init(rlSocket theConnection)
{
  theTaskSpecLength = 0;
  rlRecvMessageHeader(theConnection, &theTaskSpecLength);

  theTaskSpecBuffer = (char*)calloc(theTaskSpecLength, 1);
  rlRecvMessageBody(theConnection, theTaskSpecBuffer, theTaskSpecLength);

  agent_init(theTaskSpecBuffer);
}

void on_agent_start(rlSocket theConnection)
{
  rlRecvObservationHeader(theConnection, &theObservation);
  theObservation.intArray = (int*)calloc(theObservation.numInts, sizeof(int));
  theObservation.doubleArray = (double*)calloc(theObservation.numDoubles, sizeof(double));
  rlRecvObservationBody(theConnection, &theObservation);

  theAction = agent_start(theObservation);
  rlSendAction(theConnection, theAction);
}

void on_agent_step(rlSocket theConnection)
{
  rlRecvReward(theConnection, &theReward);
  rlRecvObservation(theConnection, &theObservation);
  theAction = agent_step(theReward, theObservation);

  rlSendAction(theConnection, theAction);
}

void on_agent_end(rlSocket theConnection)
{
  rlRecvReward(theConnection, &theReward);
  agent_end(theReward);

  free(theObservation.intArray);
  free(theObservation.doubleArray);

  theObservation.intArray = 0;
  theObservation.doubleArray = 0;
}

void on_agent_cleanup(rlSocket theConnection)
{
  agent_cleanup();
  free(theTaskSpecBuffer);
}

void run_agent(rlSocket theConnection)
{
  char theMessage[8] = {0};

  do
  { 
    rlRecvData(theConnection, theMessage, 8);
	      
    if (strncmp(theMessage, kAgentInit, 8) == 0)
    {
      on_agent_init(theConnection);
    }
    else if (strncmp(theMessage, kAgentStart, 8) == 0)
    {
      on_agent_start(theConnection);
    }
    else if (strncmp(theMessage, kAgentStep, 8) == 0)
    {
      on_agent_step(theConnection);
    }
    else if ( strncmp(theMessage, kAgentEnd, 8) == 0)
    {
      on_agent_end(theConnection);
    }
    else if (strncmp(theMessage, kAgentCleanup, 8) == 0)
    {
      on_agent_cleanup(theConnection);
    }
    else
    {
      fprintf(stderr, kUnknownMessage, theMessage);
      break;
    }
  } while (strncmp(theMessage, kAgentCleanup, 8) != 0);
}

int main(int argc, char** argv)
{
  rlSocket theConnection;

  int isValidSocket = 0;
  int isConnected = -1;
  int isClosed = 0;

  short thePort = 0;

  if (argc != 3) 
  {
    fprintf(stderr, kUsage);
    return 1;
  }

  sscanf(argv[2], "%hd", &thePort);

  while(1)
  {
    while(isConnected < 0)
    {
      theConnection = rlOpen(thePort);
      isValidSocket = rlIsValidSocket(theConnection);
      assert(isValidSocket);
      
      isConnected = rlConnect(theConnection, argv[1]);
      if (isConnected < 0) rlClose(theConnection); /* We need to try again */
    }

    run_agent(theConnection);
    
    isClosed = rlClose(theConnection);
    isConnected = -1;
    /* assert(isClosed >= 0); */
  }

  return 0;
}
