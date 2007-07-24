#include <Network/RL_network.h>

static int expConnection = 0;

int rlDidExperimentConnect()
{
  return expConnection != 0;
}

void rlSetExperimentConnection(int connection)
{
  expConnection = connection;
}

void rlCloseExperimentConnection()
{
  rlClose(expConnection);
}
