import os
import sys
import getopt
import socket
from RL_network import *

sock = None

def connectSocket():
	global sock
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

def RL_init():
	if sock == None:
		connectSocket()
	sock.sendPacket(None,kRLInit)
	(state,buf) = sock.recvPacket()

def RL_start():
	if sock == None:
		connectSocket()
	sock.sendPacket(None,kRLStart)
	(state,buf) = sock.recvPacket()
	return buf.readObservationAction()

def RL_step():
	if sock == None:
		connectSocket()
	sock.sendPacket(None,kRLStep)
	(state,buf) = sock.recvPacket()
	return buf.readRewardObservationActionTerm()

def RL_cleanup():
	if sock == None:
		connectSocket()
	sock.sendPacket(None,kRLCleanup)
	(state,buf) = sock.recvPacket()

def RL_return():
	if sock == None:
		connectSocket()
	sock.sendInt(None,kRLReturn)
	(state,buf) = sock.recvPacket()
	return buf.readReward()

def RL_num_steps():
	if sock == None:
		connectSocket()
	sock.sendPacket(None,kRLNumSteps)
	(state,buf) = sock.recvPacket()
	return buf.readInt()

def RL_env_message(inMessage):
	if sock == None:
		connectSocket()
	buf = Buffer()
	buf.writeString(inMessage)
	sock.sendPacket(buf, kRLEnvMessage)
	(state,buf) = sock.recvPacket()
	outMessage = buf.readString()
	return outMessage

def RL_agent_message(inMessage):
	if sock == None:
		connectSocket()
	buf = Buffer()
	buf.writeString(inMessage)
	sock.sendPacket(buf, kRLAgentMessage)
	(state,buf) = sock.recvPacket()
	outMessage = buf.readString()
	return outMessage

def RL_num_episodes():
	if sock == None:
		connectSocket()
	sock.sendPacket(None,kRLNumEpisodes)
	(state,buf) = sock.recvPacket()
	return buf.readInt()

def RL_episode(num_steps):
	if sock == None:
		connectSocket()
	buf = Buffer()
	buf.writeInt(num_steps)
	sock.sendPacket(buf,kRLEpisode)
	(state,buf) = sock.recvPacket()

def RL_set_state(theStateKey):
	if sock == None:
		connectSocket()
	buf = Buffer()
	buf.writeADT(theStateKey)
	sock.sendPacket(buf,kRLSetState)
	(state,buf) = sock.recvPacket()

def RL_set_random_seed(theRandomSeedKey):
	if sock == None:
		connectSocket()
	buf = Buffer()
	buf.writeADT(theRandomSeedKey)
	sock.sendPacket(buf,kRLSetRandomSeed)
	(state,buf) = sock.recvPacket()

def RL_get_state():
	if sock == None:
		connectSocket()
	sock.sendPacket(None,kRLGetState)
	(state,buf) = sock.recvPacket()
	return buf.readADT()

def RL_get_random_seed():
	if sock == None:
		connectSocket()
	sock.sendPacket(None,kRLGetRandomSeed)
	(state,buf) = sock.recvPacket()
	return buf.readADT()