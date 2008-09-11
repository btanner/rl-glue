#ifndef USEFUL_FUNCTIONS_H
#define USEFUL_FUNCTIONS_H

#include <rlglue/RL_common.h>

void copy_structure_to_structure(rl_abstract_type_t *dst, rl_abstract_type_t *src);
void clean_abstract_type(rl_abstract_type_t *theStruct);
void makeKInts(rl_abstract_type_t *theStruct, int numInts);
#endif