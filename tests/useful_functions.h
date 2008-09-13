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

#ifndef USEFUL_FUNCTIONS_H
#define USEFUL_FUNCTIONS_H

#include <rlglue/RL_common.h>

void copy_structure_to_structure(rl_abstract_type_t *dst, rl_abstract_type_t *src);
void clean_abstract_type(rl_abstract_type_t *theStruct);
void makeKInts(rl_abstract_type_t *theStruct, int numInts);
void makeKDoubles(rl_abstract_type_t *theStruct, int numDoubles);
void makeKChars(rl_abstract_type_t *theStruct, int numChars);
#endif