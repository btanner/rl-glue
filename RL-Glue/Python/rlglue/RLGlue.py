import sys

import rlglue.network.Network as Network
from rlglue.types import Observation_action
from rlglue.types import Random_seed_key 
from rlglue.types import Reward_observation_action_terminal
from rlglue.types import State_key

network = None

# () -> void
def forceConnection():
	global network
	if network == None:
		network = Network.Network()
		network.connect()
		network.clearSendBuffer()
		network.putInt(Network.kExperimentConnection)
		network.putInt(0)
		network.send()

# (int) -> void
def doStandardRecv(state):
	recvSize = network.recv(8) - 8
	
	glueState = network.getInt()
	dataSize = network.getInt()
	remaining = dataSize - recvSize
	
	if remaining < 0:
		remaining = 0
	
	remainingReceived = network.recv(remaining)
	
	# discard the header
	network.getInt()
	network.getInt()
	
	if (glueState != state):
		sys.stderr.write("Not synched with server. glueState = " + str(glueState) + " but should be " + str(state) + "\n")
		sys.exit(1)

# (int) -> void
def doCallWithNoParams(state):
	network.clearSendBuffer()
	network.putInt(state)
	network.putInt(0)
	network.send()

# () -> void
def RL_init():
	forceConnection()
	doCallWithNoParams(Network.kRLInit)
	doStandardRecv(Network.kRLInit)

# () -> Observation_action
def RL_start():
	obsact = None
	doCallWithNoParams(Network.kRLStart)
	doStandardRecv(Network.kRLStart)
	obsact = Observation_action()
	obsact.o = network.getObservation()
	obsact.a = network.getAction()
	return obsact

# () -> Reward_observation_action_terminal
def RL_step():
	roat = None
	doCallWithNoParams(Network.kRLStep)
	doStandardRecv(Network.kRLStep)
	roat = Reward_observation_action_terminal()
	roat.terminal = network.getInt()
	roat.r = network.getDouble()
	roat.o = network.getObservation()
	roat.a = network.getAction()
	return roat

# () -> void
def RL_cleanup():
	doCallWithNoParams(Network.kRLCleanup)
	doStandardRecv(Network.kRLCleanup)

# (string) -> string
def RL_agent_message(message):
	response = ""
	forceConnection()
	network.clearSendBuffer()
	network.putInt(Network.kRLAgentMessage)
	network.putInt(Network.sizeOf(message))
	network.putString(message)
	network.send()
	doStandardRecv(Network.kRLAgentMessage)
	response = network.getString()
	return response

# (string) -> string
def RL_env_message(message):
	response = ""
	forceConnection()
	network.clearSendBuffer()
	network.putInt(Network.kRLEnvMessage)
	network.putInt(Network.sizeOf(message))
	network.putString(message)
	network.send()
	doStandardRecv(Network.kRLEnvMessage)
	response = network.getString()
	return response

# () -> double
def RL_return():
	reward = 0.0
	doCallWithNoParams(Network.kRLReturn)
	doStandardRecv(Network.kRLReturn)
	reward = network.getDouble()
	return reward

# () -> int
def RL_num_steps():
	numSteps = 0
	doCallWithNoParams(Network.kRLNumSteps)
	doStandardRecv(Network.kRLNumSteps)
	numSteps = network.getInt()
	return numSteps

# () -> int
def RL_num_episodes():
	numEpisodes = 0
	doCallWithNoParams(Network.kRLNumEpisodes)
	doStandardRecv(Network.kRLNumEpisodes)
	numEpisodes = network.getInt()
	return numEpisodes

# (int) -> void
def RL_episode(num_steps):
	network.clearSendBuffer()
	network.putInt(Network.kRLEpisode)
	network.putInt(Network.kIntSize)
	network.putInt(numSteps)
	network.send()
	doStandardRecv(Network.kRLEpisode)

# () -> void
def RL_freeze():
	doCallWithNoParams(Network.kRLFreeze)
	doStandardRecv(Network.kRLFreeze)

# (State_key) -> void
def RL_set_state(sk):
	network.clearSendBuffer()
	network.putInt(Network.kRLSetState)
	network.putInt(Network.sizeOf(sk))
	network.putStateKey(sk)
	network.send()
	doStandardRecv(Network.kRLSetState)

# (Random_seed_key) -> void
def RL_set_random_key(rsk):
	network.clearSendBuffer()
	network.putInt(Network.kRLSetRandomSeed)
	network.putInt(Network.sizeOf(rsk))
	network.putRandomSeedKey(rsk)
	network.send()
	doStandardRecv(Network.kRLSetRandomSeed)

# () -> State_key
def RL_get_state():
	key = None
	doCallWithNoParams(Network.kRLGetState)
	doStandardRecv(Network.kRLGetState)
	key = network.getStateKey()
	return key

# () -> Random_seed_key
def RL_get_random_seed():
	key = None
	doCallWithNoParams(Network.kRLGetRandomSeed)
	doStandardRecv(Network.kRLGetRandomSeed)
	key = network.getRandomSeedKey()
	return key