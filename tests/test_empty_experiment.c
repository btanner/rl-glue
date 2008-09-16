/* 
* Copyright (C) 2007, Brian Tanner

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.

* 
*  $Revision$
*  $Date$
*  $Author$
*  $HeadURL$
* 
*/
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif


#include <stdio.h>

#include <rlglue/RL_glue.h>
#include <string.h>

	int tests_failed=0;
	int test_count=0;

	void check_fail(int condition){
		test_count++;
		if(condition!=0){
			printf("Failed check: %d\n",test_count);
			tests_failed++;
		}
	}

	

int main(int argc, char *argv[]) {
	char *theResponse;
	int whichEpisode=0;
	int whichStep=0;
	observation_action_t startTuple={0};
	reward_observation_action_terminal_t stepTuple={0};

	RL_init();
	
	for(whichEpisode=1;whichEpisode<5;whichEpisode++){
		startTuple=RL_start();
		
		if(whichEpisode%2==0){
			check_fail(startTuple.a.numInts!=0);
			check_fail(startTuple.a.numDoubles!=0);
			check_fail(startTuple.a.numChars!=0);

			check_fail(startTuple.o.numInts!=0);
			check_fail(startTuple.o.numDoubles!=0);
			check_fail(startTuple.o.numChars!=0);
		}else{
			check_fail(startTuple.a.numInts!=7);
			check_fail(startTuple.a.numDoubles!=3);
			check_fail(startTuple.a.numChars!=1);

			check_fail(startTuple.o.numInts!=2);
			check_fail(startTuple.o.numDoubles!=4);
			check_fail(startTuple.o.numChars!=5);
		}
		
		for(whichStep=0;whichStep<5;whichStep++){
			stepTuple=RL_step();
			check_fail(stepTuple.terminal!=0);
			check_fail(stepTuple.r!=0);

			if(whichEpisode%2==0){
				check_fail(stepTuple.a.numInts!=0);
				check_fail(stepTuple.a.numDoubles!=0);
				check_fail(stepTuple.a.numChars!=0);

				check_fail(stepTuple.o.numInts!=0);
				check_fail(stepTuple.o.numDoubles!=0);
				check_fail(stepTuple.o.numChars!=0);
			}else{
				check_fail(stepTuple.a.numInts!=7);
				check_fail(stepTuple.a.numDoubles!=3);
				check_fail(stepTuple.a.numChars!=1);

				check_fail(stepTuple.o.numInts!=2);
				check_fail(stepTuple.o.numDoubles!=4);
				check_fail(stepTuple.o.numChars!=5);
			}
			
		}
		
		
	}
	

	if(tests_failed!=0)
		printf("Failed %d / %d checks in %s\n",tests_failed,test_count, __FILE__);
	else
		printf("Passed all %d checks in %s\n",test_count,__FILE__);
	return tests_failed;
}
