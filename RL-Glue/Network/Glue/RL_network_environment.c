static int envConnection = 0;

int rlDidEnvironmentConnect()
{
  return envConnection != 0;
}

void rlSetEnvironmentConnection(int connection)
{
  envConnection = connection;
}

int rlGetEnvironmentConnection()
{
  return envConnection;
}
