#! /usr/local/bin/python

import os
import sys
import getopt
import socket
sys.path = sys.path + ['./Build']
import time
from RL_agent import *
from RL_netlib import *


def onAgentInit(sock):
	taskSpec = sock.recvString()
	agent_init(taskSpec)

def onAgentStart(sock):
	theObservation = sock.recvADT()
	theAction = agent_start(theObservation)
	sock.sendADT(theAction)

def onAgentStep(sock):
	ro = sock.recvRewardObservation()
	theAction = agent_step(ro.r, ro.o)
	sock.sendADT(theAction)

def onAgentEnd(sock):
	reward = sock.recvDouble()
	agent_end(reward)

def onAgentCleanup(sock):
	agent_cleanup()

def onAgentFreeze(sock):
	agent_freeze()

def onAgentMessage(sock):
	inMessage = sock.recvString()
	outMessage = agent_message(inMessage)
	sock.sendString(outMessage)

def runAgentEventLoop(sock):
	agentState = 0
	while agentState != kAgentCleanup:
		agentState = sock.recvInt()
		if agentState == kAgentInit:
			onAgentInit(sock)
		elif agentState == kAgentStart:
			onAgentStart(sock)
		elif agentState == kAgentStep:
			onAgentStep(sock)
		elif agentState == kAgentEnd:
			onAgentEnd(sock)
		elif agentState == kAgentCleanup:
			onAgentCleanup(sock)
		elif agentState == kAgentFreeze:
			onAgentFreeze(sock)
		elif agentState == kAgentMessage:
			onAgentMessage(sock)
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
	sock.sendInt(kAgentConnection)
	runAgentEventLoop(sock)
	sock.close()
