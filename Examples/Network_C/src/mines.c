#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "mines.h"

/*a struct to house information about the agent's position in the map
*and where walls, mines, etc are located*/
typedef struct 
{
  int START;			/* Start marker in grid */
  int GOAL;			/* End marker in grid */
  int LAND;			/* Free space in grid */
  int OBSTACLE;	                /* Obstical in grid */
  int MINE;                     /* Mine in grid */
  int row;			/* Number of rows in grid */
  int col;			/* Number of columns in grid */
  int startRow;	         	/* Starting position */
  int startCol;   		/* Starting position */
  int agentRow;		        /* Agents current position */
  int agentColumn;	        /* Agents current position */
} mine_env;

Observation o;
mine_env M;
Reward_observation ro;
int env_terminal;

/*the current map the agent starts in. To read this: the world is a 6 by 18 grid
* in any position the number corresponds to one of the objects in the mine_env struct. 
*for example in env_init the start position is labelled by a 0 so we can see the initial start
* position in this particular map is at position [12][1] or if you're not starting at zero 
*(as non coders in the real world usually do) it's position 13,2. NOTE: This environment uses random
*starts. The start position can change with every call to env_start. */
int env_map[6][18] = 
{ 
  { 3, 3, 3, 3, 3, 3 ,3 ,3, 3, 3 ,3 ,3 ,3, 3, 3, 3, 3, 3 }, 
  { 3, 2, 2, 2, 2, 2, 2, 4, 4, 2, 2, 2, 0, 2, 2, 2, 2, 3 }, 
  { 3, 2, 2, 2, 2, 4, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3 },
  { 3, 2, 2, 2, 2, 2, 2, 2, 2, 4, 4, 4, 2, 2, 2, 2, 3, 3 },
  { 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 3 },
  { 3, 3, 3, 3, 3, 3 ,3 ,3, 3, 3 ,3 ,3 ,3, 3, 3, 3, 3, 3 }
};

/* RL-Glue Interface */

Task_specification env_init()
{    
/*initializing the map struct and an observation object*/
  static char Task_spec[100] = {0};
  char temp[50] = {0};

  M.START = 0;
  M.GOAL = 1;
  M.LAND = 2;
  M.OBSTACLE = 3;
  M.MINE = 4;
  
  M.row = 6;
  M.col = 18;
  M.startRow= 1;
  M.startCol = 12;
  M.agentRow = M.startRow;
  M.agentColumn = M.startCol;
  
  o.numInts    = 1;
  o.numDoubles = 0;

  o.intArray    = (int*)malloc(sizeof(int)*o.numInts);
  o.doubleArray = 0;
/*set random seed*/
	srand(0);

  /* Create and Return task specification */
  
  strcpy(Task_spec,"1:e:1_[i]_[0,");
  sprintf(temp,"%d", M.row*M.col-1);
  strcat(Task_spec,temp);
  sprintf(temp,"]:1_[i]_[0,");
  strcat(Task_spec,temp);
  sprintf(temp,"%d]",4-1);
  strcat(Task_spec,temp);
 
  return Task_spec;
}

Observation env_start()
{   
/*this implementation of mines has a random start. The start position CANNOT BE on 
*a mine or any other hazard as env_start cannot return a terminal Observation*/
  int r = 0, c = 0;

  env_terminal = 0;
  env_map[M.startRow][M.startCol] = M.LAND;
  
  while(env_map[r][c] != M.LAND)
  {
    r = rand() % M.row;
    c = rand() % M.col;
  }

  M.startRow = r;
  M.startCol = c;
  env_map[M.startRow][M.startCol] = M.START;    
  
  M.agentColumn =  M.startCol;
  M.agentRow = M.startRow;
  
  /*this version of mines represents the agent's current position in the state as the Observation.
  *it only uses one integer and numbers all the states by the unique result of:
  * agentrow*numberofcolumns + agentcolumn*/
  o.intArray[0] = M.startRow * M.col + M.startCol;

  return o;
}

Reward_observation env_step(Action a)
{  
	/*gets next observation and reward. Checks to see if the observation is a terminal one. 
	*all of this information is stuffed into a Reward_observation struct and returned*/
  
  getNextPosition(a); /* getNextPosition will update the values of agentRow and agentColumn */
 
  o.intArray[0] = getPosition();
  
  ro.o = o;
  ro.r = getReward();
  
  if(env_terminal) /* end of episode? */
    ro.terminal = 1;
  else
    ro.terminal = 0;
	
  return ro;
}

void env_cleanup()
{
/*deallocate all memory and initialize values to 0*/
  free(o.intArray);
  free(o.doubleArray);

  o.intArray    = 0;
  o.doubleArray = 0;
}

void env_set_state(State_key sk)
{
/* not implemented, one possibility is to have the state_key be the current map and the mine_env struct*/
}
     
void env_set_random_seed(Random_seed_key rsk)
{
/* not implemented, one possibility is to have the state_key be the current map and the mine_env struct*/
}

State_key env_get_state()
{
/*not implemented.*/
  State_key theKey;
  return theKey;
}

Random_seed_key env_get_random_seed()
{
/*not implemented*/
  Random_seed_key theKey;
  return theKey;
}

Message env_message(const Message inMessage) {
/*no messages currently implemented*/
  return "mines.c does not respond to any messages.";
}


/* mines utitily functions */


void env_print(const char* header, RL_abstract_type* data) {
/* for debugging purposes, printed out the values of the RL_abstract_type passed in: This can be
* an Observaiton, Action, Random_seed_key or State_key*/
  unsigned int i = 0;
  fprintf(stderr, "%s", header);
  fprintf(stderr, "%d %d\n", data->numInts, data->numDoubles);

  for (i = 0; i < data->numInts; ++i)
    fprintf(stderr, "%d ", data->intArray[i]);
  fprintf(stderr, "\n");

  for (i = 0; i < data->numDoubles; ++i)
    fprintf(stderr, "%f ", data->doubleArray[i]);
  fprintf(stderr, "\n");
}


int getPosition()
{
	/*returns the current position of the gaent. If the agent is in a terminal state, 
	* it sets env_terminal and returns with an invalid value(-1)*/
  if (env_map[M.agentRow][M.agentColumn] != M.GOAL && env_map[M.agentRow][M.agentColumn] != M.MINE)
  {    
    /* The episode terminates if the agent hits a mine */
    return M.agentRow*M.col + M.agentColumn;
  }
  else
  {
    env_terminal = 1;
    return -1;
  }
}

void getNextPosition(Action a)
{
  /* When the move would result in hitting an obstacles, the agent simply doesn't move */
  int newRow = M.agentRow;
  int newColumn = M.agentColumn;
  
  if (a.intArray[0] == 0) /*move down*/
    newColumn = M.agentColumn - 1;
  else if (a.intArray[0] == 1) /*move up*/
    newColumn = M.agentColumn + 1;
  else if (a.intArray[0] == 2)/*move left*/
    newRow = M.agentRow - 1;
  else if (a.intArray[0] == 3)/*move right*/
    newRow = M.agentRow + 1;
  
  /*if agent steps out of bounds, don't move*/
  if(newRow >= M.row || newRow < 0 || newColumn >= M.col || newColumn < 0)
  {
    M.agentColumn = M.agentColumn;
    M.agentRow = M.agentRow;
  }
  else if (env_map[newRow][newColumn] != M.OBSTACLE)
  {
  /*if Agent doesn't hit an obstacle or step out of bounds the new state is accepted*/
    M.agentRow = newRow;
    M.agentColumn = newColumn;
  }
}

Reward getReward()
{
/*get the reward for  the transition, the rewards are -1 on all states except the goal and a mine*/
  if (env_map[M.agentRow][M.agentColumn] == M.GOAL){
    return 10;}
  else if (env_map[M.agentRow][M.agentColumn] == M.MINE){
    return -10;}
  else
    return -1;
}
