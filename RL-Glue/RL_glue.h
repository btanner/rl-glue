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

#ifndef RL_interface_h
#define RL_interface_h

#include <RL_common.h>
#include <Environment_common.h>
#include <Agent_common.h>

/* Glue */
void RL_init();
Observation_action RL_start();
Reward_observation_action_terminal RL_step();
void RL_cleanup();

Message RL_agent_message(Message message);
Message RL_env_message(Message message);

Reward RL_return();
int RL_num_steps();
int RL_num_episodes();
void RL_episode(unsigned int num_steps);
/* void RL_episode(); */
void RL_freeze();
void RL_set_state(State_key sk);
void RL_set_random_seed(Random_seed_key rsk);
State_key RL_get_state();
Random_seed_key RL_get_random_seed();

#endif
