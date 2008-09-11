#include "useful_functions.h"

#include <stdlib.h>

void makeKInts(rl_abstract_type_t *theStruct, int numInts){
	int i;
	
	theStruct->numInts=numInts;
	theStruct->intArray=(int *)calloc(numInts,sizeof(int));
	
	for(i=0;i<numInts;i++) theStruct->intArray[i]=i;
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
