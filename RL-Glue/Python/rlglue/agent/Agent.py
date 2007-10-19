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
