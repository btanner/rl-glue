from rlglue.types import Action
from rlglue.types import Observation

class Agent:
	# (string) -> void
	def agent_init(taskSpecification):
		pass
	
	# (Observation) -> Action
	def agent_start(observation):
		pass
	
	# (double, Observation) -> Action
	def agent_step(reward, observation):
		pass
	
	# (double) -> void
	def agent_end(reward):
		pass
	
	# () -> void
	def agent_cleanup():
		pass
	
	# () -> void
	def agent_freeze():
		pass
	
	# (string) -> string
	def agent_message(message):
		pass
