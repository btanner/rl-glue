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

import socket
import struct
import array
import time
import sys
import StringIO
from RL_common import *

# RL-Glue needs to know what type of object is trying to connect.
kExperimentConnection  = 1
kAgentConnection       = 2
kEnvironmentConnection = 3

kAgentInit    = 4 # agent_* start by sending one of these values
kAgentStart   = 5 # to the client to let it know what type of
kAgentStep    = 6 # event to respond to
kAgentEnd     = 7
kAgentCleanup = 8
kAgentFreeze  = 9
kAgentMessage = 10

kEnvInit          = 11
kEnvStart         = 12
kEnvStep          = 13
kEnvCleanup       = 14
kEnvSetState      = 15
kEnvSetRandomSeed = 16
kEnvGetState      = 17
kEnvGetRandomSeed = 18
kEnvMessage       = 19

kRLInit           = 20
kRLStart          = 21
kRLStep           = 22
kRLCleanup        = 23
kRLReturn         = 24
kRLNumSteps       = 25
kRLNumEpisodes    = 26
kRLEpisode        = 27
kRLSetState       = 28
kRLSetRandomSeed  = 29
kRLGetState       = 30
kRLGetRandomSeed  = 31
kRLFreeze         = 32
kRLAgentMessage   = 33
kRLEnvMessage     = 34

kRLTerm           = 35

kLocalHost = "127.0.0.1"
kDefaultPort = 4096
kRetryTimeout = 10

class rlSocket:
	
	def __init__(self):
		self.socket = None

	def open(self):
		try:
			self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
			self.socket.setsockopt(socket.IPPROTO_TCP, socket.TCP_NODELAY, 1)
		except socket.error, msg:
			self.socket = None

	def acceptConnection(self):
		(self.conn, address) = self.socket.accept()
	
	def connect(self, address, port):
		try:
			self.socket.connect((address, port))
			return 1
		except socket.error, msg:
			return None
	
	def listen(self, port):
		self.socket.setsockopt(socket.SOL_SOCKET,socket.SO_REUSEADDR, 1)
		self.socket.bind(('',port))
		self.socket.listen(5)
	
	def close(self):
		self.socket.close()
	
	def isValidSocket(self):
		return self.socket == None

	def sendPacket(self, buf, target):
		self._sendInt(target)
		if buf == None:
			self._sendInt(0)
		else:
			self._sendInt(buf.size())
			self.socket.sendall(buf.get())
	
	def recvPacket(self):
		target = self._recvInt()
		length = self._recvInt()
		print "recvPacket, target: ",str(target)
		print "recvPacket, size: ",str(length)
		s = ''
		while len(s) < length:
			s += self.socket.recv(length-len(s))
		return (target,Buffer(s))
	
	def _sendInt(self, data):
		packet = struct.pack('!i',data)
		self.socket.sendall(packet)
	
	def _recvInt(self):
		s = ''
		while len(s) < 4:
			s += self.socket.recv(4-len(s))
		return struct.unpack('!i',s)[0]


class Buffer:
	data = ''
	def __init__(self,data=''):
		self.data = StringIO.StringIO(data)

	def readString(self):
		length = self.readInt()
		return self.data.read(length)

	def writeString(self,string):
		self.writeInt(len(string))
		self.data.write(string)

	def readADT(self):
		adt = RL_abstract_type()
		numInts = self.readInt()
		numDoubles = self.readInt()
		if numInts > 0:
			s = self.data.read(numInts*4)
			adt.intArray = struct.unpack("!%di" % (numInts),s)
		if numDoubles > 0:
			s = self.data.read(numDoubles*8)
			adt.doubleArray = struct.unpack("!%dd" % (numDoubles),s)
		return adt

	def writeADT(self,adt):
		self.writeInt(len(adt.intArray))
		self.writeInt(len(adt.doubleArray))
		if len(adt.intArray) > 0:
			self.data.write(struct.pack("!%di" % (len(adt.intArray)),*(adt.intArray)))
		if len(adt.doubleArray) > 0:
			self.data.write(struct.pack("!%dd" % (len(adt.doubleArray)),*(adt.doubleArray)))

	def readRewardObservation(self):
		ro = Reward_observation()
		ro.r = self.readReward()
		ro.o = self.readADT()
		return ro

	def writeRewardObservation(self,ro):
		self.writeInt(ro.terminal)
		self.writeReward(ro.r)
		self.writeADT(ro.o)

	def readReward(self):
		s = self.data.read(8)
		return struct.unpack("!d",s)[0]

	def writeReward(self,r):
		self.data.write(struct.pack("!d",r))

	def readObservationAction(self):
		oa = Observation_action()
		oa.o = self.readADT()
		oa.a = self.readADT()
		return oa

	def readRewardObservationActionTerm(self):
		roat = Reward_observation_action_terminal()
		roat.terminal = self.readInt()
		roat.r = self.readReward()
		roat.o = self.readADT()
		roat.a = self.readADT()
		return roat

	def readInt(self):
		s = self.data.read(4)
		return struct.unpack("!i",s)[0]

	def writeInt(self,i):
		self.data.write(struct.pack("!i",i))
	
	def get(self):
		return self.data.getvalue()
	
	def size(self):
		return len(self.data.getvalue())

def waitForConnection(address,port,retryTimeout):
	sock = rlSocket()
	isConnected = None
	while (isConnected == None):
		sock.open()
		isConnected = sock.connect(address,port)
		if (isConnected == None):
			sock.close()
			time.sleep(retryTimeout)
	return sock