#include <stdlib.h>
#include <stdio.h>

#include <string.h>
#include <math.h>
#include <float.h>


#ifndef C_TASKSPEC_PARSER_H
#define C_TASKSPEC_PARSER_H

#ifndef INFINITY
#define INFINITY (1.0f/0.0f)
#endif
#define GLUE_NAN (sqrt(-1.0))

typedef struct 
{
        float version;			
        char episodic;			
        int obs_dim;			
        int num_discrete_obs_dims;
        int num_continuous_obs_dims;
        char *obs_types;	    
        double *obs_mins;           
        double *obs_maxs;			
        int action_dim;			
        int num_discrete_action_dims;
        int num_continuous_action_dims;
        char *action_types;		
        double *action_mins;		
        double *action_maxs;
		double reward_min;
		double reward_max;
} task_spec_struct;


void parse_task_spec(const char* ts, task_spec_struct* ps);

#endif /*C_TASKSPEC_PARSER_H*/
