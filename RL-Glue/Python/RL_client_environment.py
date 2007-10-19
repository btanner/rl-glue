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
import time
import socket
import getopt
from RL_environment import *
from RL_network import *

def onEnvInit(buf):
	taskSpec = env_init()
	buf = Buffer()
	buf.writeString(taskSpec)
	return buf

def onEnvStart(buf):
	theObservation = env_start()
	buf = Buffer()
	buf.writeADT(theObservation)
	return buf

def onEnvStep(buf):
	theAction = buf.readADT()
	ro = env_step(theAction)
	buf = Buffer()
	buf.writeRewardObservation(ro)
	return buf

def onEnvCleanup(buf):
	env_cleanup()
	return None

def onEnvSetState(buf):
	theStateKey = buf.readADT()
	env_set_state(theStateKey)
	return None
	
def onEnvSetRandomSeed(buf):
	theRandomSeed = buf.readADT()
	env_set_random_seed(theRandomSeed)
	return None

def onEnvGetState(buf):
	theStateKey = env_get_state()
	buf = Buffer()
	buf.writeADT(theStateKey)
	return buf
	
def onEnvGetRandomSeed(buf):
	theRandomSeed = env_get_random_seed()
	buf = Buffer()
	buf.writeADT(theRandomSeed)
	return buf

def onEnvMessage(buf):
	inMessage = buf.readString()
	outMessage = env_message(inMessage)
	buf = Buffer()
	buf.writeString(outMessage)
	return buf

def runEnvironmentEventLoop(sock):
	envState = 0
	
	while envState != kEnvCleanup:
		(envState,buf) = sock.recvPacket()
		print "envState = ",envState
		if envState == kEnvInit:
			buf = onEnvInit(buf)
		elif envState == kEnvStart:
			buf = onEnvStart(buf)
		elif envState == kEnvStep:
			buf = onEnvStep(buf)
		elif envState == kEnvCleanup:
			buf = onEnvCleanup(buf)
		elif envState == kEnvSetState:
			buf = onEnvSetState(buf)
		elif envState == kEnvSetRandomSeed:
			buf = onEnvSetRandomSeed(buf)
		elif envState == kEnvGetState:
			buf = onEnvGetState(buf)
		elif envState == kEnvGetRandomSeed:
			buf = onEnvGetRandomSeed(buf)
		elif envState == kEnvMessage:
			buf = onEnvMessage(buf)
		else:
			sys.stderr.write(kUnknownMessage % (envState))
		print "sending back envState= ",envState
		sock.sendPacket(buf,envState)

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
	sock.sendPacket(None,kEnvironmentConnection)
	runEnvironmentEventLoop(sock)
	sock.close()
