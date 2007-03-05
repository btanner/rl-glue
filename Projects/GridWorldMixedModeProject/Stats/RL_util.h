#include "RL_Interface.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
/**********************************************************************************
*	This file contains various methods that you can use to collect statisitics
*	while your program runs. Each is explained in turn.
**********************************************************************************/
FILE *f1, *f2, *f3;
double *averageRewardData, *cummulativeRewardData, *numberStepsData;
double endCumReward, endAverageReward, cummulativeReward;

double episodeTime;
int numberOfSteps;

timeval t_start_util;
timeval t_end_util;

int *numberSteps;
int utilCount;
char buf[10000];
 
void util_startTimer(timeval *s);
void util_endTimer(timeval *ss);

 
// Intialization routine - call before running to setup stats
// for episodic online learning tasks 
void initStats(int num_episodes)
{
    char temp1[5000], temp2[5000];
    sprintf(temp2,"_%d.dat", num_episodes);

    strcpy(temp1,"Data/averageRewardChallenge");
    strcat(temp1,temp2);
    f1 = fopen(temp1,"w");
    
    strcpy(temp1,"Data/numberstepsChallenge");
    strcat(temp1,temp2);    
    f2 = fopen(temp1,"w");
    
    strcpy(temp1,"Data/cummulativeRewardChallenge");
    strcat(temp1,temp2);     
    f3 = fopen(temp1,"w");
	
    numberSteps = new int[num_episodes];
	cummulativeReward = 0;
    utilCount = 0;
}

// Intialization routine - call before running to setup stats
// for continuing online learning tasks 
void initStatsCont()
{
    char temp1[5000], temp2[5000];
    strcpy(temp2,"CONT.dat");

    strcpy(temp1,"Data/averageRewardChallenge");
    strcat(temp1,temp2);
    f1 = fopen(temp1,"w");
    
    strcpy(temp1,"Data/cummulativeRewardChallenge");
    strcat(temp1,temp2);     
    f3 = fopen(temp1,"w");
	
	cummulativeReward = 0;
}
// Intialization routine - call before running to setup stats
// for episodic batch learning tasks 
void initStatsBatch(int num_episodes)
{
    char temp1[5000], temp2[5000];
    sprintf(temp2,"_%d.dat", num_episodes);

    strcpy(temp1,"Data/averageRewardChallenge");
    strcat(temp1,temp2);
    f1 = fopen(temp1,"w");
    
    strcpy(temp1,"Data/numberstepsChallenge");
    strcat(temp1,temp2);    
    f2 = fopen(temp1,"w");
    
    strcpy(temp1,"Data/cummulativeRewardChallenge");
    strcat(temp1,temp2);     
    f3 = fopen(temp1,"w");
		    
    averageRewardData = new double[num_episodes];
    cummulativeRewardData = new double[num_episodes];
    numberStepsData = new double[num_episodes];
    
    for(int x=0; x< num_episodes; x++)
    {
        averageRewardData[x] = 0;
        cummulativeRewardData[x] = 0;
        numberStepsData[x] = 0;
    }
	cummulativeReward = 0;
}
// Intialization routine - call before running to setup stats
// for continuing batch learning tasks 
void initStatsCont(int num_steps)
{
    initStatsCont();
    
    averageRewardData = new double[num_steps];
    cummulativeRewardData = new double[num_steps];
    
    for(int x=0; x<num_steps; x++)
    {
        averageRewardData[x] = 0;
        cummulativeRewardData[x] = 0;
    }
	
	cummulativeReward = 0;
}
// Collect routine - call every time RL interface takes a step
// for episodic online learning tasks
void collectStats()
{	
    sprintf(buf,"%lf\n",RL_average_reward());
    fputs(buf,f1);
    endAverageReward = RL_average_reward();
	cummulativeReward += RL_return();
    sprintf(buf,"%lf\n",cummulativeReward);
    fputs(buf,f3);
    endCumReward = cummulativeReward;
    sprintf(buf,"%d\n",RL_num_steps());
    fputs(buf,f2);	
    numberSteps[utilCount] = RL_num_steps();
    utilCount++;
}

// Collect routine - call every time RL interface takes a step
// for continuing online learning tasks
void collectStatsCont()
{	
    sprintf(buf,"%lf\n",RL_average_reward());
    fputs(buf,f1);
    endAverageReward = RL_average_reward();  
	cummulativeReward += RL_return();
	sprintf(buf,"%lf\n",cummulativeReward);
    fputs(buf,f3);	
    endCumReward = cummulativeReward;    
}
// Collect routine - call every time RL interface takes a step
// for episodic batch learning tasks
void collectStats(int x)
{
	cummulativeReward += RL_return();
	
    averageRewardData[x] = averageRewardData[x] + RL_average_reward();
    cummulativeRewardData[x] = cummulativeRewardData[x] + cummulativeReward;
    numberStepsData[x] = numberStepsData[x] + RL_num_steps();
}
// Collect routine - call every time RL interface takes a step
// for continuing batch learning tasks
void collectStatsCont(int x)
{
	cummulativeReward += RL_return();

    averageRewardData[x] = averageRewardData[x] + RL_average_reward();
    cummulativeRewardData[x] = cummulativeRewardData[x] + cummulativeReward;
}
// clean up routine - call after training/testing is done
// for episodic batch learning tasks
void endStatsBatch(int num_episodes, int num_batches)
{
    double num = 0, sum = 0;
    double meanSteps, varOfSteps;
	
    for(int x=0; x< num_episodes; x++)
    {
        numberStepsData[x] = numberStepsData[x]/num_batches;
	
        sprintf(buf,"%lf\n",averageRewardData[x]/num_batches);
        fputs(buf,f1);
        endAverageReward = averageRewardData[x]/num_batches;
        sprintf(buf,"%lf\n",cummulativeRewardData[x]/num_batches);
        fputs(buf,f3);
        endCumReward = cummulativeRewardData[x]/num_batches;
        
        sprintf(buf,"%lf\n",numberStepsData[x]);    
        fputs(buf,f2);
    }
    fclose(f1);
    fclose(f2);
    fclose(f3);
	
    for(int x=0; x< num_episodes; x++)
    {
        sum += numberStepsData[x];
    }
    meanSteps = sum/num_episodes;
    for(int x=0; x< num_episodes; x++)
    {
        num += (numberStepsData[x] - meanSteps)*(numberStepsData[x] - meanSteps);
    }
    varOfSteps = num/(num_episodes - 1);
    
    printf("\nDATA STATS::");
    printf("\n-------------------------------------------------------\n");
    printf("The mean number of steps is = %f\n",meanSteps);
    printf("The variance in the number of steps is = %f\n",varOfSteps);
    printf("--\n");
    printf("The final average reward is = %f\n",endAverageReward);
    printf("The final cummulative reward is = %f\n",endCumReward);        
    printf("-------------------------------------------------------\n");
		
    delete [] averageRewardData; averageRewardData = 0;
    delete [] cummulativeRewardData; cummulativeRewardData = 0;
    delete [] numberStepsData; numberStepsData = 0;
}
// clean up routine - call after training/testing is done
// for continuing batch learning tasks
void endStatsCont(int num_steps, int num_batches)
{
    for(int x=0; x<num_steps; x++)
    {
        sprintf(buf,"%lf\n",averageRewardData[x]/num_batches);
        fputs(buf,f1);
        endAverageReward = averageRewardData[x]/num_batches;        
        sprintf(buf,"%lf\n",cummulativeRewardData[x]/num_batches);
        fputs(buf,f3);
        endCumReward = cummulativeRewardData[x]/num_batches;        
    }
    fclose(f1);
    fclose(f3);
    
    printf("\nDATA STATS::");
    printf("\n-------------------------------------------------------\n");
    printf("The final average reward is = %f\n",endAverageReward);
    printf("The final cummulative reward is = %f\n",endCumReward);  
    printf("-------------------------------------------------------\n");

    delete [] averageRewardData; averageRewardData = 0;
    delete [] cummulativeRewardData; cummulativeRewardData = 0;
}
// clean up routine - call after training/testing is done
// for episodic online learning tasks
void endStats(int num_episodes)
{
	double num = 0, sum = 0;
	double meanSteps, varOfSteps;
	
	for(int x=0; x< num_episodes; x++)
	{
            sum += numberSteps[x];
	}	
	meanSteps = sum/num_episodes;
	for(int x=0; x< num_episodes; x++)
	{
            num = num + (numberSteps[x] - meanSteps)*(numberSteps[x] - meanSteps);
	}
	varOfSteps = num/(num_episodes - 1);
	
        printf("\nDATA STATS::");        
	printf("\n-------------------------------------------------------\n");
	printf("The mean number of steps is = %f\n",meanSteps);
	printf("The variance in the number of steps is = %f\n",varOfSteps);
        printf("--\n");
        printf("The final average reward is = %f\n",endAverageReward);
        printf("The final cummulative reward is = %f\n",endCumReward);        
	printf("-------------------------------------------------------\n");
	delete [] numberStepsData; numberStepsData = 0;
	
    fclose(f1);
    fclose(f2);
    fclose(f3);
}

// clean up routine - call after training/testing is done
// for continuing online learning tasks
void endStatsCont()
{
    fclose(f1);
    fclose(f3);
    
    printf("\nDATA STATS::");
    printf("\n-------------------------------------------------------\n");
    printf("The final average reward is = %f\n",endAverageReward);
    printf("The final cummulative reward is = %f\n",endCumReward);  
    printf("-------------------------------------------------------\n");    
}

void clearCummulativeReward()
{
	cummulativeReward = 0;
}

void util_startTimer(timeval *strt)
{
	gettimeofday(strt, NULL);
}


void util_endTimer(timeval *stp)
{
	gettimeofday(stp,NULL);
}
