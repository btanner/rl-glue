#! /usr/local/bin/python
import sys
sys.path = sys.path + ['./Build']
from RL_client_experiment import *

NUM_EPISODES = 100
rl_num_steps = []
rl_return = []

def run(num_episodes):
  for x in range(num_episodes):
    RL_episode(0)
    sys.stderr.write(".")
    rl_num_steps.append(RL_num_steps())
    rl_return.append(RL_return())


RL_init()
run(NUM_EPISODES)
RL_cleanup()
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