/* 
* Copyright (C) 2007, Adam White
* 
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (at your option) any later version.
* 
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* 
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA. */

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
