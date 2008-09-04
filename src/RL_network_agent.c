/* 
* Copyright (C) 2007, Adam White

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 */

#include <RL_network.h>

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

