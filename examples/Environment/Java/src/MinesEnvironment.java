/* 
* Copyright (C) 2007, Brian Tanner
* 
http://rl-glue.googlecode.com/

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 */

package MinesEnvironment;

import java.util.Random;

import rlglue.utilities.*;

import rlglue.environment.Environment;
import rlglue.types.Action;
import rlglue.types.Observation;
import rlglue.types.Random_seed_key;
import rlglue.types.Reward_observation;
import rlglue.types.State_key;

public class MinesEnvironment implements Environment {
	private Observation observation;
	private int numInts =1;
	private int numDoubles =0;
	private Random random = new Random();
	private boolean env_terminal = false;

	private int START=0;
	private int GOAL=1;
	private int LAND=2;
	private int OBSTACLE=3;
	private int MINE=4;
	private int row=6;
	private int col=18;
	private int startRow=1;
	private int startCol=12;
	private int agentRow = startRow;
	private int agentCol = startCol;
	private int[][] env_map = 
	{ 
	  { 3, 3, 3, 3, 3, 3 ,3 ,3, 3, 3 ,3 ,3 ,3, 3, 3, 3, 3, 3 }, 
	  { 3, 2, 2, 2, 2, 2, 2, 4, 4, 2, 2, 2, 0, 2, 2, 2, 2, 3 }, 
	  { 3, 2, 2, 2, 2, 4, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3 },
	  { 3, 2, 2, 2, 2, 2, 2, 2, 2, 4, 4, 4, 2, 2, 2, 2, 3, 3 },
	  { 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 3 },
	  { 3, 3, 3, 3, 3, 3 ,3 ,3, 3, 3 ,3 ,3 ,3, 3, 3, 3, 3, 3 }
	};
	
	public MinesEnvironment() {}
	
	public String env_init()
	{
		observation = new Observation(numInts, numDoubles);


		return "2:e:1_[i]_[0,107]:1_[i]_[0,3]:[-10,10]";
	}

	public Observation env_start()
	{   
	  int r = 0, c = 0;

	  env_terminal = false;
	  env_map[startRow][startCol] = LAND;

	  while(env_map[r][c] != LAND)
	  {
	    r = random.nextInt(row);
	    c = random.nextInt(col);
	  }

	  startRow = r;
	  startCol = c;
	  env_map[startRow][startCol] = START;    

	  agentCol = startCol;
	  agentRow = startRow;

	  observation.intArray[0] = startRow * col + startCol;

	  return observation;
	}

	public Reward_observation env_step(Action a)
	{    
		int terminal;
		getNextPosition(a); /* getNextPosition will update the values of agentRow and agentColumn */

		observation.intArray[0] = getPosition();

		if(env_terminal) /* end of episode? */
			terminal = 1;
		else
			terminal = 0;

		return new Reward_observation(getReward(),observation,terminal);
	}

	public void env_cleanup() {}

	public void env_set_state(State_key sk) {}

	public void env_set_random_seed(Random_seed_key rsk) {}

	public State_key env_get_state()
	{
		State_key theKey = new State_key(numInts,numDoubles);
		return theKey;
	}

	public Random_seed_key env_get_random_seed()
	{
		Random_seed_key theKey = new Random_seed_key(numInts,numDoubles);
		return theKey;
	}

	public String env_message(final String message) {
		return null;
	}

	private int getPosition()
	{
		if (env_map[agentRow][agentCol] != GOAL && env_map[agentRow][agentCol] != MINE)
		{    
			/* The episode terminates if the agent hits a mine */
			return agentRow*col + agentCol;
		}
		else
		{
			env_terminal = true;
			return -1;
		}
	}

	private void getNextPosition(Action a)
	{
		/* When the move would result in hitting an obstacles, the agent simply doesn't move */
		int newRow = agentRow;
		int newColumn = agentCol;

		if (a.intArray[0] == 0)
			newColumn = agentCol - 1;
		else if (a.intArray[0] == 1)
			newColumn = agentCol + 1;
		else if (a.intArray[0] == 2)
			newRow = agentRow - 1;
		else if (a.intArray[0] == 3)
			newRow = agentRow + 1;

		if(newRow >= row || newRow < 0 || newColumn >= col || newColumn < 0)
		{}
		else if (env_map[newRow][newColumn] != OBSTACLE)
		{
			agentRow = newRow;
			agentCol = newColumn;
		}
	}

	private double getReward()
	{
		if (env_map[agentRow][agentCol] == GOAL)
			return 10;
		else if (env_map[agentRow][agentCol] == MINE)
			return -10;
		else
			return -1;
	}

}