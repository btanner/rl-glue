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