from RL_common import *
import random
import sys

zeroAgentAction = Action()
zeroAgentObservation = Observation()

def agent_init(taskSpec):
	random.seed(0)
	zeroAgentAction.numInts = 1
	zeroAgentAction.numDoubles = 0
	zeroAgentAction.intArray = [1]

def agent_start(observation):
	zeroAgentObservation = observation
	zeroAgentAction.intArray = [random.randint(0,2)]
	sys.stderr.write('agent_start observation: %d\n' % (zeroAgentObservation.intArray[0]))
	sys.stderr.write('agent_start action: %d\n' % (zeroAgentAction.intArray[0]))
	return zeroAgentAction

def agent_step(reward, observation):
	zeroAgentObservation = observation
	zeroAgentAction.intArray = [random.randint(0,2)]
	sys.stderr.write('agent_step observation: %d\n' % (zeroAgentObservation.intArray[0]))
	sys.stderr.write('agent_step action: %d\n' % (zeroAgentAction.intArray[0]))
	return zeroAgentAction

def agent_end(reward):
	pass

def agent_cleanup():
	pass

def agent_freeze():
	pass

def agent_message(inMessage):
	return None