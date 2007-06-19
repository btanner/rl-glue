#! /usr/local/bin/python
import sys
sys.path = sys.path + ['./Build']
from RL_client_experiment import *

def run(num_episodes):
  for x in range(num_episodes):
    RL_episode(10)
    sys.stderr.write(".")


RL_init()
run(100)
RL_cleanup()