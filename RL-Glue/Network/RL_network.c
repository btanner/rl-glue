#include <stdlib.h>
#include <stdio.h>
#include <Network/RL_netlib.h> 


rlSocket theAgentConnection = -1;
rlSocket theEnvironmentConnection = -1;
rlSocket theExperimentConnection = -1;

/* 
   rlConnectSystems is responsible for ensuring that the Glue has an agent, environment and experiment
   program before the Glue attempts to run an experiment.

   If the experiment and the Glue are compiled together RL_init calls rlConnectSystems.
   If the Glue is separate, it has its own main() that calls rlConnectSystems.   
*/

int rlConnectSystems() {
  int isAgentConnected       = (theAgentConnection != 0);
  int isEnvironmentConnected = (theEnvironmentConnection != 0); 
  int isExperimentConnected  = (theExperimentConnection != 0);
  int theClientType = 0;
  int theClient = 0;
  int theServer = 0;
  rlBuffer theBuffer = {0};
  short port = kDefaultPort;
  char* envptr = 0;

  /* fprintf(stderr, "isAgentConnected = %d, isEnvConnected = %d, isExpConnected = %d\n",isAgentConnected, isEnvironmentConnected, isExperimentConnected); */

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

    /* fprintf(stderr, "%s %d: Connection Attempt\n", __FILE__, __LINE__); */

    rlRecvBufferData(theClient, &theBuffer);
    rlBufferRead(&theBuffer, 0, &theClientType, 1, sizeof(int));

    switch(theClientType) {
    case kAgentConnection:
      /* fprintf(stderr, "agent connected!\n"); */
      theAgentConnection = theClient;
      isAgentConnected = 1;
      break;

    case kEnvironmentConnection:
      /* fprintf(stderr, "env connected\n"); */
      theEnvironmentConnection = theClient;
      isEnvironmentConnected = 1;
      break;

    case kExperimentConnection:
      /* fprintf(stderr, "exp connected\n"); */
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
