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
  int num_episodes = 1000;
  
  RL_init(argc, argv);
  run(num_episodes);
  RL_cleanup();

  return 0;
}
