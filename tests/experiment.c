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
 */

#include <stdio.h>

#include <rlglue/Experiment_common.h>
	

int main(int argc, char *argv[]) {
  int failed=0;
  reward_observation_action_terminal_t roat;
  task_specification_t task_spec;
  task_spec=RL_init();
  printf("Task spec was: %s\n",task_spec);

    failed |= strcmp("sample task spec",task_spec)!=0;


    RL_start();
	roat=RL_step();

    failed |= strcmp("one|1.|one",RL_env_message("one"))!=0;
    failed |= strcmp("one|1.|one",RL_agent_message("one"))!=0;
	failed |= roat.terminal=0;

	roat=RL_step();

    failed |= strcmp("two|2.2.|two",RL_env_message("two"))!=0;
    failed |= strcmp("two|2.2.|two",RL_agent_message("two"))!=0;
	failed |= roat.terminal=0;

	roat=RL_step();

    failed |= strcmp("three||three",RL_env_message("three"))!=0;
    failed |= strcmp("three||three",RL_agent_message("three"))!=0;
	failed |= roat.terminal=0;

	roat=RL_step();
    failed |= strcmp("four|4.|four",RL_env_message("four"))!=0;
    failed |= strcmp("four|4.|four",RL_agent_message("four"))!=0;
	failed |= roat.terminal=0;

	roat=RL_step();
    failed |= strcmp("five|5.5.|five",RL_env_message("five"))!=0;
    failed |= strcmp("five|4.|five",RL_agent_message("five"))!=0;
	failed |= roat.terminal=0;

	return failed;
}
