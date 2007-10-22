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

from rlglue.types import Action
from rlglue.types import Observation
from rlglue.types import State_key
from rlglue.types import Random_seed_key
from rlglue.types import Reward_observation

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

kDefaultBufferSize = 4096
kIntSize = 4
kDoubleSize = 8

kUnknownMessage = "Unknown Message: %s\n"

class Network:
	
	def __init__(self):
		self.sock = None
		self.recvBuffer = StringIO.StringIO('')
		self.sendBuffer = StringIO.StringIO('')
	
	def connect(self, host=kLocalHost, port=kDefaultPort, retryTimeout=kRetryTimeout):
		while self.sock == None:
			try:
				self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
				self.sock.setsockopt(socket.IPPROTO_TCP, socket.TCP_NODELAY, 1)
				self.sock.connect((host, port))
			except socket.error, msg:
				self.sock = None
				time.sleep(retryTimeout)
			else:
				break
	
	def close(self):
		self.sock.close()
		
	def send(self):
		self.sock.sendall(self.sendBuffer.getvalue())
	
	def recv(self,size):
		s = ''
		while len(s) < size:
			s += self.sock.recv(size - len(s))
		self.recvBuffer.write(s)
		self.recvBuffer.seek(0)
		return len(s)
	
	def clearSendBuffer(self):
		self.sendBuffer.close()
		self.sendBuffer = StringIO.StringIO()
	
	def clearRecvBuffer(self):
		self.recvBuffer.close()
		self.recvBuffer = StringIO.StringIO()
	
	def flipSendBuffer(self):
		self.clearSendBuffer()

	def flipRecvBuffer(self):
		self.clearRecvBuffer()
	
	def getInt(self):
		s = self.recvBuffer.read(kIntSize)
		return struct.unpack("!i",s)[0]
	
	def getDouble(self):
		s = self.recvBuffer.read(kDoubleSize)
		return struct.unpack("!d",s)[0]
	
	def getString(self):
		length = self.getInt()
		return self.recvBuffer.read(length)
	
	def getObservation(self):
		numInts = self.getInt()
		numDoubles = self.getInt()		
		obs = Observation(numInts,numDoubles)
		if numInts > 0:
			s = self.recvBuffer.read(numInts*kIntSize)
			obs.intArray = struct.unpack("!%di" % (numInts),s)
		if numDoubles > 0:
			s = self.recvBuffer.read(numDoubles*kDoubleSize)
			obs.doubleArray = struct.unpack("!%dd" % (numDoubles),s)
		return obs

	def getAction(self):
		numInts = self.getInt()
		numDoubles = self.getInt()		
		action = Action(numInts,numDoubles)
		if numInts > 0:
			s = self.recvBuffer.read(numInts*kIntSize)
			action.intArray = struct.unpack("!%di" % (numInts),s)
		if numDoubles > 0:
			s = self.recvBuffer.read(numDoubles*kDoubleSize)
			action.doubleArray = struct.unpack("!%dd" % (numDoubles),s)
		return action

	def getStateKey(self):
		numInts = self.getInt()
		numDoubles = self.getInt()		
		key = State_key(numInts,numDoubles)
		if numInts > 0:
			s = self.recvBuffer.read(numInts*kIntSize)
			key.intArray = struct.unpack("!%di" % (numInts),s)
		if numDoubles > 0:
			s = self.recvBuffer.read(numDoubles*kDoubleSize)
			key.doubleArray = struct.unpack("!%dd" % (numDoubles),s)
		return key
	
	def getRandomSeedKey(self):
		numInts = self.getInt()
		numDoubles = self.getInt()		
		key = Random_seed_key(numInts,numDoubles)
		if numInts > 0:
			s = self.recvBuffer.read(numInts*kIntSize)
			key.intArray = struct.unpack("!%di" % (numInts),s)
		if numDoubles > 0:
			s = self.recvBuffer.read(numDoubles*kDoubleSize)
			key.doubleArray = struct.unpack("!%dd" % (numDoubles),s)
		return key
	
	def putInt(self,value):
		self.sendBuffer.write(struct.pack("!i",value))
	
	def putDouble(self,value):
		self.sendBuffer.write(struct.pack("!d",value))
	
	def putString(self,value):
		if value == None:
			value = ''
		self.putInt(len(value))
		self.sendBuffer.write(value)
	
	def putObservation(self,obs):
		self.putInt(len(obs.intArray))
		self.putInt(len(obs.doubleArray))
		if len(obs.intArray) > 0:
			self.sendBuffer.write(struct.pack("!%di" % (len(obs.intArray)),*(obs.intArray)))
		if len(obs.doubleArray) > 0:
			self.sendBuffer.write(struct.pack("!%dd" % (len(obs.doubleArray)),*(obs.doubleArray)))
	
	def putAction(self,action):
		self.putInt(len(action.intArray))
		self.putInt(len(action.doubleArray))
		if len(action.intArray) > 0:
			self.sendBuffer.write(struct.pack("!%di" % (len(action.intArray)),*(action.intArray)))
		if len(action.doubleArray) > 0:
			self.sendBuffer.write(struct.pack("!%dd" % (len(action.doubleArray)),*(action.doubleArray)))
	
	def putStateKey(self,key):
		self.putInt(len(key.intArray))
		self.putInt(len(key.doubleArray))
		if len(key.intArray) > 0:
			self.sendBuffer.write(struct.pack("!%di" % (len(key.intArray)),*(key.intArray)))
		if len(key.doubleArray) > 0:
			self.sendBuffer.write(struct.pack("!%dd" % (len(key.doubleArray)),*(key.doubleArray)))

	def putRandomSeedKey(self,key):
		self.putInt(len(key.intArray))
		self.putInt(len(key.doubleArray))
		if len(key.intArray) > 0:
			self.sendBuffer.write(struct.pack("!%di" % (len(key.intArray)),*(key.intArray)))
		if len(key.doubleArray) > 0:
			self.sendBuffer.write(struct.pack("!%dd" % (len(key.doubleArray)),*(key.doubleArray)))
	
	def putRewardObservation(self,rewardObservation):
		self.putInt(rewardObservation.terminal);
		self.putDouble(rewardObservation.r);
		self.putObservation(rewardObservation.o);
	
	def sizeOfAction(self,action):
		size = kIntSize * 2
		intSize = 0
		doubleSize = 0
		if action != None:
			if action.intArray != None:
				intSize = kIntSize * len(action.intArray)
			if action.doubleArray != None:
				doubleSize = kDoubleSize * len(action.doubleArray)
		return size + intSize + doubleSize
	
	def sizeOfObservation(self,observation):
		size = kIntSize * 2
		intSize = 0
		doubleSize = 0
		if observation != None:
			if observation.intArray != None:
				intSize = kIntSize * len(observation.intArray)
			if observation.doubleArray != None:
				doubleSize = kDoubleSize * len(observation.doubleArray)
		return size + intSize + doubleSize
	
	def sizeOfRandomSeed(self,key):
		size = kIntSize * 2
		intSize = 0
		doubleSize = 0
		if key != None:
			if key.intArray != None:
				intSize = kIntSize * len(key.intArray)
			if key.doubleArray != None:
				doubleSize = kDoubleSize * len(key.doubleArray)
		return size + intSize + doubleSize
	
	def sizeOfStateKey(self,key):
		size = kIntSize * 2
		intSize = 0
		doubleSize = 0
		if key != None:
			if key.intArray != None:
				intSize = kIntSize * len(key.intArray)
			if key.doubleArray != None:
				doubleSize = kDoubleSize * len(key.doubleArray)
		return size + intSize + doubleSize
	
	def sizeOfRewardObservation(self,reward_observation):
		return kIntSize + kDoubleSize + self.sizeOfObservation(reward_observation.o)
