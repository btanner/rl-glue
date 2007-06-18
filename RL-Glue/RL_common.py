
class RL_abstract_type:
	numInts = 0
	numDoubles = 0
	intArray = []
	doubleArray = []

Observation = RL_abstract_type
Action = RL_abstract_type
Random_seed_key = RL_abstract_type
State_key = RL_abstract_type

class Observation_action:
	o = Observation()
	a = Action()

class Reward_observation:
	r = 0.0
	o = Observation()
	terminal = False

class Reward_observation_action_terminal:
	r = 0.0
	o = Observation()
	a = Action()
	terminal = False