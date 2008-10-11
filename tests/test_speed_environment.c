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
*  $Revision: 134 $
*  $Date: 2008-09-16 16:28:05 -0600 (Tue, 16 Sep 2008) $
*  $Author: brian@tannerpages.com $
*  $HeadURL: https://rl-glue-ext.googlecode.com/svn/trunk/projects/codecs/C/tests/test_1_environment.c $
* 
*/
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif


#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <rlglue/Environment_common.h>
#include <rlglue/utils/C/RLStruct_util.h>
#include "useful_functions.h"


observation_t *o=0;
reward_observation_t ro={0};
char* env_responseMessage=0;
int env_stepCount=0;
int env_episodeCount=0;

const char* env_init()
{    
	o=allocateRLStructPointer(0,0,0);
	return "sample task spec";
}

const observation_t *env_start()
{
	env_episodeCount++;
	env_stepCount=0;
	clearRLStruct(o);
	__RL_CHECK_STRUCT(o)
	return o;
}

const reward_observation_t *env_step(const action_t *a)
{
	int terminal=0;
	env_stepCount++;
	clearRLStruct(o);
	    
        /*Short episode with big observations*/
        if(env_episodeCount%2==0){
			__RL_CHECK_STRUCT(o)
            set_k_ints_in_abstract_type(o, 50000);
			__RL_CHECK_STRUCT(o)
            set_k_doubles_in_abstract_type(o, 50000);
			__RL_CHECK_STRUCT(o)

            if(env_stepCount==200)terminal=1;
        }
        /*Longer episode with smaller observations*/
        if(env_episodeCount%2==1){
			__RL_CHECK_STRUCT(o)
            set_k_ints_in_abstract_type(o, 5);
		__RL_CHECK_STRUCT(o)
            set_k_doubles_in_abstract_type(o, 5);
		__RL_CHECK_STRUCT(o)

            if(env_stepCount==5000)terminal=1;
        }

	
	ro.observation=o;
	ro.reward=1.0;
	ro.terminal=terminal;
	__RL_CHECK_STRUCT(ro.observation)
    return &ro;
}

void env_cleanup()
{
  clearRLStruct(o);
}

void env_set_state(const state_key_t *sk)
{
}
     
void env_set_random_seed(const random_seed_key_t *rsk)
{
}

const state_key_t *env_get_state()
{
	state_key_t *theKey=0;
	clearRLStruct(theKey);
	return theKey;
}

const random_seed_key_t *env_get_random_seed()
{
	random_seed_key_t *theKey=0;
	clearRLStruct(theKey);
	return theKey;
}

const char* env_message(const char* inMessage) {
	return "";
}
