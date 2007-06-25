#! /usr/local/bin/python
import os
import sys
import time
import socket
import getopt
sys.path = sys.path + ['./Build']
from RL_environment import *
from RL_netlib import *

def onEnvInit(sock):
	taskSpec = env_init()
	sock.sendString(taskSpec)

def onEnvStart(sock):
	theObservation = env_start()
	sock.sendADT(theObservation)

def onEnvStep(sock):
	theAction = sock.recvADT()
	ro = env_step(theAction)
	sock.sendRewardObservation(ro)

def onEnvCleanup(sock):
	env_cleanup()

def onEnvSetState(sock):
	theStateKey = sock.recvADT()
	env_set_state(theStateKey)
	
def onEnvSetRandomSeed(sock):
	theRandomSeed = sock.recvADT()
	env_set_random_seed(theRandomSeed)

def onEnvGetState(sock):
	theStateKey = env_get_state()
	sock.sendADT(theStateKey)
	
def onEnvGetRandomSeed(sock):
	theRandomSeed = env_get_random_seed()
	sock.sendADT(theRandomSeed)

def onEnvMessage(sock):
	inMessage = sock.recvString()
	outMessage = env_message(inMessage)
	sock.sendString(outMessage)

def runEnvironmentEventLoop(sock):
	envState = 0
	
	while envState != kEnvCleanup:
		envState = sock.recvInt()
		if envState == kEnvInit:
			onEnvInit(sock)
		elif envState == kEnvStart:
			onEnvStart(sock)
		elif envState == kEnvStep:
			onEnvStep(sock)
		elif envState == kEnvCleanup:
			onEnvCleanup(sock)
		elif envState == kEnvSetState:
			onEnvSetState(sock)
		elif envState == kEnvSetRandomSeed:
			onEnvSetRandomSeed(sock)
		elif envState == kEnvGetState:
			onEnvGetState(sock)
		elif envState == kEnvGetRandomSeed:
			onEnvGetRandomSeed(sock)
		elif envState == kEnvMessage:
			onEnvMessage(sock)
		else:
			sys.stderr.write(kUnknownMessage % (agentState))

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

while isDaemon or first:
	first = False
	sock = waitForConnection(host,port,kRetryTimeout)
	sock.sendInt(kEnvironmentConnection)
	runEnvironmentEventLoop(sock)
	sock.close()
