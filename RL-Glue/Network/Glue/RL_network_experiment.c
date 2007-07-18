int expConnection = 0;

int rlDidExperimentConnect()
{
  return expConnection != 0;
}

void rlSetExperimentConnection(int connection)
{
  expConnection = connection;
}
