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
#include <string.h>
#include <assert.h>
#include <rlglue/RL_glue.h>
	
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

	RL_start();

	roat=RL_step();

	
	
	check_fail(roat.o.numInts!=1);
	check_fail(roat.o.numDoubles!=0);
	check_fail(roat.o.numChars!=0);
	check_fail(roat.o.intArray[0]!=0);
    check_fail(strcmp("one|1.|one",RL_env_message("one"))!=0);
    check_fail(strcmp("one|1.|one",RL_agent_message("one"))!=0);
	check_fail(roat.terminal!=0);
	

	roat=RL_step();

    check_fail(strcmp("two|2.2.|two",RL_env_message("two"))!=0);
    check_fail(strcmp("two|2.2.|two",RL_agent_message("two"))!=0);
	check_fail(roat.terminal!=0);
	check_fail(roat.o.numInts!=1);
	check_fail(roat.o.numDoubles!=0);
	check_fail(roat.o.numChars!=0);
	check_fail(roat.o.intArray[0]!=1);

	roat=RL_step();

    check_fail(strcmp("three||three",RL_env_message("three"))!=0);
    check_fail(strcmp("three||three",RL_agent_message("three"))!=0);
	check_fail(roat.terminal!=0);
	check_fail(roat.o.numInts!=1);
	check_fail(roat.o.numDoubles!=0);
	check_fail(roat.o.numChars!=0);	
	check_fail(roat.o.intArray[0]!=2);

	roat=RL_step();
    check_fail(strcmp("four|4.|four",RL_env_message("four"))!=0);
    check_fail(strcmp("four|4.|four",RL_agent_message("four"))!=0);
	check_fail(roat.terminal!=0);
	check_fail(roat.o.numInts!=1);
	check_fail(roat.o.numDoubles!=0);
	check_fail(roat.o.numChars!=0);
	check_fail(roat.o.intArray[0]!=3);
	

	roat=RL_step();
    check_fail(strcmp("five|5.5.|five",RL_env_message("five"))!=0);
	check_fail(strcmp("five|4.|five",RL_agent_message("five"))!=0);
	check_fail(roat.terminal==0);
	
	if(tests_failed!=0)
		printf("Failed %d / %d checks in %s\n",tests_failed,test_count, __FILE__);
	else
		printf("Passed all %d checks in %s\n",test_count,__FILE__);
	return tests_failed;
}
