import rlglue.environment.Environment;
import rlglue.types.Action;
import rlglue.types.Observation;
import rlglue.types.Reward_observation;
import rlglue.types.State_key;
import rlglue.types.Random_seed_key;

import java.util.Random;

class MineEnv
{
    public int start;
    public int goal;
    public int land;
    public int obstacle;
    public int mine;
    public int row;
    public int col;
    public int startRow;
    public int startCol;
    public int agentRow;
    public int agentCol;
}

public class Mines implements Environment
{
    protected int envMap[][] = 
    { 
	{ 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3 }, 
	{ 3, 2, 2, 2, 2, 2, 2, 4, 4, 2, 2, 2, 0, 2, 2, 2, 2, 3 }, 
	{ 3, 2, 2, 2, 2, 4, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3 },
	{ 3, 2, 2, 2, 2, 2, 2, 2, 2, 4, 4, 4, 2, 2, 2, 2, 3, 3 },
	{ 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 3 },
	{ 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3 }
    };

    protected MineEnv env;
    protected Random rand;
    protected int isTerminal;

    public Mines()
    {
    }

    public String env_init()
    {
	String taskSpec = "1:e:1_[i]_[0,107]:1_[i]_[0,3]";

	rand = new Random();
	env = new MineEnv();
	env.start = 0;
	env.goal = 1;
	env.land = 2;
	env.obstacle = 3;
	env.mine = 4;
	
	env.row = 6;
	env.col = 18;
	env.startRow = 1;
	env.startCol = 12;
	env.agentRow = env.startRow;
	env.agentCol = env.startCol;

	return taskSpec;
    }

    public Observation env_start()
    {
	int r = 0;
	int c = 0;

	isTerminal = 0;
	envMap[env.startRow][env.startCol] = env.land;
	
	while(envMap[r][c] != env.land)
	{
	    r = rand.nextInt(env.row);
	    c = rand.nextInt(env.col);
	}
	
	env.startRow = r;
	env.startCol = c;
	envMap[env.startRow][env.startCol] = env.start;    
	
	env.agentCol = env.startCol;
	env.agentRow = env.startRow;

	Observation o = new Observation(1,0);
	o.intArray[0] = env.startRow * env.col + env.startCol;
	
	return o;
    }

    public Reward_observation env_step(Action action)
    {
	Reward_observation ro = new Reward_observation();
	Observation o = new Observation(1,0);

	this.getNextPosition(action); /* getNextPosition will update the values of agentRow and agentColumn */	
	o.intArray[0] = getPosition();
	
	ro.o = o;
	ro.r = getReward();
	ro.terminal = isTerminal;
	
	return ro;
    }

    public void env_cleanup()
    {
    }

    public String env_message(final String message)
    {
	return "Mines.java does not respond to any messages.";
    }

    public Random_seed_key env_get_random_seed()
    {
	return new Random_seed_key(1,0);
    }

    public State_key env_get_state()
    {
	return new State_key(1,0);
    }

    public void env_set_random_seed(Random_seed_key rsk)
    {
	for (int i = 0; i < rsk.intArray.length; ++i)
	    System.err.print("" + rsk.intArray[i] + " ");
	System.err.println("");

	for (int i = 0; i < rsk.doubleArray.length; ++i)
	    System.err.print("" + rsk.doubleArray[i] + " ");
	System.err.println("");
    }

    public void env_set_state(State_key rsk)
    {
    }

    public int getPosition()
    {
	/*returns the current position of the gaent. If the agent is in a terminal state, 
	 * it sets env_terminal and returns with an invalid value(-1)*/
	if (envMap[env.agentRow][env.agentCol] != env.goal && envMap[env.agentRow][env.agentCol] != env.mine)
	{    
	    /* The episode terminates if the agent hits a mine */
	    return env.agentRow*env.col + env.agentCol;
	}
	else
	{
	    isTerminal = 1;
	    return -1;
	}
    }

    public void getNextPosition(Action a)
    {
	/* When the move would result in hitting an obstacles, the agent simply doesn't move */
	int newRow = env.agentRow;
	int newColumn = env.agentCol;
	
	if (a.intArray[0] == 0) /*move down*/
	    newColumn = env.agentCol - 1;
	else if (a.intArray[0] == 1) /*move up*/
	    newColumn = env.agentCol + 1;
	else if (a.intArray[0] == 2)/*move left*/
	    newRow = env.agentRow - 1;
	else if (a.intArray[0] == 3)/*move right*/
	    newRow = env.agentRow + 1;
	
	/*if agent steps out of bounds, don't move*/
	if(newRow >= env.row || newRow < 0 || newColumn >= env.col || newColumn < 0)
	{
	     env.agentCol = env.agentCol;
	     env.agentRow = env.agentRow;
	}
	else if (envMap[newRow][newColumn] != env.obstacle)
	{
	    /*if Agent doesn't hit an obstacle or step out of bounds the new state is accepted*/
	    env.agentRow = newRow;
	    env.agentCol = newColumn;
	}
    }

    public double getReward()
    {
	double reward = 0;

	/*get the reward for  the transition, the rewards are -1 on all states except the goal and a mine*/
	if (envMap[env.agentRow][env.agentCol] == env.goal) {
	    reward = 10;
	}
	else if (envMap[env.agentRow][env.agentCol] == env.mine) {
	    reward = -10;
	}
	else {
	    reward = -1;
	}
	return reward;
    }
}
