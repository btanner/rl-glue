import sys
import getopt
import socket
from RL_netlib import *

sock = None

def RL_init():
	global sock
	first = True
	isDaemon = False
	port = kDefaultPort
	host = kLocalHost

	try:
		opts, args = getopt.getopt(sys.argv[1:], "", ["stayalive","port=","host="])
	except getopt.GetoptError:
		# print help information and exit:
		print "Invalid args"
		sys.exit(2)

	for o, a in opts:
		if o == "--stayalive":
			isDaemon = True
		if o == "--port":
			port = int(a)
		if o == "--host":
			host = socket.gethostbyname(a)

	sock = waitForConnection(host,port,kRetryTimeout)
	sock.sendInt(kExperimentConnection)
	sock.sendInt(kRLInit)

def RL_start():
	sock.sendInt(kRLStart)
	return sock.recvObservationAction()

def RL_step():
	sock.sendInt(kRLStep)
	return sock.recvRewardObservationActionTerm()

def RL_cleanup():
	sock.sendInt(kRLCleanup)
	sock.close()

def RL_return():
	sock.sendInt(kRLReturn)
	return sock.recvDouble()

def RL_num_steps():
	sock.sendInt(kRLNumSteps)
	return sock.recvInt()

def RL_env_message(inMessage):
	sock.sendInt(kRLEnvMessage)
	sock.sendString(inMessage)
	outMessage = sock.recvString()
	return outMessage

def RL_agent_message(inMessage):
	sock.sendInt(kRLAgentMessage)
	sock.sendString(inMessage)
	outMessage = sock.recvString()
	return outMessage

def RL_num_episodes():
	sock.sendInt(kRLNumEpisodes)
	return sock.recvInt()

def RL_episode(num_steps):
	sock.sendInt(kRLEpisode)
	sock.sendInt(num_steps)

def RL_set_state(theStateKey):
	sock.sendInt(kRLSetState)
	sock.sendADT(theStateKey)

def RL_set_random_seed(theRandomSeedKey):
	sock.sendInt(kRLSetRandomSeed)
	sock.sendADT(theRandomSeedKey)

def RL_get_state():
	sock.sendInt(kRLGetState)
	theStateKey = sock.recvADT()
	return theStateKey

def RL_get_random_seed():
	sock.sendInt(kRLGetRandomSeed)
	theRandomSeedKey = sock.recvADT()
	return theRandomSeedKey