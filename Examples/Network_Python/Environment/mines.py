import random
import sys
from RL_common import *

class MineEnv:
	START = 0					# Start marker in grid
	GOAL = 0					# End marker in grid
	LAND = 0					# Free space in grid
	OBSTACLE = 0			# Obstical in grid
	MINE = 0					# Mine in grid
	row = 0						# Number of rows in grid
	col = 0						# Number of columns in grid
	startRow = 0			# Starting position
	startCol = 0			# Starting position
	agentRow = 0			# Agent's current position
	agentColumn = 0		# Agent's current position

mine_observation = Observation()
M = MineEnv()
mine_ro = Reward_observation()
mine_terminal = 0

env_map = [ [ 3, 3, 3, 3, 3, 3 ,3 ,3, 3, 3 ,3 ,3 ,3, 3, 3, 3, 3, 3 ], 
						[ 3, 2, 2, 2, 2, 2, 2, 4, 4, 2, 2, 2, 0, 2, 2, 2, 2, 3 ], 
						[ 3, 2, 2, 2, 2, 4, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3 ],
						[ 3, 2, 2, 2, 2, 2, 2, 2, 2, 4, 4, 4, 2, 2, 2, 2, 3, 3 ],
						[ 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 3 ],
						[ 3, 3, 3, 3, 3, 3 ,3 ,3, 3, 3 ,3 ,3 ,3, 3, 3, 3, 3, 3 ]]

def getPosition():
	global mine_terminal
	if env_map[M.agentRow][M.agentColumn] != M.GOAL and env_map[M.agentRow][M.agentColumn] != M.MINE:
		# The episode terminates if the agent hits a mine
		return M.agentRow*M.col + M.agentColumn
	else:
		mine_terminal = 1
		return -1

def getNextPosition(action):
  # When the move would result in hitting an obstacles, the agent simply doesn't move
  newRow = M.agentRow
  newColumn = M.agentColumn
  
  if action.intArray[0] == 0:
    newColumn = M.agentColumn - 1
  elif action.intArray[0] == 1:
    newColumn = M.agentColumn + 1
  elif action.intArray[0] == 2:
    newRow = M.agentRow - 1
  elif action.intArray[0] == 3:
    newRow = M.agentRow + 1
  
  if newRow >= M.row or newRow < 0 or newColumn >= M.col or newColumn < 0:
    M.agentColumn = M.agentColumn
    M.agentRow = M.agentRow
  elif env_map[newRow][newColumn] != M.OBSTACLE:
    M.agentRow = newRow
    M.agentColumn = newColumn

def getReward():
	if env_map[M.agentRow][M.agentColumn] == M.GOAL:
		return 10
	elif env_map[M.agentRow][M.agentColumn] == M.MINE:
		return -10
	else:
		return -1

def env_init():
  M.START = 0
  M.GOAL = 1
  M.LAND = 2
  M.OBSTACLE = 3
  M.MINE = 4
  
  M.row = 5
  M.col = 18
  M.startRow= 1
  M.startCol = 12
  M.agentRow = M.startRow
  M.agentColumn = M.startCol
  
  mine_observation.numInts = 1
  mine_observation.numDoubles = 0

  mine_observation.doubleArray = [];

  random.seed(0)

  # Return task specification
  
  Task_spec = "1:e:1_[i]_[0,%d]:1_[i]_[0,%d]" % (M.row*M.col-1, 4-1)

  return Task_spec;

def env_start():
	r = 0
	c = 0

	mine_terminal = 0
	env_map[M.startRow][M.startCol] = M.LAND

	while env_map[r][c] != M.LAND:
		r = random.randint(0,2) % M.row
		c = random.randint(0,2) % M.col

	M.startRow = r
	M.startCol = c
	env_map[M.startRow][M.startCol] = M.START    

	M.agentColumn =  M.startCol
	M.agentRow = M.startRow

	mine_observation.intArray = [M.startRow * M.col + M.startCol]
	sys.stderr.write('env_start observation %d\n' % (mine_observation.intArray[0]))
	return mine_observation
	
def env_step(action):
	getNextPosition(action) # getNextPosition will update the values of agentRow and agentColumn
 
	mine_observation.intArray = [getPosition()]
  
	mine_ro.o = mine_observation
	mine_ro.r = getReward()
  
	if mine_terminal: # end of episode?
		mine_ro.terminal = 1
	else:
		mine_ro.terminal = 0
	
  	sys.stderr.write('env_step action %d\n' % (action.intArray[0]))
	sys.stderr.write('env_step observation %d\n' % (mine_observation.intArray[0]))
	return mine_ro

def env_cleanup():
	pass

def env_set_state(stateKey):
	pass

def env_set_random_seed(randomSeedKey):
	pass

def env_get_state():
	return State_key()

def env_get_random_seed():
	return Random_seed_key()

def env_message(inMessage):
	return None