#ifndef libSocketRL_h
#define libSocketRL_h

/* Defines types for RL-Glue */
#include <RLcommon.h>

/* Data types */
typedef struct rlSocket_t
{
  short port;
  int os_socket;
} rlSocket;

/* Basic network functionality */
rlSocket rlOpen(short port);
rlSocket rlAcceptConnection(rlSocket socket);

int rlIsValidSocket(rlSocket socket);
int rlConnect(rlSocket socket, const char* address);
int rlListen(rlSocket socket);
int rlClose(rlSocket socket);

int rlSendData(rlSocket socket, const void* data, int length);
int rlRecvData(rlSocket socket, void* data, int length);

int rlGetSystemByteOrder();

/* Send/Recv Action/Observation/Reward */
void rlSendMessageHeader(rlSocket theSocket, int theMessageLength);
void rlSendMessageBody(rlSocket theSocket, const char* theMessage, int theMessageLength);
void rlSendMessage(rlSocket theSocket, const char* theMessage, int theMessageLength);

void rlSendActionHeader(rlSocket theSocket, Action theAction);
void rlSendActionBody(rlSocket theSocket, Action theAction);
void rlSendAction(rlSocket theSocket, Action theAction);

void rlSendObservationHeader(rlSocket theSocket, Observation theObservation);
void rlSendObservationBody(rlSocket theSocket, Observation theObservation);
void rlSendObservation(rlSocket theSocket, Observation theObservation);

void rlSendReward(rlSocket theSocket, Reward theReward);

void rlSendTerminal(rlSocket theSocket, int isTerminal);

void rlRecvMessageHeader(rlSocket theSocket, int* theMessageLength);
void rlRecvMessageBody(rlSocket theSocket, char* const theMessage, int theMessageLength);
void rlRecvMessage(rlSocket theSocket, char* const theMessage, int* theMessageLength);

void rlRecvActionHeader(rlSocket theSocket, Action* const theAction);
void rlRecvActionBody(rlSocket theSocket, Action* const theAction);
void rlRecvAction(rlSocket theSocket, Action* const theAction);

void rlRecvObservationHeader(rlSocket theSocket, Observation* const theObservation);
void rlRecvObservationBody(rlSocket theSocket, Observation* const theObservation);
void rlRecvObservation(rlSocket theSocket, Observation* const theObservation);

void rlRecvReward(rlSocket theSocket, Reward* const theReward);

void rlRecvTerminal(rlSocket theSocket, int* const isTerminal);

/* Send Observation_action, Reward_observation, Reward_observation_action_terminal */
void rlSendObservationActionHeader(rlSocket theSocket, Observation_action theObservationAction);
void rlSendObservationactionBody(rlSocket theSocket, Observation_action theObservationAction);
void rlSendObservationAction(rlSocket theSocket, Observation_action theObservationAction);

void rlSendRewardObservationHeader(rlSocket theSocket, Reward_observation theRewardObservation);
void rlSendRewardObservationBody(rlSocket theSocket, Reward_observation theRewardObservation);
void rlSendRewardObservation(rlSocket theSocket, Reward_observation theRewardObservation);

void rlSendRewardObservationActionTerminalHeader(rlSocket theSocket, Reward_observation_action_terminal theRewardObservationActionTerminal);
void rlSendRewardObservationActionTerminalBody(rlSocket theSocket, Reward_observation_action_terminal theRewardObservationActionTerminal);
void rlSendRewardObservationActionTerminal(rlSocket theSocket, Reward_observation_action_terminal theRewardObservationActionTerminal);

void rlRecvObservationActionHeader(rlSocket theSocket, Observation_action* const theObservationAction);
void rlRecvObservationActionBody(rlSocket theSocket, Observation_action* const theObservationAction);
void rlRecvObservationAction(rlSocket theSocket, Observation_action* const theObservationAction);

void rlRecvRewardObservationHeader(rlSocket theSocket, Reward_observation* const theRewardObservation);
void rlRecvRewardObservationBody(rlSocket theSocket, Reward_observation* const theRewardObservation);
void rlRecvRewardObservation(rlSocket theSocket, Reward_observation* const theRewardObservation);

void rlRecvRewardObservationActionTerminalHeader(rlSocket theSocket, Reward_observation_action_terminal* const theRewardObservationActionTerminal);
void rlRecvRewardObservationActionTerminalBody(rlSocket theSocket, Reward_observation_action_terminal* const theRewardObservationActionTerminal);
void rlRecvRewardObservationActionTerminal(rlSocket theSocket, Reward_observation_action_terminal* const theRewardObservationActionTerminal);

#endif
