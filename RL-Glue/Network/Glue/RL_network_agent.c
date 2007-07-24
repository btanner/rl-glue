#include <Network/RL_network.h>

static int agentConnection = 0;

int rlDidAgentConnect()
{
  return agentConnection != 0;
}

void rlCloseAgentConnection()
{
  rlBuffer theBuffer = {0};

  rlBufferCreate(&theBuffer, 8);
  rlSendBufferData(agentConnection, &theBuffer, kRLTerm);

  rlClose(agentConnection);
  agentConnection = 0;

  rlBufferDestroy(&theBuffer);
}

void rlSetAgentConnection(int connection)
{
  if (rlDidAgentConnect())
    rlCloseAgentConnection();

  agentConnection = connection;
}

int rlGetAgentConnection()
{
  return agentConnection;
}

