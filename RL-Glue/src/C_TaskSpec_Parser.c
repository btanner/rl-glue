#include "C_TaskSpec_Parser.h"

/*int main(void) {
	task_spec_struct tss;
	int i;
	double state_mins[] = {-5.0, 10.0,  3.0, -20.0, -10.0, -1.0, 100.0};
  double state_maxs[] = {-3.0, 100.0, 7.0,   0.0,  -8.0,  3.0, 110.0};
  char action_types[] = {'i','f','f','i'};
  double action_mins[] = {-5.0, -1.0, -20.0, -30.0};
  double action_maxs[] = { 5.0,  1.0,   0.0, -28.0};
	parse_task_spec("2.0:e:7_[f,i,f,f,i,f,i]_[-5.0,-3.0]_[10.0,100.0]_[3.0,7.0]_[-20.0,0.0]_[-10.0,-8.0]_[-1.0,3.0]_[100.0,110.0]:4_[i,f,f,i]_[-5.0,5.0]_[-1.0,1.0]_[-20.0,0.0]_[-30.0,-28.0]:[]", &tss);
	printf("numStateInts = %d\n", tss.num_discrete_obs_dims);
	printf("numStateDoubles = %d\n", tss.num_continuous_obs_dims);
	for (i = 0; i < tss.obs_dim; i++)
	{
		printf("[%lf,%lf]\n",tss.obs_mins[i],tss.obs_maxs[i]);
	}
	printf("numActionInts = %d\n", tss.num_discrete_action_dims);
	printf("numActionDoubles = %d\n", tss.num_continuous_action_dims);
	for (i = 0; i < tss.action_dim; i++)
	{
		printf("[%lf,%lf]\n",tss.action_mins[i],tss.action_maxs[i]);
	}
	printf("Reward: [%lf,%lf]\n",tss.reward_min,tss.reward_max);
	
	parse_task_spec("2:e:7_[f,i,f,f,i,f,i,]_[-inf,-3]_[10,inf]_[3,7]_[-inf,inf]_[-10,-8]_[-1,3]_[100,110]:4_[i,f,f,i]_[-5,5]_[-1,1]_[-20,0]_[-30,-28]", &tss);
	printf("numStateInts = %d\n", tss.num_discrete_obs_dims);
	printf("numStateDoubles = %d\n", tss.num_continuous_obs_dims);
	for (i = 0; i < tss.obs_dim; i++)
	{
		printf("[%lf,%lf]\n",tss.obs_mins[i],tss.obs_maxs[i]);
	}
	printf("numActionInts = %d\n", tss.num_discrete_action_dims);
	printf("numActionDoubles = %d\n", tss.num_continuous_action_dims);
	for (i = 0; i < tss.action_dim; i++)
	{
		printf("[%lf,%lf]\n",tss.action_mins[i],tss.action_maxs[i]);
	}
	printf("Reward: [%lf,%lf]\n",tss.reward_min,tss.reward_max);
	return 0;
}*/

void parse_range(const char** ts, double* min, double* max)
/* parses a single range in the taskspec, ie. '[min,max]'. ts must start at the '['. Advances taskspec to end of ']' */
{
	int characters_read;
	int scan_args;
	/*obtain minimum value*/
	sscanf(*ts, " [ %n",&characters_read);
	*ts = *ts + characters_read;
	scan_args = sscanf(*ts, " %lf , %n", min, &characters_read);
	if(scan_args == 1)
	{
		/*if value is read correctly*/
		*ts = *ts + characters_read;
	}
	else if(scan_args < 1)
	{
		scan_args = sscanf(*ts, " -inf , %n", &characters_read);
		if (scan_args == 1)
		{
			*min = (double)-INFINITY;
			*ts = *ts + characters_read;
		}
		else if (scan_args == 0)
		{
			/*if no value is read (ie we are using negative inf as the min)*/
			*min = GLUE_NAN;
			sscanf(*ts, " , %n", &characters_read);
			*ts = *ts + characters_read;
		}
	}
	else{
		printf("\n Error on trying to read the minimum value of a range in the taskspec. Exiting .... \n\n" ); 
		exit(0);
	}
	/*obtain maximum value*/
	scan_args = sscanf(*ts," %lf ] %n",max,&characters_read);
	if(scan_args == 1)
	{
		/*if value is correctly read*/
		  *ts = *ts + characters_read;
	}
	else if(scan_args < 1){
		scan_args = sscanf(*ts, " inf ] %n", &characters_read);
		if (scan_args == 1)
		{
			*max = (double)INFINITY;
			*ts = *ts + characters_read;
		}
		else if (scan_args < 1)
		{
			/*if no value is read (ie we are using positive inf as the max)*/
			*max = GLUE_NAN;
			sscanf(*ts, " ] %n", &characters_read);
			*ts = *ts + characters_read;
		}
	}
	else{
		printf("\n Error on trying to read the maximum value a range in the taskspec. Exiting .... \n\n");
		exit(0);
	}
	/* If both min and max are given, make sure range is valid: ie. min < max */
	if (!isnan(*min) && !isnan(*max) && *min >= *max)
	{
		if (*min > *max)
		{
			printf("\nError: min(%f) is greater than max(%f) in taskspec range. Exiting...\n\n",(float)*min,(float)*max);
			exit(0);
		}
		else /* if they are equal just throw a warning [ML] */
		{
			printf("\nWarning: min(%f) is equal to max(%f) in taskspec range.\n",(float)*min,(float)*max);
		}
	}
}

/*NOTE for users unfamiliar with sscanf: %n in sscanf will return the number of characters read in that sscanf call. */
/*parses the observation or action portion of a taskspec (since they both have the same format)*/ 
void parse_type(const char** ts, int* dim, char** types, double** mins, double** maxs, int* num_discrete_dims, int* num_continuous_dims)
{
  int characters_read,scan_args,i;
  *num_discrete_dims = 0; 
  *num_continuous_dims =0;

  scan_args = sscanf(*ts," : %d _ [%n",dim,&characters_read); /* get the numer of dimensions to read*/
 
   if(scan_args != 1) /*If it fails to read the first arguement, exit*/
  {
    printf("\nError6: Incorrect task spec format. Cannot read number of dimensions for observation or action Exiting....\n\n"); 
    exit(0);
  }
  *ts = *ts + characters_read; /* move along the string*/
  
  /*allocate the types, mins and maxs arrays*/
  *types = (char*)malloc(sizeof(char)*((*dim)+1)); 
  *mins = (double*)malloc(sizeof(double)*(*dim));
  *maxs = (double*)malloc(sizeof(double)*(*dim));

  
  /*get all the types for all the variables.*/
  for (i = 0; i < (*dim)-1; i++)
  {
    scan_args = sscanf(*ts," %c ,%n",(&((*types)[i])),&characters_read); /* scans for the i'th variable type*/
	
    if (scan_args != 1) /*fails to read in arguements*/
    {
      printf("\nError7: Incorrect task spec format. Cannot read in variable type for the %d 'th observation or action Exiting....\n\n", i); 
      exit(0);
    }
	/*counts how many discrete and how many continuous variables*/
	if(strncmp(&((*types)[i]), "i", 1) == 0) /*if it's an int*/
		(*num_discrete_dims) = (*num_discrete_dims) + 1;
	else if(strncmp(&((*types)[i]), "f", 1) == 0) /* if it's a float*/
		(*num_continuous_dims) = (*num_continuous_dims) + 1;
	else{
		printf("\nError: Variable type not of type int or float. GRRR!! Variable of type %c Exiting... \n\n", (*types)[i]);
		exit(0);
	}		
  *ts = *ts + characters_read; /*move along the string*/
  } 

  /*get the last type for the last variable*/
  scan_args = sscanf(*ts," %c %n",(&((*types)[i])),&characters_read);
  if (scan_args != 1)
  {
    printf("\nError8: Incorrect task spec format. cannot read last variable type of observation or action Exiting....\n\n"); 
    exit(0);
  }
	if(strncmp(&((*types)[i]), "i", 1) == 0)
		(*num_discrete_dims) = (*num_discrete_dims) + 1;
	else if(strncmp(&((*types)[i]), "f", 1) == 0)
		(*num_continuous_dims) = (*num_continuous_dims) + 1;
	else{
		printf("\nError: Variable type not of type int or float. GRRR!! Variable of type %c Exiting... \n\n", (*types)[i]);
		exit(0);
	}	
  *ts = *ts + characters_read;
  (*types)[++i] = '\0';
  
	char c;
	scan_args = sscanf(*ts," %c %n",&c,&characters_read);
	while (c != ']' && scan_args > 0)
	{
		*ts = *ts + characters_read;
		scan_args = sscanf(*ts," %c %n",&c,&characters_read);
	}
	*ts = *ts + characters_read;
  
	/*Get the min and max values for all the variables*/  
  for (i = 0; i < (*dim); i++)
  {
		sscanf(*ts, " _ %n", &characters_read);
		*ts = *ts + characters_read;
		parse_range(ts,(&((*mins)[i])),(&((*maxs)[i])));
  }/*End of for loop to get values*/

}

void parse_reward(const char** ts, double* min, double* max)
/*parses the reward section of the taskspec, ie. :[min,max] */
{
	/*getting the last min max value pair*/
	/*obtain minimum value*/
	int characters_read;
	sscanf(*ts, " : %n", &characters_read);
	*ts = *ts + characters_read;
	parse_range(ts,min,max);
}

void parse_task_spec(const char* ts, task_spec_struct* ps)
{
  int characters_read;
  int scan_args = sscanf(ts," %f : %c %n ",&(ps->version),&(ps->episodic),&characters_read); /* read in version, episodic or continuous, and the number of characters consumed in this call to scanf*/
  if(scan_args != 2) 
  {
    printf("\nError1: Incorrect task spec format. Error near version number, or episodic/continuing task. Exiting....\n\n"); 
    exit(0);
  }
	if (ps->episodic != 'c' && ps->episodic != 'e')
	{
		printf("\nError: Task spec episodic field is not 'e' or 'c'. Exiting...\n\n");
		exit(0);
	}
  ts += characters_read; /* increase the pointer to where observations lie. Now the string looks like obsdim_[obtypes]_[obvalues]:actdim_[acttypes]_[actvalues]*/
  parse_type(&ts,&(ps->obs_dim),&(ps->obs_types),&(ps->obs_mins),&(ps->obs_maxs),&(ps->num_discrete_obs_dims), &(ps->num_continuous_obs_dims)); /*parse for observation data*/
  parse_type(&ts,&(ps->action_dim),&(ps->action_types),&(ps->action_mins), &(ps->action_maxs), &(ps->num_discrete_action_dims), &(ps->num_continuous_action_dims)); /*parse for action data*/
  if (ps->version > 1)
	parse_reward(&ts,&(ps->reward_min),&(ps->reward_max));
}

/*check if obs_min[index] is negative GLUE_INFINITY*/
int isObsMinNegINFINITY(int index, task_spec_struct* ps)
{
	return isinf(ps->obs_mins[index]);
}
/*check if action_min[index] is negative GLUE_INFINITY*/
int isActionMinNegINFINITY(int index, task_spec_struct* ps)
{
	return isinf(ps->action_mins[index]);
}
/*check if obs_max[index] is positive GLUE_INFINITY*/
int isObsMaxPosINFINITY(int index, task_spec_struct* ps)
{
	return isinf(ps->obs_maxs[index]);
}
/*check if action_max[index] is positive GLUE_INFINITY*/
int isActionMaxPosINFINITY(int index, task_spec_struct* ps)
{
	return isinf(ps->action_maxs[index]);
}
/*check if the value range for observation[index] is known*/
int isObsUnknown(int index, task_spec_struct* ps)
{
	return isnan(ps->obs_mins[index]);
}
/*check if the value range for action[index] is known*/
int isActionUnknown(int index, task_spec_struct* ps)
{
	return isnan(ps->action_mins[index]);
}
int isRewardMinNegINFINITY(task_spec_struct* ps)
{
	return isinf(ps->reward_min);
}
int isRewardMaxPosINFINITY(task_spec_struct* ps)
{
	return isinf(ps->reward_max);
}
int isRewardMinUnknown(task_spec_struct* ps)
{
	return isnan(ps->reward_min);
}
int isRewardMaxUnknown(task_spec_struct* ps)
{
	return isnan(ps->reward_max);
}
