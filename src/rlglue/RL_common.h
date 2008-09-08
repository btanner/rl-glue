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


typedef char* task_specification_t;
typedef double reward_t;
typedef int terminal_t;

/* Strings are not guaranteed to be null terminated I think */
typedef struct rl_abstract_t_struct
{
	unsigned int numInts;
	unsigned int numDoubles;
	unsigned int numChars;
	int* intArray;
	double* doubleArray;
	char* charArray;
} rl_abstract_type_t;

typedef rl_abstract_type_t observation_t;
typedef rl_abstract_type_t action_t;
typedef rl_abstract_type_t random_seed_key_t;
typedef rl_abstract_type_t state_key_t;
typedef char* message_t;

typedef struct observation_action_struct{
  observation_t o;
  action_t a;
} observation_action_t;

typedef struct reward_observation_struct
{
  reward_t r;
  observation_t o;
  terminal_t terminal;
} reward_observation_t;

typedef struct {
  reward_t r;
  observation_t o;
  action_t a;
  terminal_t terminal;
} reward_observation_action_terminal_t;

#ifdef __cplusplus
}
#endif

#endif
