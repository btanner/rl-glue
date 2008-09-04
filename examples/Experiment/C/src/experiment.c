/* 
* Copyright (C) 2007, Brian Tanner

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

#include <stdio.h>
#include <RL_glue.h>

#define NUM_EPISODES 1000
int rl_num_steps[NUM_EPISODES];
double rl_return[NUM_EPISODES];

void run(int num_episodes) {        
  int x = 0;
  for(x = 0; x < num_episodes; ++x) {
    RL_episode(0);
    fprintf(stderr, ".");
    rl_num_steps[x] = RL_num_steps();
    rl_return[x] = RL_return();
  }
}

int main(int argc, char *argv[]) {
  unsigned int i = 0;
  double avg_steps = 0.0;
  double avg_return = 0.0;

  RL_init();
  run(NUM_EPISODES);
  RL_cleanup();
  for (i = 0; i < NUM_EPISODES; i++) {
    avg_steps += rl_num_steps[i];
    avg_return += rl_return[i];
  }

  avg_steps /= NUM_EPISODES;
  avg_return /= NUM_EPISODES;
  printf("\n-----------------------------------------------\n");
  printf("Number of episodes: %d\n",NUM_EPISODES);
  printf("Average number of steps per episode: %f\n", avg_steps);
  printf("Average return per episode: %f\n", avg_return);
  printf("-----------------------------------------------\n");
  
  return 0;
}
