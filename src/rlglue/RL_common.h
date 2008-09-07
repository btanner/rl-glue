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

#ifndef RLcommon_h
#define RLcommon_h

#ifdef __cplusplus
extern "C" {
#endif


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

#ifdef __cplusplus
}
#endif

#endif
