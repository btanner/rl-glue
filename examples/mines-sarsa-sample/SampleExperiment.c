/* 
* Copyright (C) 2008, Brian Tanner

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

#include <stdio.h>	/* for printf */
#include <math.h> /* for sqrt */
#include <rlglue/RL_glue.h> /* RL_ function prototypes and RL-Glue types */

typedef struct {
	double mean;
	double standard_dev;
} evaluation_point_t;

int whichEpisode=0;
evaluation_point_t *evaluate_agent();



void save_result_csv(evaluation_point_t *the_score[], char *fileName){
	FILE *fp;
	int i=0;

	fp=fopen(fileName, "w");
	fprintf(fp, "#Results from SampleExperiment.c.  First line is means, second line is standard deviations.\n");
	
	for(i=0;i<21;i++){
		fprintf(fp,"%.2f,",the_score[i]->mean);
	}
	fprintf(fp,"\n");
	
	for(i=0;i<21;i++){
		fprintf(fp,"%.2f,",the_score[i]->standard_dev);
	}
	fprintf(fp,"\n");

	fclose(fp);
}

void print_score(int afterEpisodes, evaluation_point_t *the_score) {
    printf("%d\t\t%.2f\t\t%.2f\n", afterEpisodes, the_score->mean, the_score->standard_dev);
}


/*
	This function will freeze the agent's policy and test it after every 25 episodes.
*/
void offline_demo(){
	int i=0;
	int j=0;
	evaluation_point_t *this_score=0;
	evaluation_point_t *statistics[21];
	
	this_score=evaluate_agent();
	print_score(0,this_score);
	statistics[0]=this_score;
	
	for(i=0;i<20;i++){
		for(j=0;j<25;j++){
			RL_episode(0);
		}
		this_score=evaluate_agent();
		print_score((i+1)*25,this_score);
		statistics[i+1]=this_score;
	}
	
	save_result_csv(statistics,"results.csv");
	
	for(i=0;i<21;i++){
		free(statistics[i]);
	}
	
}

int main(int argc, char *argv[]) {
	printf("Starting offline demo\n----------------------------\nWill alternate learning for 25 episodes, then freeze policy and evaluate for 100 episodes.\n\n");
	printf("After Episode\tMean Return\tStandard Deviation\n-------------------------------------------------------------------------\n");
	RL_init();
	offline_demo();
	
	
	printf("\nNow we will save the agent's learned value function to a file....\n");

	RL_agent_message("save_policy results.dat");

	printf("\nCalling RL_cleanup and RL_init to clear the agent's memory...\n");

	RL_cleanup();
	RL_init();


	printf("Evaluating the agent's default policy:\n\t\tMean Return\tStandardDeviation\n------------------------------------------------------\n");
	print_score(0,evaluate_agent());
	
	printf("\nLoading up the value function we saved earlier.\n");
	RL_agent_message("load_policy results.dat");

	printf("Evaluating the agent after loading the value function:\n\t\tMean Return\tStandardDeviation\n------------------------------------------------------\n");
	print_score(0,evaluate_agent());

    printf("Evaluating the agent a few times from a fixed start state of 3,3:\n\t\tMean Return\tStandardDeviation\n-------------------------------------------\n");
    RL_env_message("set-start-state 3 3");
	print_score(0,evaluate_agent());

	printf("Evaluating the agent again with the random start state:\n\t\tMean Return\tStandardDeviation\n-----------------------------------------------------\n");
    RL_env_message("set-random-start-state");
	print_score(0,evaluate_agent());


	printf("\nProgram Complete.\n");
	RL_cleanup();

	return 0;
}


evaluation_point_t *evaluate_agent(){
	int i=0;
	double sum=0;
	double sum_of_squares=0;
	double this_return=0;
	double mean;
	double variance;
	double n=100.0f;
	evaluation_point_t *eval_point=0;
	
	RL_agent_message("freeze learning");
	for(i=0;i<10;i++){
		/* We use a cutoff here in case the policy is bad
		   and will never end an episode */
		RL_episode(5000);
		this_return=RL_return();
		sum+=this_return;
		sum_of_squares+=this_return*this_return;
	}
	
	mean=sum/n;
	variance = (sum_of_squares - n*mean*mean)/(n - 1.0f);
	eval_point=(evaluation_point_t *)malloc(sizeof(evaluation_point_t));
	eval_point->mean=mean;
	eval_point->standard_dev=sqrt(variance);

	RL_agent_message("unfreeze learning");
	return eval_point;
}
