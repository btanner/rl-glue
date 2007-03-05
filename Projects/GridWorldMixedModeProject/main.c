//Must include all the following files
#include "RL_Interface.h"
#include "gnu.h"
#include "RL_util.h"
#include <time.h>
#include <stdio.h>
/*********************************************************************************
*	Benchmark driver program. 
*
*	Completely defines the experimental setup of the benchmark. Mostly format
*	episodic/continuing, online/batch/train&test.
*
*	TEMPLATE
*********************************************************************************/
time_t start;
time_t stop;

//Method runs an experiment
void run(int num_episodes)
{        
	for(int x=0; x < num_episodes; x++)
        {
            RL_episode();	//run a single episode
            printf(".");
            fflush(stdout);
            collectStats();	//collect stats on episode
        }
}

void startTimer(time_t *strt)
{
	time(strt);
}


void endTimer(time_t *stp)
{
	time(stp);
}


void printTimeTaken()
{
	int time = (int)(stop - start);
	printf("\nTIMING STATS::");
	printf("\n**************************************************************\n");
	printf("Time taken to complete the run: %d seconds\n",time);
	printf("**************************************************************\n");
}

//MAIN PROGRAM
int main(int argc, char *argv[])
{
		int num_episodes = 1000;
                
		startTimer(&start);
		RL_init();					//initialize interface
		initStats(num_episodes);	//initialize stats routine
		
		run(num_episodes);			//run experiment	
            
		endStats(num_episodes);	//end stats collection
		endTimer(&stop);
		
		printTimeTaken();
		RL_cleanup();
		graph(num_episodes,false);	//graph stats
}
