# 
# Copyright (C) 2007, Mark Lee
# 
#http://rl-glue.googlecode.com/
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

import random
import sys
from rlglue.agent.Agent import Agent
import rlglue.agent.AgentLoader
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
	
