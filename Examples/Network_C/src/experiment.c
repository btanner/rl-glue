#include <stdio.h>
#include <stdlib.h>
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
  int trial = 0;
  unsigned int i = 0;
  double avg_steps = 0.0;
  double avg_return = 0.0;

  char* buffer = 0;

  Random_seed_key key = {0};
  key.numInts = 10;
  key.numDoubles = 10;
  key.intArray = (int*)malloc(key.numInts * sizeof(int));
  key.doubleArray = (double*)malloc(key.numDoubles * sizeof(double));

  for (i = 0; i < 10; ++i)
  {
    key.intArray[i] = i;
    key.doubleArray[i] = (double)i;
  }

  for (trial = 0; trial < 1; ++trial) {

    char* agent_response = RL_agent_message("query_agent");
    char* env_response = RL_env_message("query_env");

    fprintf(stderr, "agent_response: |%s|\n", agent_response);
    fprintf(stderr, "env_response: |%s|\n", env_response);

    RL_init();

    for ( i = 0; i < 100; ++i) {
      buffer = (char*)calloc(91414, sizeof(char));      
      fprintf(stderr, "env_response: %s\n", RL_env_message(buffer));
      free(buffer);
    }

    RL_set_random_seed(key);

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
  }
   
  return 0;
}
