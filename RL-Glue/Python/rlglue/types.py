
class Action:
	intArray = []
	doubleArray = []
	def __init__(self,numInts=None,numDoubles=None):
		if numInts != None:
			intArray = [0]*numInts
		if numDoubles != None:
			doubleArray = [0.0]*numDoubles

class Observation:
	intArray = []
	doubleArray = []
	def __init__(self,numInts=None,numDoubles=None):
		if numInts != None:
			intArray = [0]*numInts
		if numDoubles != None:
			doubleArray = [0.0]*numDoubles

class Random_seed_key:
	intArray = []
	doubleArray = []
	def __init__(self,numInts=None,numDoubles=None):
		if numInts != None:
			intArray = [0]*numInts
		if numDoubles != None:
			doubleArray = [0.0]*numDoubles

class State_key:
	intArray = []
	doubleArray = []
	def __init__(self,numInts=None,numDoubles=None):
		if numInts != None:
			intArray = [0]*numInts
		if numDoubles != None:
			doubleArray = [0.0]*numDoubles

class Observation_action:
	o = Observation()
	a = Action()
	def __init__(self,theObservation=None,theAction=None):
		if theObservation != None:
			this.o = theObservation
		if theAction != None:
			this.a = theAction

class Reward_observation:
	r = 0.0
	o = Observation()
	terminal = False
	def __init__(self,reward=None, theObservation=None, terminal=None):
		if reward != None:
			r = reward
		if theObservation != None:
			this.o = theObservation
		if terminal != None:
			this.terminal = terminal

class Reward_observation_action_terminal:
	r = 0.0
	o = Observation()
	a = Action()
	terminal = False
	def __init__(self,reward=None, theObservation=None, theAction=None, terminal=None):
		if reward != None:
			r = reward
		if theObservation != None:
			this.o = theObservation
		if theAction != None:
			this.a = theAction
		if terminal != None:
			this.terminal = terminal