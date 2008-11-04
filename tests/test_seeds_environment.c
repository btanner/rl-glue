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


#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <rlglue/Environment_common.h>
#include <rlglue/utils/C/RLStruct_util.h>
#include "useful_functions.h"



static observation_t *o=0;

const char* env_init()
{    
	o=allocateRLStructPointer(0,0,0);
	return "";
}

const observation_t *env_start()
{
	clearRLStruct(o);
	return o;
}

const reward_observation_terminal_t *env_step(const action_t *a)
{
	static reward_observation_terminal_t ro={0};
	clearRLStruct(o);
	ro.observation=o;
	return &ro;
}

void env_cleanup()
{
	freeRLStructPointer(o);
	o=0;
}


const char* env_message(const char* inMessage) {
	return "";
}
	
