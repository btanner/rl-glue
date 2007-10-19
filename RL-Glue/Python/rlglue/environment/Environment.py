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
