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

class RL_abstract_type:
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