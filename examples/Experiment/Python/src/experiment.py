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

import sys

import rlglue.RLGlue as RLGlue


NUM_EPISODES = 1000
rl_num_steps = []
rl_return = []

def run(num_episodes):
  for x in range(num_episodes):
    RLGlue.RL_episode(0)
    sys.stderr.write(".")
    rl_num_steps.append(RLGlue.RL_num_steps())
    rl_return.append(RLGlue.RL_return())


RLGlue.RL_init()
run(NUM_EPISODES)
RLGlue.RL_cleanup()
avg_steps = 0.0
avg_return = 0.0
for i in range(NUM_EPISODES):
	avg_steps += rl_num_steps[i]
	avg_return += rl_return[i]
avg_steps /= NUM_EPISODES
avg_return /= NUM_EPISODES
print "\n-----------------------------------------------"
print "Number of episodes:",NUM_EPISODES
print "Average number of steps per episode:",avg_steps
print "Average return per episode:", avg_return
print "-----------------------------------------------"