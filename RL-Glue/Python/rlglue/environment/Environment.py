
from rlglue.types import Action
from rlglue.types import Observation
from rlglue.types import Random_seed_key
from rlglue.types import Reward_observation
from rlglue.types import State_key

class Environment:
	# () -> string
	def env_init():
		pass
	
	# () -> Observation
 	def env_start():
		pass
	
	# (Action) -> Reward_observation
	def env_step(action):
		pass
	
	# () -> void
	def env_cleanup():
		pass
	
	# (State_key) -> void
	def env_set_state(key):
		pass
	
	# (Random_seed_key) -> void
	def env_set_random_seed(key):
		pass
	
	# () -> State_key
	def env_get_state():
		pass
	
	# () -> Random_seed_key
	def env_get_random_seed():
		pass
	
	# (string) -> string
	def env_message(message):
		pass
