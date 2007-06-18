#include "Glue_utilities.h"


void parse_type(char** ts, int* dim, char** types, float** mins, float** maxs, int* num_discrete_dims, int* num_continuous_dims)
{
  int characters_read,scan_args,i;
  *num_discrete_dims = 0;
  *num_continuous_dims =0;
  scan_args = sscanf(*ts," %d _ [%n",dim,&characters_read); /* get the numer of dimensions to read*/
  if(scan_args != 1)
  {
    printf("\nError6: Incorrect task spec format. Cannot read number of dimensions for observation or action Exiting....\n\n"); 
    exit(0);
  }
  *ts = *ts + characters_read;
/*  types  =new char[dim+1];
  mins = new float[dim];
  maxs = new float[dim];*/
  *types = (char*)malloc(sizeof(char)*((*dim)+1));
  *mins = (float*)malloc(sizeof(float)*(*dim));
  *maxs = (float*)malloc(sizeof(float)*(*dim));

  
  /*get all the types for all the variables.*/
  for (i = 0; i < (*dim)-1; i++)
  {
    scan_args = sscanf(*ts," %c ,%n",(&((*types)[i])),&characters_read);
    if (scan_args != 1)
    {
      printf("\nError7: Incorrect task spec format. Cannot read in variable type for the %d 'th observation or action Exiting....\n\n", i); 
      exit(0);
    }
	/*counts how many discrete and how many continuous variables*/
	if(strncmp(&((*types)[i]), "i", 1) == 0)
		(*num_discrete_dims) = (*num_discrete_dims) + 1;
	else if(strncmp(&((*types)[i]), "f", 1) == 0)
		(*num_continuous_dims) = (*num_continuous_dims) + 1;
	else{
		printf("\nError: Variable type not of type int or float. GRRR!! Variable of type %s Exiting... \n\n", (*types)[i]);
		exit(0);
	}		
  *ts = *ts + characters_read;
  } 

  /*get the last type for the last variable*/
  scan_args = sscanf(*ts," %c ] _ [%n",(&((*types)[i])),&characters_read);
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
	printf("\nError: Variable type not of type int or float. GRRR!! Variable of type %s Exiting... \n\n", (*types)[i]);
	exit(0);
}	
  *ts = *ts + characters_read;
  (*types)[++i] = '\0';
  
  
/*Get the min and max values for all the variables*/  
  for (i = 0; i < (*dim)-1; i++)
  {
	//obtain minimum value
	scan_args = sscanf(*ts, " %f , %n", (&((*mins)[i])), &characters_read);
	if(scan_args == 1)
	{
		//if value is read correctly
		*ts = *ts + characters_read;
	}
	else if(scan_args ==0)
	{
		//if no value is read (ie we are using negative inf as the min)
		((*mins)[i]) = -1*INFINITY;
		sscanf(*ts, " , %n", &characters_read);
		*ts = *ts + characters_read;
	}
	else{
		printf("\n Error on trying to read the minimum value of the %d th observation or action Exiting .... \n\n", i ); 
		exit(0);
	}
	//obtain maximum value
    scan_args = sscanf(*ts," %f ] _ [%n",(&((*maxs)[i])),&characters_read);
	if(scan_args == 1)
	{
		//if value is correctly read
		  *ts = *ts + characters_read;
	}
	else if(scan_args ==0){
		//if no value is read (ie we are using positive inf as the max)
		((*maxs)[i]) = INFINITY;
		sscanf(*ts, " ] _ [%n", &characters_read);
		*ts = *ts + characters_read;
	}
	else{
		printf("\n Error on trying to read the maximum value of the %d th observation or action Exiting .... \n\n", i);
		exit(0);
	}
  }//End of for loop to get values

  //getting the last min max value pair
	//obtain minimum value
	scan_args = sscanf(*ts, " %f , %n",  (&((*mins)[i])), &characters_read);
	if(scan_args == 1)
	{
		//if value is read correctly
		*ts = *ts + characters_read;
	}
	else if(scan_args ==0)
	{
		//if no value is read (ie we are using negative inf as the min)
		((*mins)[i]) = -1*INFINITY;
		sscanf(*ts, " , %n", &characters_read);
		*ts = *ts + characters_read;
	}
	else{
		printf("\n Error on trying to read the minimum value of the %d th observation or action Exiting .... \n\n", i ); 
		exit(0);
	}
	//obtain maximum value
    scan_args = sscanf(*ts," %f ] :%n",&((*maxs)[i]),&characters_read);
	if(scan_args == 1)
	{
		//if value is correctly read
		  *ts = *ts + characters_read;
	}
	else if(scan_args ==0){
		//if no value is read (ie we are using positive inf as the max)
		(*maxs)[i] = INFINITY;
		sscanf(*ts, " ] :%n", &characters_read);
		  *ts = *ts + characters_read;
	}
	else{
		printf("\n Error on trying to read the maximum value of the %d th observation or action Exiting .... \n\n", i);
		exit(0);
	}
}

void parse_task_spec(char* ts, task_spec_struct* ps)
{
  int i,characters_read;
  int scan_args = sscanf(ts," %f : %c : %n ",&(ps->version),&(ps->episodic),&characters_read); /* read in version, episodic or continuous, and the number of characters consumed in this call to scanf*/
  if(scan_args != 2) 
  {
    printf("\nError1: Incorrect task spec format. Error near version number, or episodic/continuing task Exiting....\n\n"); 
    exit(0);
  }
  ts += characters_read; /* increase the pointer to where observations lie. Now the string looks like obsdim_[obtypes]_[obvalues]:actdim_[acttypes]_[actvalues]*/
  parse_type(&ts,&(ps->obs_dim),&(ps->obs_types),&(ps->obs_mins),&(ps->obs_maxs),&(ps->num_discrete_obs_dims), &(ps->num_continuous_obs_dims)); /*parse for observation data*/
  parse_type(&ts,&(ps->action_dim),&(ps->action_types),&(ps->action_mins), &(ps->action_maxs), &(ps->num_discrete_action_dims), &(ps->num_continuous_action_dims)); /*parse for action data*/
  
}
