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
static reward_observation_terminal_t ro={0};
static char* responseMessage=0;
static int stepCount=0;

const char* env_init()
{    
	o=allocateRLStructPointer(0,0,0);
	return "sample task spec";
}

const observation_t *env_start()
{
	stepCount=0;
	clearRLStruct(o);
	makeKInts(o,1);
	makeKDoubles(o,2);
	makeKChars(o,3);
	__RL_CHECK_STRUCT(o)
	return o;
}

const reward_observation_terminal_t *env_step(const action_t *a)
{
	clearRLStruct(o);
	makeKInts(o,1);
	o->intArray[0]=stepCount;
	
	ro.observation=o;
	ro.reward=1.0;
  
	stepCount++;
	
	ro.terminal=stepCount==5;
	__RL_CHECK_STRUCT(o)
    return &ro;
}

void env_cleanup()
{
	freeRLStructPointer(o);
	o=0;
	if(responseMessage!=0){
	
		free(responseMessage);
		responseMessage=0;
	} 
}


const char* env_message(const char* inMessage) {
	int timesToPrint=stepCount%3;
	int i;
	char tmpBuffer[1024];
	
	sprintf(tmpBuffer,"%s|",inMessage);
	for(i=0;i<timesToPrint;i++){
		sprintf(tmpBuffer,"%s%d.", tmpBuffer,stepCount);
	}
	sprintf(tmpBuffer,"%s|%s",tmpBuffer,inMessage);
	
	if(responseMessage!=0){
		free(responseMessage);
		responseMessage=0;
	}
	responseMessage=(char *)calloc(strlen(tmpBuffer)+1,sizeof(char));
	sprintf(responseMessage,"%s",tmpBuffer);
	return responseMessage;
}
