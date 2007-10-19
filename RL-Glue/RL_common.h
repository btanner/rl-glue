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

#ifndef RLcommon_h
#define RLcommon_h

typedef char* Task_specification;
typedef double Reward;

typedef struct RL_abstract_type_t
{
  unsigned int numInts;
  unsigned int numDoubles;
  int* intArray;
  double* doubleArray;
} RL_abstract_type;

typedef RL_abstract_type Observation;
typedef RL_abstract_type Action;
typedef RL_abstract_type Random_seed_key;
typedef RL_abstract_type State_key;
typedef char* Message;

typedef struct {
  Observation o;
  Action a;
} Observation_action;

typedef struct Reward_observation_t
{
  Reward r;
  Observation o;
  int terminal;
} Reward_observation;

typedef struct {
  Reward r;
  Observation o;
  Action a;
  int terminal;
} Reward_observation_action_terminal;

#endif
