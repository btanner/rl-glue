static int agentConnection = 0;

int rlDidAgentConnect()
{
  return agentConnection != 0;
}

void rlSetAgentConnection(int connection)
{
  agentConnection = connection;
}

int rlGetAgentConnection()
{
  return agentConnection;
}
