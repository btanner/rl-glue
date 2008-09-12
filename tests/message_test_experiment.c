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


#include <stdio.h>

#include <rlglue/Experiment_common.h>
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

	check_fail(strcmp("null",RL_env_message(0))!=0);
	check_fail(strcmp("empty",RL_env_message(""))!=0);
	check_fail(strcmp("null",RL_agent_message(0))!=0);
	check_fail(strcmp("empty",RL_agent_message(""))!=0);

	check_fail(strcmp("",RL_env_message("empty"))!=0);
	check_fail(strcmp("",RL_agent_message("empty"))!=0);

	check_fail(RL_env_message("null")!=0);
	check_fail(RL_agent_message("null")!=0);

	check_fail(strcmp("1",RL_env_message("1"))!=0);
	check_fail(strcmp("1",RL_agent_message("1"))!=0);

	check_fail(strcmp("1000000000000000000000",RL_env_message("1000000000000000000000"))!=0);
	check_fail(strcmp("1000000000000000000000",RL_agent_message("1000000000000000000000"))!=0);

	check_fail(strcmp("21111111111111111111111111111111111111111111111111111111311111111111111111111111111111111111111111111111111111113",RL_env_message("21111111111111111111111111111111111111111111111111111111311111111111111111111111111111111111111111111111111111113"))!=0);
	check_fail(strcmp("45555555555555555555555555555555555555555555555555555555655555555555555555555555555555555555555555555555555555559",RL_agent_message("45555555555555555555555555555555555555555555555555555555655555555555555555555555555555555555555555555555555555559"))!=0);

	if(tests_failed!=0)
		printf("Failed %d / %d in message_test 1\n",tests_failed, test_count);

	return tests_failed;
}
