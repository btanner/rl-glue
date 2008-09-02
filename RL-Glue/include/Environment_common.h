/* 
* Copyright (C) 2007, Matt Radkie
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

#ifndef Environment_common_h
#define Environment_common_h

#include <RL_common.h>

/*
#ifdef __cplusplus
extern "C" {
#endif
*/
	/* Environment Interface */
	Task_specification env_init();
	Observation env_start();
	Reward_observation env_step(Action a);
	void env_cleanup();
	void env_set_state(State_key sk);
	void env_set_random_seed(Random_seed_key rsk);
	State_key env_get_state();
	Random_seed_key env_get_random_seed();
	Message env_message(const Message message);
/*
#ifdef __cplusplus
}
#endif
*/
#endif
