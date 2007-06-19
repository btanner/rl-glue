#include <stdlib.h>
#include <stdio.h>
#include <Network/RL_netlib.h> 

extern void rlSetAgentConnection(int);
extern void rlSetEnvironmentConnection(int);
extern void rlSetExperimentConnection(int);
extern int rlIsAgentConnected();
extern int rlIsEnvironmentConnected();
extern int rlIsExperimentConnected();

/* 
   rlConnectSystems is responsible for ensuring that the Glue has an agent, environment and experiment
   program before the Glue attempts to run an experiment.

   If the experiment and the Glue are compiled together RL_init calls rlConnectSystems.
   If the Glue is separate, it has its own main() that calls rlConnectSystems.   
*/

int rlConnectSystems() {
  int isAgentConnected       = rlIsAgentConnected();
  int isEnvironmentConnected = rlIsEnvironmentConnected(); 
  int isExperimentConnected  = rlIsExperimentConnected();
  int theClientType = 0;
  int theClient = 0;
  int theServer = 0;
  rlSocket theExperimentConnection = 0;
  rlBuffer theBuffer = {0};
  short port = kDefaultPort;
  char* envptr = 0;

  rlBufferCreate(&theBuffer, sizeof(int) * 2);

  if (!isAgentConnected || !isEnvironmentConnected || !isExperimentConnected) {
    envptr = getenv("RLGLUE_PORT");  
    if (envptr != 0) {
      port = strtol(envptr, 0, 10);
      if (port == 0) {
	port = kDefaultPort;
      }
      fprintf(stderr, "RL-Glue is listening for connections on port=%d\n", port);
    }

    theServer = rlOpen(port);
    rlListen(theServer, port);
  }

  while(!isAgentConnected || !isEnvironmentConnected || !isExperimentConnected) {
    theClient = rlAcceptConnection(theServer);
    rlRecvBufferData(theClient, &theBuffer);
    rlBufferRead(&theBuffer, 0, &theClientType, 1, sizeof(int));

    switch(theClientType) {
    case kAgentConnection:
      rlSetAgentConnection(theClient);
      isAgentConnected = 1;
      break;

    case kEnvironmentConnection:
      rlSetEnvironmentConnection(theClient);
      isEnvironmentConnected = 1;
      break;

    case kExperimentConnection:
      rlSetExperimentConnection(theClient);
      isExperimentConnected = 1;
      theExperimentConnection = theClient;
      break;

    default:
      fprintf(stderr, "RL_network.c: Unknown Connection Type: %d\n", theClientType);
      break;
    };
  }

  rlClose(theServer);
  rlBufferDestroy(&theBuffer);

  return theExperimentConnection;
}
