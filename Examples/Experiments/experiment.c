/* 
* Copyright (C) 2007, Brian Tanner
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
