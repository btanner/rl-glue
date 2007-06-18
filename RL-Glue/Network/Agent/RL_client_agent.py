#! /usr/local/bin/python
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

while isDaemon or first:
	first = False
	sock = waitForConnection(host,port,kRetryTimeout)
	sock.sendInt(kAgentConnection)
	runAgentEventLoop(sock)
	sock.close()
