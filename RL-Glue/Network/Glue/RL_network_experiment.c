#include <Network/RL_network.h>

static int expConnection = 0;

int rlDidExperimentConnect()
{
  return expConnection != 0;
}

void rlCloseExperimentConnection()
{
  rlClose(expConnection);
  expConnection = 0;
}

void rlSetExperimentConnection(int connection)
{
  /* We can't really send a term signal back to the user benchmark,
     they won't know what to do with it. */
  if (rlDidExperimentConnect())
    rlCloseExperimentConnection();

    expConnection = connection;
}
