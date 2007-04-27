#include "Glue_utilities.h"

void parse_type(char*& ts, int& dim, char*& types, float*& mins, float*& maxs)
{
  int j,n,i;
  n = sscanf(ts," %d _ [%n",&dim,&j);
  if(n != 1)
  {
    printf("\nError6: Incorrect task spec format. Exiting....\n\n"); 
    exit(0);
  }
  ts += j;
  types  =new char[dim+1];
  mins = new float[dim];
  maxs = new float[dim];
  for (i = 0; i < dim-1; i++)
  {
    n = sscanf(ts," %c ,%n",&(types[i]),&j);
    if (n != 1)
    {
      printf("\nError7: Incorrect task spec format. Exiting....\n\n"); 
      exit(0);
    }
    ts += j;
  } 
  n = sscanf(ts," %c ] _ [%n",&(types[i++]),&j);
  if (n != 1)
  {
    printf("\nError8: Incorrect task spec format. Exiting....\n\n"); 
    exit(0);
  }
  ts += j;
  types[i] = '\0';
  
  for (i = 0; i < dim-1; i++)
  {
    n = sscanf(ts," %f , %f ] _ [%n",&(mins[i]),&(maxs[i]),&j);
    if (n != 2)
    {
      printf("\nError9: Incorrect task spec format. Exiting....\n\n"); 
      exit(0);
    }
    ts += j;
  }
  n = sscanf(ts," %f , %f ] :%n",&(mins[i]),&(maxs[i]),&j);
  if (n != 2)
  {
    printf("\nError: Incorrect task spec format. Exiting....\n\n"); 
    exit(0);
  }
  ts += j;
}

void parse_task_spec(char* ts, task_spec_struct* ps)
{
  int i,j;
  int n = sscanf(ts," %f : %c : %n",&(ps->version),&(ps->episodic),&j);
  if(n != 2)
  {
    printf("\nError1: Incorrect task spec format. Exiting....\n\n"); 
    exit(0);
  }
  ts += j;
  parse_type(ts,ps->obs_dim,ps->obs_types,ps->obs_mins,ps->obs_maxs);
  parse_type(ts,ps->action_dim,ps->action_types,ps->action_mins,ps->action_maxs);
  
  sscanf(ts," %f , %f ",&(ps->Rmin), &(ps->Rmax));
  if(n != 2)
  {
    printf("\nError2: Incorrect task spec format. Exiting....\n\n"); 
    exit(0);
  }
}
