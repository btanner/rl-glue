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
*  $Revision: 808 $
*  $Date: 2008-09-11 19:23:09 -0600 (Thu, 11 Sep 2008) $
*  $Author: brian@tannerpages.com $
*  $HeadURL: https://rl-glue.googlecode.com/svn/trunk/tests/useful_functions.c $
* 
*/

#include "useful_functions.h"

#include <stdlib.h>

void makeKInts(rl_abstract_type_t *theStruct, int numInts){
	int i;
	
	theStruct->numInts=numInts;
	theStruct->intArray=(int *)calloc(numInts,sizeof(int));
	for(i=0;i<numInts;i++) theStruct->intArray[i]=i;
}

void makeKDoubles(rl_abstract_type_t *theStruct, int numDoubles){
	int i;
	
	theStruct->numDoubles=numDoubles;
	theStruct->doubleArray=(double *)calloc(numDoubles,sizeof(double));
	
	for(i=0;i<numDoubles;i++) theStruct->doubleArray[i]=(double)i/(double)3;	
}
void makeKChars(rl_abstract_type_t *theStruct, int numChars){
	int i;
	
	theStruct->numChars=numChars;
	theStruct->charArray=(char *)calloc(numChars,sizeof(char));
	
	for(i=0;i<numChars;i++) theStruct->charArray[i]='a';
}




void copy_structure_to_structure(rl_abstract_type_t *dst, rl_abstract_type_t *src){
	int i;
	clean_abstract_type(dst);
	/* Now the counts and arrays for ints, doubles, and chars are all 0 */
	if(dst->numInts!=src->numInts){
		dst->numInts=src->numInts;
		dst->intArray=(int *)calloc(dst->numInts, sizeof(int));
	}
	for(i=0;i<dst->numDoubles;i++) dst->doubleArray[i]=src->doubleArray[i];
	if(dst->numDoubles!=src->numDoubles){
		dst->numDoubles=src->numDoubles;
		dst->doubleArray=(double *)calloc(dst->numDoubles, sizeof(double));
	}
	for(i=0;i<dst->numDoubles;i++) dst->doubleArray[i]=src->doubleArray[i];
	
	for(i=0;i<dst->numChars;i++) dst->charArray[i]=src->charArray[i];
	if(dst->numChars!=src->numChars){
		dst->numChars=src->numChars;
		dst->charArray=(char *)calloc(dst->numChars, sizeof(char));
	}
	for(i=0;i<dst->numChars;i++) dst->charArray[i]=src->charArray[i];
}

void clean_abstract_type(rl_abstract_type_t *the_struct){
	the_struct->numInts=0;
	the_struct->numDoubles=0;
	the_struct->numChars=0;
	if(the_struct->intArray!=0){
		free(the_struct->intArray);
		the_struct->intArray=0;
	}
	if(the_struct->doubleArray!=0){
		free(the_struct->doubleArray);
		the_struct->doubleArray=0;
	}
	if(the_struct->charArray!=0){
		free(the_struct->charArray);
		the_struct->charArray=0;
	}	
}
