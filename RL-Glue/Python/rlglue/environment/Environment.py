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
