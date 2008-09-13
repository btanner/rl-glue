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
*  $Revision: 809 $
*  $Date: 2008-09-11 22:56:21 -0600 (Thu, 11 Sep 2008) $
*  $Author: brian@tannerpages.com $
*  $HeadURL: https://rl-glue.googlecode.com/svn/trunk/tests/experiment.c $
* 
*/


#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <rlglue/Experiment_common.h>
	
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
  reward_observation_action_terminal_t roat;
  task_specification_t task_spec;
  task_spec=RL_init();

  check_fail(strcmp(task_spec,"sample task spec")!=0);
  
	if(tests_failed!=0)
		printf("Failed %d / %d checks in %s\n",tests_failed,test_count, __FILE__);
	else
		printf("Passed all %d checks in %s\n",test_count,__FILE__);
	return tests_failed;
}
