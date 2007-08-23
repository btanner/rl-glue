#include <stdio.h>
#include <stdlib.h>
#include <RL_glue.h>

#define NUM_EPISODES 1
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

  char* agent_response = 0;
  char* env_response = 0;

  Random_seed_key key = {0};
  key.numInts = 2;
  key.numDoubles = 2;
  key.intArray = (int*)calloc(key.numInts, sizeof(int));
  key.doubleArray = (double*)calloc(key.numDoubles, sizeof(double));

  for (i = 0; i < key.numInts; ++i)
    key.intArray[i] = i;

  for (i = 0; i < key.numDoubles; ++i)
    key.doubleArray[i] = (double) i;

  for (trial = 0; trial < 1; ++trial) {

    agent_response = RL_agent_message(0);
    env_response = RL_env_message(0);

    fprintf(stderr, "agent_response: |%s|\n", agent_response);
    fprintf(stderr, "env_response: |%s|\n", env_response);

    RL_init();

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
