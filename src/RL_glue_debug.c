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

#include <assert.h>
#include <rlglue/RL_common.h>


int __rlglue_check_abstract_type(const rl_abstract_type_t *theStruct){
	if(theStruct->numInts>1000000){printf("abstract type integrity error: numInts = %d\n",theStruct->numInts);return 1;}
	if(theStruct->numDoubles>1000000)return 2;
	if(theStruct->numChars>1000000)return 3;

	if(theStruct->numInts>0 && theStruct->intArray==0)return 4;
	if(theStruct->numDoubles>0 && theStruct->doubleArray==0)return 5;
	if(theStruct->numChars>0 && theStruct->charArray==0)return 6;

	if(theStruct->numInts==0 && theStruct->intArray!=0)return 7;
	if(theStruct->numDoubles==0 && theStruct->doubleArray!=0)return 8;
	if(theStruct->numChars==0 && theStruct->charArray!=0)return 9;
	
	return 0;
}

/* Take ths out later */
void __rlglue_print_abstract_type(const rl_abstract_type_t *theStruct){
	int i;
		printf("Printing Abstract Type\n-----------------\n");
	printf("\t Ints: %d \t Doubles: %d\t Chars: %d\n",theStruct->numInts, theStruct->numDoubles, theStruct->numChars);
	if(theStruct->numInts>0)assert(theStruct->intArray!=0);
	if(theStruct->numDoubles>0)assert(theStruct->doubleArray!=0);
	if(theStruct->numChars>0)assert(theStruct->charArray!=0);
	
	if(theStruct->numInts<100){
		printf("Ints: ");
		for(i=0;i<theStruct->numInts;i++)
			printf("\t%d",theStruct->intArray[i]);
	}
	printf("\n");
	if(theStruct->numDoubles<100){
		printf("Doubles: ");
		for(i=0;i<theStruct->numDoubles;i++)
			printf("\t%f",theStruct->doubleArray[i]);
	}
	printf("\n");
	if(theStruct->numChars<100){
		printf("Chars: ");
		for(i=0;i<theStruct->numChars;i++)
			printf("\t%c",theStruct->charArray[i]);
	}
	printf("\n");
}
