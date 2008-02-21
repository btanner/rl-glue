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