# 
# Copyright (C) 2007, Mark Lee
# 
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

import random
import sys
from rlglue.environment.Environment import Environment
from rlglue.types import Observation
from rlglue.types import Action
from rlglue.types import Reward_observation
from rlglue.types import State_key
from rlglue.types import Random_seed_key

class mines(Environment):
	class MineEnv:
		START = 0					# Start marker in grid
		GOAL = 1					# End marker in grid
		LAND = 2					# Free space in grid
		OBSTACLE = 3			# Obstical in grid
		MINE = 4					# Mine in grid
		row = 6						# Number of rows in grid
		col = 18					# Number of columns in grid
		startRow = 1			# Starting position
		startCol = 12			# Starting position
		agentRow = 0			# Agent's current position
		agentColumn = 0		# Agent's current position
	
	
	MINES_RANDOM_START=1 #Whether or not to start agent in a random position each episode
	
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
	
	def getPosition(self):
		if self.env_map[self.M.agentRow][self.M.agentColumn] != self.M.GOAL and self.env_map[self.M.agentRow][self.M.agentColumn] != self.M.MINE:
			# The episode terminates if the agent hits a mine
			return self.M.agentRow*self.M.col + self.M.agentColumn
		else:
			self.mine_terminal = 1
			return -1
	
	def getNextPosition(self,action):
	  # When the move would result in hitting an obstacles, the agent simply doesn't move
	  newRow = self.M.agentRow
	  newColumn = self.M.agentColumn
  
	  if action.intArray[0] == 0:
	    newColumn = self.M.agentColumn - 1
	  elif action.intArray[0] == 1:
	    newColumn = self.M.agentColumn + 1
	  elif action.intArray[0] == 2:
	    newRow = self.M.agentRow - 1
	  elif action.intArray[0] == 3:
	    newRow = self.M.agentRow + 1
  
	  if newRow >= self.M.row or newRow < 0 or newColumn >= self.M.col or newColumn < 0:
	    self.M.agentColumn = self.M.agentColumn
	    self.M.agentRow = self.M.agentRow
	  elif self.env_map[newRow][newColumn] != self.M.OBSTACLE:
	    self.M.agentRow = newRow
	    self.M.agentColumn = newColumn
	
	def getReward(self):
		if self.env_map[self.M.agentRow][self.M.agentColumn] == self.M.GOAL:
			return 10
		elif self.env_map[self.M.agentRow][self.M.agentColumn] == self.M.MINE:
			return -10
		else:
			return -1
	
	def env_init(self):  
	  self.mine_observation.doubleArray = []
		
	  random.seed(0)
		
	  # Return task specification
  	
	  Task_spec = "2:e:1_[i]_[0,%d]:1_[i]_[0,%d]:[-10,10]" % (self.M.row*self.M.col-1, 4-1)
		
	  return Task_spec;
	
	def env_start(self):
		r = 0
		c = 0
		
		self.mine_terminal = 0
		self.env_map[self.M.startRow][self.M.startCol] = self.M.LAND
		
		if self.MINES_RANDOM_START: #should we start each episode in a random position?
			while self.env_map[r][c] != self.M.LAND:
				r = random.randint(0,self.M.row-1)
				c = random.randint(0,self.M.col-1)
			self.M.startRow = r
			self.M.startCol = c
		
		self.M.agentColumn =  self.M.startCol
		self.M.agentRow = self.M.startRow
		
		self.env_map[self.M.startRow][self.M.startCol] = self.M.START
		
		self.mine_observation.intArray = [self.getPosition()]
		#sys.stderr.write('env_start observation %d\n' % (mine_observation.intArray[0]))
		return self.mine_observation
	
	def env_step(self,action):
		self.getNextPosition(action) # getNextPosition will update the values of agentRow and agentColumn
		
		self.mine_observation.intArray = [self.getPosition()]
		
		self.mine_ro.o = self.mine_observation
		self.mine_ro.r = self.getReward()
  	
		if self.mine_terminal: # end of episode?
			self.mine_ro.terminal = 1
		else:
			self.mine_ro.terminal = 0
		
		#sys.stderr.write('env_step action %d\n' % (action.intArray[0]))
		#sys.stderr.write('env_step observation %d\n' % (mine_observation.intArray[0]))
		return self.mine_ro
	
	def env_cleanup(self):
		pass
	
	def env_set_state(self, stateKey):
		pass
	
	def env_set_random_seed(self, randomSeedKey):
		pass
	
	def env_get_state(self):
		return State_key()
	
	def env_get_random_seed(self):
		return Random_seed_key()
	
	def env_message(self,inMessage):
		return None
	
