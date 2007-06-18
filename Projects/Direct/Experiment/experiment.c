#include <stdio.h>
#include <RL_glue.h>

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
  RL_init();
  run(100);
  RL_cleanup();

  return 0;
}
