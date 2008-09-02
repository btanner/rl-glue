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

#include <RL_glue.h>

static Action last_action     = {0};
static Observation last_state = {0};

static Reward last_reward  = 0;
static Reward total_reward = 0;

static int is_terminal     = 0;
static int num_steps       = 0;
static int total_steps     = 0;
static int num_episodes    = 0;

static Reward_observation_action_terminal roa;
static Observation_action oa;

void RL_init() {
  Task_specification task_spec = 0;

  task_spec = env_init();
  agent_init(task_spec);

  last_reward     = 0;
  total_reward    = 0;

  num_steps       = 0;
  is_terminal     = 0;
  total_steps     = 0;
  num_episodes    = 0;
}

Observation_action RL_start() {
  num_steps    = 1;
  is_terminal  = 0;
  total_reward = 0;

  last_state = env_start();
  last_action = agent_start(last_state);
  
  oa.o = last_state;
  oa.a = last_action;
  
  return oa;
}

Reward_observation_action_terminal RL_step() {
  Reward_observation ro;
  ro = env_step(last_action);
  last_reward = ro.r;
  last_state = ro.o;
  
  roa.r = ro.r;
  roa.o = ro.o;
  roa.terminal = ro.terminal;
  
  is_terminal = ro.terminal;
  total_reward += last_reward;

  if (ro.terminal == 1) {
    num_episodes += 1;
    total_steps  += num_steps;
    agent_end(last_reward);
  }
  else {
    last_action = agent_step(last_reward,last_state);
    num_steps += 1; /* increment num_steps if state is not terminal */
    roa.a = last_action;
  }
  return roa;
}

void RL_cleanup() {
  env_cleanup();
  agent_cleanup();
}

Message RL_agent_message(const Message message) {
  return agent_message(message);
}

Message RL_env_message(const Message message) {
  return env_message(message);
}

void RL_freeze() {
  agent_freeze();
}

void RL_episode(unsigned int maxStepsThisEpisode) {
  unsigned int x = 0;
  RL_start();
/* RL_start sets current step to 1, so we should start x at 1 */
  for ( x = 1; !is_terminal && (maxStepsThisEpisode == 0 ? 1 : x < maxStepsThisEpisode); ++x ) {
    RL_step();
  }
}

Reward RL_return() {
  return total_reward;
}

int RL_num_steps() {
  /* number of steps of the current or just completed episodes of run */
  return num_steps;
}

int RL_num_episodes() {
  return num_episodes;
}

State_key RL_get_state() {
  return env_get_state();
}

void RL_set_state(State_key sk) {
  env_set_state(sk);
}

void RL_set_random_seed(Random_seed_key rsk) {
  env_set_random_seed(rsk);
}

Random_seed_key RL_get_random_seed() {
  return env_get_random_seed();
}
