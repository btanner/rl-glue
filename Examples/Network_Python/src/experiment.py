#! /usr/local/bin/python
import sys
import RL_common
import RLGlue

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