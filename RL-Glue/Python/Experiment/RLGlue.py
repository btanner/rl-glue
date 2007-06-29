import os
import sys
import getopt
import socket
from RL_network import *

sock = None

def RL_init():
	global sock
	first = True
	isDaemon = False
	port = kDefaultPort
	host = kLocalHost

	if os.environ.has_key('RLGLUE_HOST'):
		host = socket.gethostbyname(os.environ['RLGLUE_HOST'])

	if os.environ.has_key('RLGLUE_PORT'):
		port = int(os.environ['RLGLUE_PORT'])

	if os.environ.has_key('RLGLUE_AUTORECONNECT'):
		isDaemon = int(os.environ['RLGLUE_AUTORECONNECT']) != 0

	sock = waitForConnection(host,port,kRetryTimeout)
	sock.sendPacket(None,kExperimentConnection)
	sock.sendPacket(None,kRLInit)
	(state,buf) = sock.recvPacket()

def RL_start():
	sock.sendPacket(None,kRLStart)
	(state,buf) = sock.recvPacket()
	return buf.readObservationAction()

def RL_step():
	sock.sendPacket(None,kRLStep)
	(state,buf) = sock.recvPacket()
	return buf.readRewardObservationActionTerm()

def RL_cleanup():
	sock.sendPacket(None,kRLCleanup)
	(state,buf) = sock.recvPacket()
	sock.close()

def RL_return():
	sock.sendPacket(None,kRLReturn)
	(state,buf) = sock.recvPacket()
	return buf.readReward()

def RL_num_steps():
	sock.sendPacket(None,kRLNumSteps)
	(state,buf) = sock.recvPacket()
	return buf.readInt()

def RL_env_message(inMessage):
	buf = Buffer()
	buf.writeString(inMessage)
	sock.sendPacket(buf, kRLEnvMessage)
	(state,buf) = sock.recvPacket()
	outMessage = buf.readString()
	return outMessage

def RL_agent_message(inMessage):
	buf = Buffer()
	buf.writeString(inMessage)
	sock.sendPacket(buf, kRLAgentMessage)
	(state,buf) = sock.recvPacket()
	outMessage = buf.readString()
	return outMessage

def RL_num_episodes():
	sock.sendPacket(None,kRLNumEpisodes)
	(state,buf) = sock.recvPacket()
	return buf.readInt()

def RL_episode(num_steps):
	buf = Buffer()
	buf.writeInt(num_steps)
	sock.sendPacket(buf,kRLEpisode)
	(state,buf) = sock.recvPacket()

def RL_set_state(theStateKey):
	buf = Buffer()
	buf.writeADT(theStateKey)
	sock.sendPacket(buf,kRLSetState)
	(state,buf) = sock.recvPacket()

def RL_set_random_seed(theRandomSeedKey):
	buf = Buffer()
	buf.writeADT(theRandomSeedKey)
	sock.sendPacket(buf,kRLSetRandomSeed)
	(state,buf) = sock.recvPacket()

def RL_get_state():
	sock.sendPacket(None,kRLGetState)
	(state,buf) = sock.recvPacket()
	return buf.readADT()

def RL_get_random_seed():
	sock.sendPacket(None,kRLGetRandomSeed)
	(state,buf) = sock.recvPacket()
	return buf.readADT()