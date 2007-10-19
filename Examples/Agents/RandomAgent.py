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
		randomify(action)
		return action
	
	def agent_step(self,reward, observation):
		randomify(action)
		return action
	
	def agent_end(self,reward):
		pass
	
	def agent_cleanup(self):
		pass
	
	def agent_freeze(self):
		pass
	
	def agent_message(self,inMessage):
		return None
	
	def randomify(self,action):
		action.intArray[0] = random.randrange(4)
	
