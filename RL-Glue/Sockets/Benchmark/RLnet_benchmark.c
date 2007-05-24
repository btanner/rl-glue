#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include <RLcommon.h>
#include <RLnet/RLnet.h>

static rlSocket theGlueConnection;
static Observation theObservation;
static Action theAction;
static int isAllocated;

const char* kRLInit = "init";
const char* kRLStart = "start";
const char* kRLStep = "step";
const char* kRLReturn = "return";
const char* kRLAverageReward = "aver";
const char* kRLAverageNumSteps = "avens";
const char* kRLNumSteps = "numstep";
const char* kRLNumEpisodes = "numeps";
const char* kRLEpisode = "episode";
const char* kRLSetState = "sets";
const char* kRLSetRandomSeed = "setrs";
const char* kRLGetState = "gets";
const char* kRLGetRandomSeed = "getrs";
const char* kRLCleanup = "cleanup";

static void send_msg(rlSocket theSocket, const char* theMessage)
{
  char send_buffer[8] = {0};
  strncpy(send_buffer, theMessage, 8);
  rlSendData(theSocket, send_buffer, 8);
}

static void recv_receipt(rlSocket theSocket, const char* theExpectedMessage)
{
  char recv_buffer[8] = {0};
  rlRecvData(theSocket, recv_buffer, 8);


  if(strncmp(recv_buffer, theExpectedMessage, 8) != 0)
    fprintf(stderr, "Expected: %s\nReceived: %s\n", theExpectedMessage, recv_buffer);

}

void RL_init(int argc, char** argv)
{
  int isValidSocket = 0;
  int isConnected = -1;

  /* Fixme: We need to handle default & user specified ports & ips */

  while(isConnected < 0) 
  {
    theGlueConnection = rlOpen(4095);
    isValidSocket = rlIsValidSocket(theGlueConnection);
    assert(isValidSocket);
   
    isConnected = rlConnect(theGlueConnection, "127.0.0.1");
    if (isConnected < 0) rlClose(theGlueConnection); /* We need to try again */
  }

  send_msg(theGlueConnection, kRLInit);

  isAllocated = 0;

  recv_receipt(theGlueConnection, kRLInit);
}

Observation_action RL_start()
{
  Observation_action theObservationAction;
  send_msg(theGlueConnection, kRLStart);

  rlRecvObservationHeader(theGlueConnection, &theObservation);
  rlRecvActionHeader(theGlueConnection, &theObservation);

  if (!isAllocated)
  {
    theObservation.intArray = calloc(theObservation.numInts, sizeof(int));
    theObservation.doubleArray = calloc(theObservation.numDoubles, sizeof(double));
    isAllocated = 1;
  }

  rlRecvObservationBody(theGlueConnection, &theObservation);
  rlRecvActionBody(theGlueConnection, &theAction);

  theObservationAction.o = theObservation;
  theObservationAction.a = theAction;

  recv_receipt(theGlueConnection, kRLStart);

  return theObservationAction;
}

Reward_observation_action_terminal RL_step()
{
  Reward_observation_action_terminal theRewardObservationActionTerminal;
  send_msg(theGlueConnection, kRLStep);

  rlRecvObservationHeader(theGlueConnection, &theObservation);
  rlRecvActionHeader(theGlueConnection, &theAction);

  rlRecvReward(theGlueConnection, &theRewardObservationActionTerminal.r);
  rlRecvObservationBody(theGlueConnection, &theObservation);
  rlRecvActionBody(theGlueConnection, &theAction);
  rlRecvTerminal(theGlueConnection, &theRewardObservationActionTerminal.terminal);

  theRewardObservationActionTerminal.o = theObservation;
  theRewardObservationActionTerminal.a = theAction;

  recv_receipt(theGlueConnection, kRLStep);

  return theRewardObservationActionTerminal;
}

Reward RL_return()
{
  Reward theReturn = 0;
  send_msg(theGlueConnection, kRLReturn);
  rlRecvReward(theGlueConnection, &theReturn);
  recv_receipt(theGlueConnection, kRLReturn);
  return theReturn;
}

Reward RL_average_reward()
{
  Reward theAverageReward = 0;
  send_msg(theGlueConnection, kRLAverageReward);
  rlRecvReward(theGlueConnection, &theAverageReward);
  recv_receipt(theGlueConnection, kRLAverageReward);
  return theAverageReward;
}

double RL_average_num_steps()
{
  double theAverageNumberOfSteps = 0;
  send_msg(theGlueConnection, kRLAverageNumSteps);
  rlRecvData(theGlueConnection, &theAverageNumberOfSteps, sizeof(double));
  recv_receipt(theGlueConnection, kRLAverageNumSteps);
  return theAverageNumberOfSteps;
}

int RL_num_steps()
{
  int theNumberOfSteps = 0;
  send_msg(theGlueConnection, kRLNumSteps);
  rlRecvData(theGlueConnection, &theNumberOfSteps, sizeof(int));
  recv_receipt(theGlueConnection, kRLNumSteps);
  return theNumberOfSteps;
}

int RL_num_episodes()
{
  int theNumberOfEpisodes = 0;
  send_msg(theGlueConnection, kRLNumEpisodes);
  rlRecvData(theGlueConnection, &theNumberOfEpisodes, sizeof(int));
  recv_receipt(theGlueConnection, kRLNumEpisodes);
  return theNumberOfEpisodes;
}

/* void RL_episode(); */
void RL_episode(int numSteps)
{
  send_msg(theGlueConnection, kRLEpisode);
  rlSendData(theGlueConnection, &numSteps, sizeof(int));
  recv_receipt(theGlueConnection, kRLEpisode);
}

void RL_set_state(State_key sk)
{
  send_msg(theGlueConnection, kRLSetState);
  rlSendData(theGlueConnection, &sk, sizeof(State_key));
  recv_receipt(theGlueConnection, kRLSetState);
}

void RL_set_random_seed(Random_seed_key rsk)
{
  send_msg(theGlueConnection, kRLSetRandomSeed);
  rlSendData(theGlueConnection, &rsk, sizeof(Random_seed_key));
  recv_receipt(theGlueConnection, kRLSetRandomSeed);
}

State_key RL_get_state()
{
  State_key theStateKey = 0;
  send_msg(theGlueConnection, kRLGetState);
  rlRecvData(theGlueConnection, &theStateKey, sizeof(State_key));
  recv_receipt(theGlueConnection, kRLGetState);
  return theStateKey;
}

Random_seed_key RL_get_random_seed()
{
  Random_seed_key theRandomSeedKey = 0;
  send_msg(theGlueConnection, kRLGetRandomSeed);
  rlRecvData(theGlueConnection, &theRandomSeedKey, sizeof(Random_seed_key));
  recv_receipt(theGlueConnection, kRLGetRandomSeed);
  return theRandomSeedKey;
}

void RL_cleanup()
{
  send_msg(theGlueConnection, kRLCleanup);
  recv_receipt(theGlueConnection, kRLCleanup);
  
  /* Cleanup our memory */
  free(theObservation.intArray);
  free(theObservation.doubleArray);

  free(theAction.intArray);
  free(theAction.doubleArray);

  theObservation.numInts = 0;
  theObservation.numDoubles = 0;

  theAction.numInts = 0;
  theAction.numDoubles = 0;

  isAllocated = 0;
}
