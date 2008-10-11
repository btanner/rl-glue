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
*  $Revision: 231 $
*  $Date: 2008-09-28 17:06:16 -0600 (Sun, 28 Sep 2008) $
*  $Author: brian@tannerpages.com $
*  $HeadURL: https://rl-glue-ext.googlecode.com/svn/trunk/projects/codecs/C/src/rlglue/utils/C/RLStruct_util.h $
* 
*/
#ifndef RLStruct_Util_H
#define RLStruct_Util_H

#ifdef __cplusplus
extern "C" {
#endif

#include <rlglue/RL_common.h>


/**	
*	Brian Tanner, Sept 2008
*	Created some functions for people not super savvy with C that might make using this codec a little easier.
*/

/*	Copies all of the data from src to dst, freeing and allocating only if necessary*/
void replaceRLStruct(const rl_abstract_type_t *src, rl_abstract_type_t *dst);

/*	Frees the 3 arrays if they are not null, sets them to null, and sets numInts, numDoubles, numChars to 0*/
void clearRLStruct(rl_abstract_type_t *dst);

/*  calls clearRLStruct on dst, and then frees the pointers */
void freeRLStructPointer(rl_abstract_type_t *dst);

/*  Given a pointer to a rl_abstract_type_t, allocate arrays of the requested size and 
    set numInts, numDoubles, numChars in the struct appropriately. */
void allocateRLStruct(rl_abstract_type_t *dst, const unsigned int numInts, const unsigned int numDoubles, const unsigned int numChars);

/* Create a new rl_abstract_type_t, allocate its arrays and its numInts/Doubles/Chars using allocateRLStruct, return the pointer */
rl_abstract_type_t *allocateRLStructPointer(const unsigned int numInts, const unsigned int numDoubles, const unsigned int numChars);

/* Create a new rl_abstract_type_t pointer that is a copy of an existing one (src) */
rl_abstract_type_t *duplicateRLStructToPointer(const rl_abstract_type_t *src);

#ifdef __cplusplus
}
#endif
#endif
