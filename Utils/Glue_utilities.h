#include <stdlib.h>
#include <stdio.h>

#include "string.h"
#include "math.h"


#ifndef GLUE_UTIL_H
#define GLUE_UTIL_H

typedef struct 
{
        float version;			
        char episodic;			
        int obs_dim;			
        int num_discrete_obs_dims;
        int num_continuous_obs_dims;
        char *obs_types;	    
        float *obs_mins;           
        float *obs_maxs;			
        int action_dim;			
        int num_discrete_action_dims;
        int num_continuous_action_dims;
        int* discrete_action_dim_lookup; // The map from discrete ordinal to total ordinal
        int* continuous_action_dim_lookup; // The map from continuous ordinal to total ordinal
        char *action_types;		
        float *action_mins;		
        float *action_maxs;
} task_spec_struct;


void parse_task_spec(char* ts, task_spec_struct* ps);

#endif /*GLUE_UTIL_H_*/
