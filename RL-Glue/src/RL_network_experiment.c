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
