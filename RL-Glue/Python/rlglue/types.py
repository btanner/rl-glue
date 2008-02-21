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


class Action:
	def __init__(self,numInts=None,numDoubles=None):
		self.intArray = []
		self.doubleArray = []
		if numInts != None:
			self.intArray = [0]*numInts
		if numDoubles != None:
			self.doubleArray = [0.0]*numDoubles

class Observation:
	def __init__(self,numInts=None,numDoubles=None):
		self.intArray = []
		self.doubleArray = []
		if numInts != None:
			self.intArray = [0]*numInts
		if numDoubles != None:
			self.doubleArray = [0.0]*numDoubles

class Random_seed_key:
	def __init__(self,numInts=None,numDoubles=None):
		self.intArray = []
		self.doubleArray = []
		if numInts != None:
			self.intArray = [0]*numInts
		if numDoubles != None:
			self.doubleArray = [0.0]*numDoubles

class State_key:
	def __init__(self,numInts=None,numDoubles=None):
		self.intArray = []
		self.doubleArray = []
		if numInts != None:
			self.intArray = [0]*numInts
		if numDoubles != None:
			self.doubleArray = [0.0]*numDoubles

class Observation_action:
	def __init__(self,theObservation=None,theAction=None):
		if theObservation != None:
			self.o = theObservation
		else:
			self.o = Observation()
		if theAction != None:
			self.a = theAction
		else:
			self.a = Action()

class Reward_observation:
	def __init__(self,reward=None, theObservation=None, terminal=None):
		if reward != None:
			self.r = reward
		else:
			self.r = 0.0
		if theObservation != None:
			self.o = theObservation
		else:
			self.o = Observation()
		if terminal != None:
			self.terminal = terminal
		else:
			self.terminal = False

class Reward_observation_action_terminal:
	def __init__(self,reward=None, theObservation=None, theAction=None, terminal=None):
		if reward != None:
			self.r = reward
		else:
			self.r = 0.0
		if theObservation != None:
			self.o = theObservation
		else:
			self.o = Observation()
		if theAction != None:
			self.a = theAction
		else:
			self.a = Action()
		if terminal != None:
			self.terminal = terminal
		else:
			self.terminal = False
	

