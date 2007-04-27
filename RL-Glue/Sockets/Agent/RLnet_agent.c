#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include <RLcommon.h>
#include <RLnet/RLnet.h>

#ifdef NETWORK_DEBUG
#define RLNET_DEBUG(x) x
#else
#define RLNET_DEBUG(x)
#endif

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

void print_reward(Reward theReward)
{
  fprintf(stderr, "AGENT RECV: theReward %f\n", theReward);
}

void print_action_header(Action theAction)
{
  fprintf(stderr, "AGENT SENT: theAction.numInts = %d\n", theAction.numInts);
  fprintf(stderr, "AGENT SENT: theAction.numDoubles = %d\n", theAction.numDoubles);
}

void print_observation_header(Observation theObservation)
{
  fprintf(stderr, "AGENT RECV: theObservation.numInts = %d\n", theObservation.numInts);
  fprintf(stderr, "AGENT RECV: theObservation.numDoubles = %d\n", theObservation.numDoubles);
}

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
  RLNET_DEBUG( print_observation_header(theObservation); )

  theAction = agent_start(theObservation);
  rlSendAction(theConnection, theAction);
  RLNET_DEBUG( print_action_header(theAction); )
}

void on_agent_step(rlSocket theConnection)
{
  rlRecvReward(theConnection, &theReward);
  RLNET_DEBUG( print_reward(theReward); )

  rlRecvObservation(theConnection, &theObservation);
  RLNET_DEBUG( print_observation_header(theObservation); )

  theAction = agent_step(theReward, theObservation);

  rlSendAction(theConnection, theAction);
  RLNET_DEBUG( print_action_header(theAction); )
}

void on_agent_end(rlSocket theConnection)
{
  rlRecvReward(theConnection, &theReward);
  RLNET_DEBUG( print_reward(theReward); )

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
    RLNET_DEBUG( fprintf(stderr, "AGENT RECV: %s\n", theMessage); )
	      
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
  int theResult = 0;
  short thePort = 0;

  if (argc != 3) 
  {
    fprintf(stderr, kUsage);
    return 1;
  }

  sscanf(argv[2], "%hd", &thePort);

  theConnection = rlOpen(thePort);
  theResult = rlIsValidSocket(theConnection);
  assert(theResult);
  
  theResult = rlConnect(theConnection, argv[1]);
  assert(theResult >= 0);
  
  run_agent(theConnection);
  
  theResult = rlClose(theConnection);
  assert(theResult == 0);
  
  return 0;
}
