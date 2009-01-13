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

int whichEpisode=0;

void evaluate_agent();



/*
	This function will freeze the agent's policy and test it after every 25 episodes.
*/
void offline_demo(){
	int i=0;
	int j=0;

	printf("0");
	evaluate_agent();
	for(i=0;i<20;i++){
		for(j=0;j<25;j++){
			RL_episode(0);
		}
		printf("%d",((i+1)*25));
		evaluate_agent();
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
	evaluate_agent();
	
	printf("\nLoading up the value function we saved earlier.\n");
	RL_agent_message("load_policy results.dat");

	printf("Evaluating the agent after loading the value function:\n\t\tMean Return\tStandardDeviation\n------------------------------------------------------\n");
	evaluate_agent();
	printf("\nProgram Complete.\n");
	RL_cleanup();


	return 0;
}


void evaluate_agent(){
	int i=0;
	double sum=0;
	double sum_of_squares=0;
	double this_return=0;
	double mean;
	double variance;
	double n=100.0f;

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
	printf("\t\t%.2f\t",mean);
	if(mean>=100.0f){
		printf("\t");
	}
	printf("\t+/- %.2f \n",sqrt(variance));

	RL_agent_message("unfreeze learning");
}
