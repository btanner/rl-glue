import random
import sys
from RL_common import *


sarsaAgentAction = Action()
Q = []
Epsilon = 0.1
alpha = 0.01
gamma = 0.9
NumStates = 108
NumActions = 4
old_s = 0
old_a = 0

def agent_init(taskSpec):
	random.seed(0)
	sarsaAgentAction.intArray = []
	sarsaAgentAction.doubleArray = []
	for s in range(NumStates):
		Q.append([])
		for a in range(NumActions):
			Q[s].append(0)

def agent_start(observation):
	global old_s, old_a
	s = observation.intArray[0]
	a = E_greedy(s)
	old_s = s
	old_a = a
	sarsaAgentAction.intArray = [a]
	#sys.stderr.write('agent_start observation: %d\n' % (observation.intArray[0]))
	#sys.stderr.write('agent_start action: %d\n' % (sarsaAgentAction.intArray[0]))
	return sarsaAgentAction


def agent_step(reward, observation):
	global old_s, old_a
	s = observation.intArray[0]
	a = E_greedy(s)
	Q[old_s][old_a] += alpha*(reward + gamma*Q[s][a] - Q[old_s][old_a])
	old_s = s
	old_a = a
	sarsaAgentAction.intArray = [a]
	#sys.stderr.write('agent_step observation: %d\n' % (observation.intArray[0]))
	#sys.stderr.write('agent_step action: %d\n' % (sarsaAgentAction.intArray[0]))
	return sarsaAgentAction

def agent_end(reward):
	Q[old_s][old_a] += alpha*(reward - Q[old_s][old_a])

def agent_cleanup():
	pass

def agent_freeze():
	pass

def agent_message(inMessage):
	return None

def E_greedy(s):
	if random.random() < Epsilon:
		return random.randint(0,NumActions-1)
	else:
		best = None
		bestA = []
		for a in range(NumActions):
			if best == None or Q[s][a] > best:
				bestA = [a]
				best = Q[s][a]
			elif Q[s][a] == best:
				bestA.append(a)
		return bestA[random.randint(0,len(bestA)-1)]
