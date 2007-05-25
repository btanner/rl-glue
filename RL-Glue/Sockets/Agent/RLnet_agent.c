#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>

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
extern void agent_init(int argc, char** argv, Task_specification task_spec);
extern Action agent_start(Observation o);
extern Action agent_step(Reward r, Observation o);
extern void agent_end(Reward r);
extern void agent_cleanup();

const char* kUnknownMessage = "Unknown Message: %s\n";
const char* kDefaultIp = "127.0.0.1";
const short kDefaultPort = 4697;

void on_agent_init(int argc, char** argv, rlSocket theConnection)
{
  theTaskSpecLength = 0;
  rlRecvMessageHeader(theConnection, &theTaskSpecLength);

  theTaskSpecBuffer = (char*)calloc(theTaskSpecLength, 1);
  rlRecvMessageBody(theConnection, theTaskSpecBuffer, theTaskSpecLength);

  agent_init(argc, argv, theTaskSpecBuffer);
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

void run_agent(int argc, char** argv, rlSocket theConnection)
{
  char theMessage[8] = {0};

  do
  { 
    rlRecvData(theConnection, theMessage, 8);
	      
    if (strncmp(theMessage, kAgentInit, 8) == 0)
    {
      on_agent_init(argc, argv, theConnection);
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

void parse_command_line(int argc, char** argv, char* const ip_buffer, int ip_buffer_size, short* port) {
  int c = 0;

  while((c = getopt(argc, argv, "h:p:")) != -1) {
    switch(c) {
    case 'h':
      sscanf(optarg, "%s", ip_buffer);
      break;

    case 'p':
      sscanf(optarg, "%hd", port);
      break;
    };
  }
}

int main(int argc, char** argv)
{
  rlSocket theConnection;

  int isValidSocket = 0;
  int isConnected = -1;
  int isClosed = 0;

  char ipbuffer[256] = {0};
  int ipbuffersize = 256;
  short port = kDefaultPort;

  strncpy(ipbuffer, kDefaultIp, ipbuffersize);
  parse_command_line(argc, argv, ipbuffer, ipbuffersize, &port);

  while(1)
  {
    while(isConnected < 0)
    {
      theConnection = rlOpen(port);
      isValidSocket = rlIsValidSocket(theConnection);
      assert(isValidSocket);
      
      isConnected = rlConnect(theConnection, ipbuffer);
      if (isConnected < 0) rlClose(theConnection); /* We need to try again */
    }

    run_agent(argc, argv, theConnection);
    
    isClosed = rlClose(theConnection);
    isConnected = -1;
    /* assert(isClosed >= 0); */
  }

  return 0;
}
