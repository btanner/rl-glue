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
*  $Revision: 211 $
*  $Date: 2008-09-28 03:21:30 -0600 (Sun, 28 Sep 2008) $
*  $Author: brian@tannerpages.com $
*  $HeadURL: https://rl-glue-ext.googlecode.com/svn/trunk/projects/codecs/C/src/RLStruct_util.c $
* 
*/
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <assert.h>
#include <stdlib.h> /* free, calloc */
#include <string.h> /* memcpy */
#include <rlglue/utils/C/RLStruct_util.h>

/**	
*	Sept 8 2008, Brian Tanner is creating replace function
*	This one takes a src and dst, and puts all the data from the src into the dst
*	Freeing and reallocating if necessary
**/
void replaceRLStruct(const rl_abstract_type_t *src, rl_abstract_type_t *dst){
	assert(src!=0);
	assert(dst!=0);
	
	if(dst->numInts!=src->numInts){
		if(dst->numInts>0 && dst->intArray!=0){
			free(dst->intArray);
		}
		dst->numInts=src->numInts;
		dst->intArray=(int *)calloc(dst->numInts,sizeof(int));
		assert(dst->intArray!=0);
	}
	if(src->numInts>0){
		memcpy(dst->intArray, src->intArray,dst->numInts*sizeof(int));
	}

	if(dst->numDoubles!=src->numDoubles){
		if(dst->numDoubles>0 && dst->doubleArray!=0){
			free(dst->doubleArray);
		}
		dst->numDoubles=src->numDoubles;
		dst->doubleArray=(double *)calloc(dst->numDoubles,sizeof(double));
	}
	if(src->numDoubles>0){
		memcpy(dst->doubleArray, src->doubleArray,dst->numDoubles*sizeof(double));
	}

	if(dst->numChars!=src->numChars){
		if(dst->numChars>0 && dst->charArray!=0){
			free(dst->charArray);
		}
		dst->numChars=src->numChars;
		dst->charArray=(char *)calloc(dst->numChars,sizeof(char));
	}
	if(src->numChars>0){
		memcpy(dst->charArray, src->charArray,dst->numChars*sizeof(char));
	}	
}

/**
Created by Brian Tanner on Sept 27, 2008.
I thought this might be handy for people
*/
void clearRLStruct(rl_abstract_type_t *dst){
	if(dst==0)return;
	if(dst->intArray!=0){
		free(dst->intArray);
	}
	dst->intArray=0;

	if(dst->doubleArray!=0){
		free(dst->doubleArray);
	}
	dst->doubleArray=0;
	if(dst->charArray!=0){
		free(dst->charArray);
	}
	dst->charArray=0;

	dst->numInts=0;
	dst->numDoubles=0;
	dst->numChars=0;
	
}

void freeRLStructPointer(rl_abstract_type_t *dst){
	if(dst!=0){
		clearRLStruct(dst);
		free(dst);
	}
}

void allocateRLStruct(rl_abstract_type_t *dst, const unsigned int numInts, const unsigned int numDoubles, const unsigned int numChars){
	assert(dst!=0);
	clearRLStruct(dst);
	dst->numInts=numInts;
	dst->numDoubles=numDoubles;
	dst->numChars=numChars;
	
	if(dst->numInts!=0)
		dst->intArray=(int *)calloc(dst->numInts,sizeof(int));

	if(dst->numDoubles!=0)
		dst->doubleArray=(double *)calloc(dst->numDoubles,sizeof(double));

	if(dst->numChars!=0)
		dst->charArray=(char *)calloc(dst->numChars,sizeof(char));
}

rl_abstract_type_t *allocateRLStructPointer(const unsigned int numInts, const unsigned int numDoubles, const unsigned int numChars){
	rl_abstract_type_t *dst=(rl_abstract_type_t *)calloc(1,sizeof(rl_abstract_type_t));
	allocateRLStruct(dst,numInts,numDoubles,numChars);
	return dst;
}

rl_abstract_type_t *duplicateRLStructToPointer(const rl_abstract_type_t *src){
	assert(src!=0);
	rl_abstract_type_t *dst=(rl_abstract_type_t *)calloc(1,sizeof(rl_abstract_type_t));
	replaceRLStruct(src,dst);
	return dst;
}