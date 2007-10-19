#! /usr/local/bin/python

# 
# Copyright (C) 2007, Mark Lee
# 
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

import os
import sys
import getopt
import socket
import time
from RL_agent import *
from RL_network import *


def onAgentInit(buf):
	taskSpec = buf.readString()
	agent_init(taskSpec)
	return None

def onAgentStart(buf):
	theObservation = buf.readADT()
	theAction = agent_start(theObservation)
	buf = Buffer()
	buf.writeADT(theAction)
	return buf

def onAgentStep(buf):
	ro = buf.readRewardObservation()
	theAction = agent_step(ro.r, ro.o)
	buf = Buffer()
	buf.writeADT(theAction)
	return buf

def onAgentEnd(buf):
	reward = buf.readReward()
	agent_end(reward)
	return None

def onAgentCleanup(buf):
	agent_cleanup()
	return None

def onAgentFreeze(buf):
	agent_freeze()
	return None

def onAgentMessage(buf):
	inMessage = buf.readString()
	outMessage = agent_message(inMessage)
	buf = Buffer()
	buf.writeString(outMessage)
	return buf

def runAgentEventLoop(sock):
	agentState = 0
	while agentState != kAgentCleanup:
		(agentState,buf) = sock.recvPacket()
		if agentState == kAgentInit:
			buf = onAgentInit(buf)
		elif agentState == kAgentStart:
			buf = onAgentStart(buf)
		elif agentState == kAgentStep:
			buf = onAgentStep(buf)
		elif agentState == kAgentEnd:
			buf = onAgentEnd(buf)
		elif agentState == kAgentCleanup:
			buf = onAgentCleanup(buf)
		elif agentState == kAgentFreeze:
			buf = onAgentFreeze(buf)
		elif agentState == kAgentMessage:
			buf = onAgentMessage(buf)
		else:
			sys.stderr.write(kUnknownMessage % (agentState))
		sock.sendPacket(buf,agentState)

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
	sock.sendPacket(None,kAgentConnection)
	runAgentEventLoop(sock)
	sock.close()
