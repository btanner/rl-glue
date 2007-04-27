/* Standard Headers */
#include <string.h>

/* Network Headers */
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>

/* RLnet Library Header */
#include <RLnet/RLnet.h>

rlSocket rlOpen(short thePort)
{
  int flag = 1;
  rlSocket theSocket = {0};

  theSocket.os_socket = socket(PF_INET, SOCK_STREAM, 0);
  theSocket.port = thePort;

  setsockopt(theSocket.os_socket, IPPROTO_TCP, TCP_NODELAY, 
	     (char*)&flag, sizeof(int));

  return theSocket;
}

rlSocket rlAcceptConnection(rlSocket theSocket)
{
  rlSocket theClient = {0};
  struct sockaddr_in theClientAddress = {0};
  unsigned int theSocketSize = sizeof(struct sockaddr_in);

  theClient.os_socket = accept(theSocket.os_socket,
			       (struct sockaddr*)&theClientAddress,
			       &theSocketSize);

  return theClient;
}

int rlIsValidSocket(rlSocket theSocket)
{
  return theSocket.os_socket != -1;
}

int rlConnect(rlSocket theSocket, const char* theAddress)
{
  int theStatus = 0;
  struct sockaddr_in theDestination;
  theDestination.sin_family = AF_INET;
  theDestination.sin_port = htons(theSocket.port);
  theDestination.sin_addr.s_addr = inet_addr(theAddress);
  memset(&theDestination.sin_zero, '\0', 8);

  theStatus = connect(theSocket.os_socket, 
		      (struct sockaddr*)&theDestination, 
		      sizeof(struct sockaddr));

  return theStatus;
}

int rlListen(rlSocket theSocket)
{
  struct sockaddr_in theServer;
  int theStatus = 0;
  int yes = 1;
  
  theServer.sin_family = AF_INET;
  theServer.sin_port = htons(theSocket.port);
  theServer.sin_addr.s_addr = INADDR_ANY;
  memset(&theServer.sin_zero, '\0', 8);
  
  /* We don't really care if this fails... */
  setsockopt(theSocket.os_socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

  theStatus = bind(theSocket.os_socket, 
		   (struct sockaddr*)&theServer, 
		   sizeof(struct sockaddr));

  if (theStatus == -1) return -1;
  
  theStatus = listen(theSocket.os_socket, 10);
  if (theStatus == -1) return -1;

  return theStatus;
}

int rlClose(rlSocket theSocket)
{
  return close(theSocket.os_socket);
}

int rlSendData(rlSocket theSocket, const void* theData, int theLength)
{
  int theBytesSent = 0;
  int theMsgError = 0;

  const char* theDataBuffer = (const char*)theData;
  
  while (theBytesSent < theLength)
  {
    theMsgError = send(theSocket.os_socket, theDataBuffer + theBytesSent, theLength - theBytesSent, 0);
    if (theMsgError == -1) break;
    else theBytesSent += theMsgError;
  }

  return theBytesSent;
}

int rlRecvData(rlSocket theSocket, void* theData, int theLength)
{
  int theBytesRecv = 0;
  int theMsgError = 0;

  char* theDataBuffer = (char*)theData;

  while (theBytesRecv < theLength)
  {
    theMsgError = recv(theSocket.os_socket, theDataBuffer + theBytesRecv, theLength - theBytesRecv, 0);
    if (theMsgError <= 0) break;
    else theBytesRecv += theMsgError;
  }

  return theBytesRecv;
}

int rlGetSystemByteOrder()
{
  /*
    Endian will be 1 when we are on a little endian machine,
    and not 1 on a big endian machine.
  */

  const int one = 1;
  const char endian = *(char*)&one;

  return endian;
}

/* From here on functions rely on RLcommon types. Should this be split into a separate header & c file */
void rlSendMessageHeader(rlSocket theSocket, int theMessageLength)
{
  rlSendData(theSocket, &theMessageLength, sizeof(int));
}

void rlSendMessageBody(rlSocket theSocket, const char* theMessage, int theMessageLength)
{
  rlSendData(theSocket, theMessage, theMessageLength);
}

void rlSendMessage(rlSocket theSocket, const char* theMessage, int theMessageLength)
{
  rlSendMessageHeader(theSocket, theMessageLength);
  rlSendMessageBody(theSocket, theMessage, theMessageLength);
}

void rlSendActionHeader(rlSocket theSocket, Action theAction)
{
  rlSendData(theSocket, &theAction.numInts, sizeof(int));
  rlSendData(theSocket, &theAction.numDoubles, sizeof(int));
}

void rlSendActionBody(rlSocket theSocket, Action theAction)
{
  if (theAction.numInts > 0)
    rlSendData(theSocket, theAction.intArray, theAction.numInts * sizeof(int));

  if (theAction.numDoubles > 0)
    rlSendData(theSocket, theAction.doubleArray, theAction.numDoubles * sizeof(double));
}

void rlSendAction(rlSocket theSocket, Action theAction)
{
  rlSendActionHeader(theSocket, theAction);
  rlSendActionBody(theSocket, theAction);
}

void rlSendObservationHeader(rlSocket theSocket, Observation theObservation)
{
  rlSendData(theSocket, &theObservation.numInts, sizeof(int));
  rlSendData(theSocket, &theObservation.numDoubles, sizeof(int));
}

void rlSendObservationBody(rlSocket theSocket, Observation theObservation)
{
  if (theObservation.numInts > 0)
    rlSendData(theSocket, theObservation.intArray, theObservation.numInts*sizeof(int));

  if (theObservation.numDoubles > 0)
    rlSendData(theSocket, theObservation.doubleArray, theObservation.numDoubles*sizeof(double));
}

void rlSendObservation(rlSocket theSocket, Observation theObservation)
{
  rlSendObservationHeader(theSocket, theObservation);
  rlSendObservationBody(theSocket, theObservation);
}

void rlSendReward(rlSocket theSocket, Reward theReward)
{
  rlSendData(theSocket, &theReward, sizeof(Reward));
}

void rlSendTerminal(rlSocket theSocket, int isTerminal)
{
  rlSendData(theSocket, &isTerminal, sizeof(int));
}

void rlRecvMessageHeader(rlSocket theSocket, int* theMessageLength)
{
  rlRecvData(theSocket, theMessageLength, sizeof(int));
}

void rlRecvMessageBody(rlSocket theSocket, char* const theMessage, int theMessageLength)
{
  rlRecvData(theSocket, theMessage, theMessageLength);
}

void rlRecvMessage(rlSocket theSocket, char* const theMessage, int *theMessageLength)
{
  int theRecvLength = 0;
  rlRecvMessageHeader(theSocket, &theRecvLength);
  rlRecvMessageBody(theSocket, theMessage, theRecvLength);
  *theMessageLength = theRecvLength;
}

void rlRecvActionHeader(rlSocket theSocket, Action* const theAction)
{
  rlRecvData(theSocket, &theAction->numInts, sizeof(int));
  rlRecvData(theSocket, &theAction->numDoubles, sizeof(int));   
}

void rlRecvActionBody(rlSocket theSocket, Action* const theAction)
{
  if (theAction->numInts > 0)
    rlRecvData(theSocket, theAction->intArray, theAction->numInts * sizeof(int));

  if (theAction->numDoubles > 0)
    rlRecvData(theSocket, theAction->doubleArray, theAction->numDoubles * sizeof(double));
}

void rlRecvAction(rlSocket theSocket, Action* const theAction)
{
  rlRecvActionHeader(theSocket, theAction);
  rlRecvActionBody(theSocket, theAction);
}

void rlRecvObservationHeader(rlSocket theSocket, Observation* const theObservation)
{
  rlRecvData(theSocket, &theObservation->numInts, sizeof(int));
  rlRecvData(theSocket, &theObservation->numDoubles, sizeof(int));
}

void rlRecvObservationBody(rlSocket theSocket, Observation* const theObservation)
{
  if (theObservation->numInts > 0)
    rlRecvData(theSocket, theObservation->intArray, theObservation->numInts * sizeof(int));
  
  if (theObservation->numDoubles > 0)
    rlRecvData(theSocket, theObservation->doubleArray, theObservation->numDoubles * sizeof(double));
}

void rlRecvObservation(rlSocket theSocket, Observation* const theObservation)
{
  rlRecvObservationHeader(theSocket, theObservation);
  rlRecvObservationBody(theSocket, theObservation);
}

void rlRecvReward(rlSocket theSocket, Reward* const theReward)
{
  rlRecvData(theSocket, theReward, sizeof(Reward));
}

void rlRecvTerminal(rlSocket theSocket, int* isTerminal)
{
  rlRecvData(theSocket, isTerminal, sizeof(int));
}

/* Send Observation_action, Reward_observation, Reward_observation_action_terminal */
void rlSendObservationActionHeader(rlSocket theSocket, Observation_action theObservationAction)
{
  rlSendObservationHeader(theSocket, theObservationAction.o);
  rlSendActionHeader(theSocket, theObservationAction.a);
}

void rlSendObservationActionBody(rlSocket theSocket, Observation_action theObservationAction)
{
  rlSendObservationBody(theSocket, theObservationAction.o);
  rlSendActionBody(theSocket, theObservationAction.a);
}

void rlSendObservationAction(rlSocket theSocket, Observation_action theObservationAction)
{
  rlSendObservationActionHeader(theSocket, theObservationAction);
  rlSendObservationActionBody(theSocket, theObservationAction);
}

void rlSendRewardObservationHeader(rlSocket theSocket, Reward_observation theRewardObservation)
{
  rlSendObservationHeader(theSocket, theRewardObservation.o);
}

void rlSendRewardObservationBody(rlSocket theSocket, Reward_observation theRewardObservation)
{
  rlSendReward(theSocket, theRewardObservation.r);
  rlSendObservationBody(theSocket, theRewardObservation.o);
  rlSendTerminal(theSocket, theRewardObservation.terminal);
}

void rlSendRewardObservation(rlSocket theSocket, Reward_observation theRewardObservation)
{
  rlSendRewardObservationHeader(theSocket, theRewardObservation);
  rlSendRewardObservationBody(theSocket, theRewardObservation);
}

void rlSendRewardObservationActionTerminalHeader(rlSocket theSocket, Reward_observation_action_terminal theRewardObservationActionTerminal)
{
  rlSendObservationHeader(theSocket, theRewardObservationActionTerminal.o);
  rlSendActionHeader(theSocket, theRewardObservationActionTerminal.a);
}

void rlSendRewardObservationActionTerminalBody(rlSocket theSocket, Reward_observation_action_terminal theRewardObservationActionTerminal)
{
  rlSendReward(theSocket, theRewardObservationActionTerminal.r);
  rlSendObservationBody(theSocket, theRewardObservationActionTerminal.o);
  rlSendActionBody(theSocket, theRewardObservationActionTerminal.a);
  rlSendTerminal(theSocket, theRewardObservationActionTerminal.terminal);
}

void rlSendRewardObservationActionTerminal(rlSocket theSocket, Reward_observation_action_terminal theRewardObservationActionTerminal)
{
  rlSendRewardObservationActionTerminalHeader(theSocket, theRewardObservationActionTerminal);
  rlSendRewardObservationActionTerminalBody(theSocket, theRewardObservationActionTerminal);
}

void rlRecvObservationActionHeader(rlSocket theSocket, Observation_action* const theObservationAction)
{
  rlRecvObservationHeader(theSocket, &theObservationAction->o);
  rlRecvActionHeader(theSocket, &theObservationAction->a);
}

void rlRecvObservationActionBody(rlSocket theSocket, Observation_action* const theObservationAction)
{
  rlRecvObservationBody(theSocket, &theObservationAction->o);
  rlRecvActionBody(theSocket, &theObservationAction->a);
}

void rlRecvObservationAction(rlSocket theSocket, Observation_action* const theObservationAction)
{
  rlRecvObservationActionHeader(theSocket, theObservationAction);
  rlRecvObservationActionBody(theSocket, theObservationAction);  
}

void rlRecvRewardObservationHeader(rlSocket theSocket, Reward_observation* const theRewardObservation)
{
  rlRecvObservationHeader(theSocket, &theRewardObservation->o);
}

void rlRecvRewardObservationBody(rlSocket theSocket, Reward_observation* const theRewardObservation)
{
  rlRecvReward(theSocket, &theRewardObservation->r);
  rlRecvObservationBody(theSocket, &theRewardObservation->o);
  rlRecvTerminal(theSocket, &theRewardObservation->terminal);
}

void rlRecvRewardObservation(rlSocket theSocket, Reward_observation* const theRewardObservation)
{
  rlRecvRewardObservationHeader(theSocket, theRewardObservation);
  rlRecvRewardObservationBody(theSocket, theRewardObservation);
}

void rlRecvRewardObservationActionTerminalHeader(rlSocket theSocket, Reward_observation_action_terminal* const theRewardObservationActionTerminal)
{
  rlRecvObservationHeader(theSocket, &theRewardObservationActionTerminal->o);
  rlRecvActionHeader(theSocket, &theRewardObservationActionTerminal->a);
}

void rlRecvRewardObservationActionTerminalBody(rlSocket theSocket, Reward_observation_action_terminal* const theRewardObservationActionTerminal)
{
  rlRecvReward(theSocket, &theRewardObservationActionTerminal->r);
  rlRecvObservationBody(theSocket, &theRewardObservationActionTerminal->o);
  rlRecvActionBody(theSocket, &theRewardObservationActionTerminal->a);
  rlRecvTerminal(theSocket, &theRewardObservationActionTerminal->terminal);
}

void rlRecvRewardObservationActionTerminal(rlSocket theSocket, Reward_observation_action_terminal* const theRewardObservationActionTerminal)
{
  rlRecvRewardObservationActionTerminalHeader(theSocket, theRewardObservationActionTerminal);
  rlRecvRewardObservationActionTerminalBody(theSocket, theRewardObservationActionTerminal);
}

