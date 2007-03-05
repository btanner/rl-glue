#include "mines.h"

Task_specification env_init()
{    

    M.START = 0;
    M.GOAL = 1;
    M.LAND = 2;
    M.OBSTACLE = 3;
    M.MINE = 4;
	
    M.row = 5;
    M.col = 18;
    M.startRow= 1;
    M.startCol = 12;
    M.agentRow = M.startRow;
    M.agentColumn = M.startCol;
    
    // Return task specification:
    char* Task_spec = new char[100];
    char temp[50];
    
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
    env_terminal = false;
    int r = 0, c = 0;
    env_map[M.startRow][M.startCol] = M.LAND;
    
    while(env_map[r][c] != M.LAND)
    {
        r =  rand()% M.row;
        c = rand()% M.col;
    }
    M.startRow = r;
    M.startCol = c;
    env_map[M.startRow][M.startCol] = M.START;    
    
    M.agentColumn =  M.startCol;
    M.agentRow = M.startRow;
	
    return M.startRow * M.col + M.startCol;
}

Reward_observation env_step(Action a)
{    

    getNextPosition(a); //getNextPosition will update the values of agentRow and agentColumn
    
    ro.o	= getPosition(); 
    ro.r = getReward();
	
    if(env_terminal) //end of episode?
        ro.terminal = true;
    else
        ro.terminal = false;

    return ro;
}

int getPosition()
{
    if (env_map[M.agentRow][M.agentColumn] != M.GOAL && env_map[M.agentRow][M.agentColumn] != M.MINE)
    {    // The episode terminates if the agent hits a mine
        return M.agentRow*M.col + M.agentColumn;
    }
    else
    {	env_terminal = true;
        return -1;
    }
}

void getNextPosition(int a)
{
    // When the move would result in hitting an obstacles, the agent simply doesn't move
    int newRow = M.agentRow;
    int newColumn = M.agentColumn;
    
    if (a == 0)
        newColumn = M.agentColumn - 1;
    else if (a == 1)
        newColumn = M.agentColumn + 1;
    else if (a == 2)
        newRow = M.agentRow - 1;
    else if (a == 3)
        newRow = M.agentRow + 1;
    
    if(newRow >= M.row || newRow < 0 || newColumn >= M.col || newColumn < 0)
    {
        M.agentColumn = M.agentColumn;
        M.agentRow = M.agentRow;
    }
    else if (env_map[newRow][newColumn] != M.OBSTACLE)
    {
        M.agentRow = newRow;
        M.agentColumn = newColumn;
    }
}

Reward getReward()
{
    if (env_map[M.agentRow][M.agentColumn] == M.GOAL){
        return 10;}
    else if (env_map[M.agentRow][M.agentColumn] == M.MINE){
        return -10;}
    else
        return -1;
}

