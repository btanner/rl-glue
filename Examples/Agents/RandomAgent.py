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
from rlglue.agent.Agent import Agent
from rlglue.types import Action
from rlglue.types import Observation

class RandomAgent(Agent):
	
	action = Action(1,0)
	
	def agent_init(self,taskSpec):
		random.seed(0)
	
	def agent_start(self,observation):
		self.randomify(self.action)
		return self.action
	
	def agent_step(self,reward, observation):
		self.randomify(self.action)
		return self.action
	
	def agent_end(self,reward):
		pass
	
	def agent_cleanup(self):
		pass
	
	def agent_freeze(self):
		pass
	
	def agent_message(self,inMessage):
		return None
	
	def randomify(self,action):
		action.intArray = [random.randrange(4)]
	
