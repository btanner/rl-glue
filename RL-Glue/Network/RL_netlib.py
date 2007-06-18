import socket
import struct
import array
import time
import sys
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
kAgentFreeze  = 30
kAgentMessage = 31

kEnvInit          = 9
kEnvStart         = 10
kEnvStep          = 11
kEnvCleanup       = 12
kEnvSetState      = 13
kEnvSetRandomSeed = 14
kEnvGetState      = 15
kEnvGetRandomSeed = 16
kEnvMessage       = 32

kRLInit           = 17
kRLStart          = 18
kRLStep           = 19
kRLCleanup        = 20
kRLReturn         = 21
kRLNumSteps       = 22
kRLNumEpisodes    = 23
kRLEpisode        = 24
kRLSetState       = 25
kRLSetRandomSeed  = 26
kRLGetState       = 27
kRLGetRandomSeed  = 28
kRLFreeze         = 29
kRLAgentMessage   = 33
kRLEnvMessage     = 34

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
	
	def sendString(self, string):
		sys.stderr.write("sending string %s, length is %d\n" % (string, len(string)))
		
		self._sendInt(4 + len(string))
		self._sendInt(len(string))
		self.socket.sendall(string)
	
	def _sendInt(self, data):
		packet = struct.pack('i',socket.htonl(data))
		self.socket.sendall(packet)
	
	def _sendDouble(self,data):
		packet = struct.pack('d',data)
		self.socket.sendall(packet)
	
	def _sendInts(self, data):
		packet = array.array('l',data)
		self.socket.sendall(packet)

	def _sendDoubles(self,data):
		packet = array.array('d',data)
		self.socket.sendall(packet)
	
	def recvString(self):
		s = ''
		length = self._recvInt()
		while len(s) < length:
			s += self.socket.recv(length)
		return s
	
	def _recvInt(self):
		s = ''
		while len(s) < 4:
			s += self.socket.recv(4)
		return struct.unpack('i',s)[0]
		
	def _recvDouble(self):
		s = ''
		while len(s) < 8:
			s += self.socket.recv(8)
		return struct.unpack('d',s)[0]
	
	def _recvInts(self,num):
		s = ''
		while len(s) < num*4:
			s += self.socket.recv(num*4)
		return struct.unpack("%di" % (num),s)

	def _recvDoubles(self,num):
		s = ''
		while len(s) < num*8:
			s += self.socket.recv(num*8)
		return struct.unpack("%dd" % (num),s)

	def sendADT(self, data):
		self._sendInt(8 + 4*data.numInts + 8*data.numDoubles)
		self._sendInt(data.numInts)
		self._sendInt(data.numDoubles)
		if data.numInts > 0:
			self._sendInts(data.intArray)
		if data.numDoubles > 0:
			self._sendData(data.doubleArray)

	def recvADT(self):
		data = RL_abstract_type()
		size = self._recvInt()
		data.numInts = self._recvInt()
		data.numDoubles = self._recvInt()
		if data.numInts > 0:
			data.intArray = self._recvInts(data.numInts)
		if data.numDoubles > 0:
			data.doubleArray = self._recvDoubles(data.numDoubles)
		return data

	def sendRewardObservation(self, ro):
		self._sendInt(12)
		self._sendInt(ro.terminal)
		self._sendDouble(ro.r)
		self.sendADT(ro.o)

	def recvRewardObservation(self):
		ro = Reward_observation()
		size = self._recvInt()
		ro.r = self._recvDouble()
		ro.o = self.recvADT()
		return ro

	def recvDouble(self):
		size = self._recvInt()
		return self._recvDouble()

	def recvInt(self):
		size = self._recvInt()
		return self._recvInt()

	def sendInt(self, i):
		self._sendInt(4)
		self._sendInt(i)

	def recvObservationAction(self):
		oa = Observation_action()
		oa.o = self.recvADT()
		oa.a = self.recvADT()
		return oa

	def recvRewardObservationActionTerm(self):
		roat = Reward_observation_action_terminal()
		size = self._recvInt()
		roat.terminal = self._recvInt()
		roat.r = self._recvDouble()
		roat.o = self.recvADT()
		roat.a = self.recvADT()

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