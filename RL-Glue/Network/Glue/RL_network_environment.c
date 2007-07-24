#include <Network/RL_network.h>

static int envConnection = 0;

int rlDidEnvironmentConnect()
{
  return envConnection != 0;
}

void rlCloseEnvironmentConnection()
{
  rlBuffer theBuffer = {0};
  rlBufferCreate(&theBuffer, 8);
  rlSendBufferData(envConnection, &theBuffer, kRLTerm);

  rlClose(envConnection);
  envConnection = 0;

  rlBufferDestroy(&theBuffer);
}

void rlSetEnvironmentConnection(int connection)
{
  if (rlDidEnvironmentConnect())
    rlCloseEnvironmentConnection();

  envConnection = connection;
}

int rlGetEnvironmentConnection()
{
  return envConnection;
}
