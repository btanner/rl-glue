#include <stdio.h>
#include "RL_Interface.h"

void run(int num_episodes)
{        
  int x = 0;
  for(x = 0; x < num_episodes; ++x)
  {
    RL_episode(10);
    fprintf(stderr, ".");
  }
}

int main(int argc, char *argv[])
{
  double average_reward = 0.0;
  double average_num_steps = 0.0;
  int num_episodes = 1000;
  
  RL_init(argc, argv);
  run(num_episodes);

  average_reward = RL_average_reward();
  average_num_steps = RL_average_num_steps();

  RL_cleanup();

  fprintf(stderr, "Average Reward: %f\n", average_reward);
  fprintf(stderr, "Average Num Steps: %f\n", average_num_steps);

  return 0;
}
