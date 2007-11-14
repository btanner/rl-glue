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

import sys

import rlglue.network.Network as Network
from rlglue.types import Action
from rlglue.types import Observation

class ClientEnvironment:
	kUnknownMessage = "Unknown Message: "
	network = None
	env = None

	# (agent) -> void
	def __init__(self, environment):
		self.env = environment
		self.network = Network.Network()

	# () -> void
	def onEnvInit(self):
		taskSpec = self.env.env_init()
		self.network.clearSendBuffer()
		self.network.putInt(Network.kEnvInit)
		self.network.putInt(len(taskSpec) + 4) # Also including the length put in by putString
		self.network.putString(taskSpec)

	# () -> void
	def onEnvStart(self):
		observation = self.env.env_start()
		size = self.network.sizeOfObservation(observation)
		self.network.clearSendBuffer()
		self.network.putInt(Network.kEnvStart)
		self.network.putInt(size)
		self.network.putObservation(observation)

	# () -> void
	def onEnvStep(self):
		action = self.network.getAction()
		reward_observation = self.env.env_step(action)
		size = self.network.sizeOfRewardObservation(reward_observation)
		self.network.clearSendBuffer()
		self.network.putInt(Network.kEnvStep)
		self.network.putInt(size)
		self.network.putRewardObservation(reward_observation)

	# () -> void
	def onEnvCleanup(self):
		self.env.env_cleanup()
		self.network.clearSendBuffer()
		self.network.putInt(Network.kEnvCleanup)
		self.network.putInt(0) # No data in this packet

	# () -> void
	def onEnvGetRandomSeed(self):
		key = self.env.env_get_random_seed()
		self.network.clearSendBuffer()
		self.network.putInt(Network.kEnvGetRandomSeed)
		self.network.putInt(self.network.sizeOfRandomSeed(key))
		self.network.putRandomSeedKey(key)

	# () -> void
	def onEnvGetState(self):
		key = self.env.env_get_state()
		self.network.clearSendBuffer()
		self.network.putInt(Network.kEnvGetState)
		self.network.putInt(self.network.sizeOfStateKey(key))
		self.network.putStateKey(key)

	# () -> void
	def onEnvSetRandomSeed(self):
		key = network.getRandomSeedKey()
		self.env.env_set_random_seed(key)
		self.network.clearSendBuffer()
		self.network.putInt(Network.kEnvSetRandomSeed)
		self.network.putInt(0) # No data in this packet

	# () -> void
	def onEnvSetState(self):
		key = network.getStateKey()
		self.env.env_set_state(key)
		self.network.clearSendBuffer()
		self.network.putInt(Network.kEnvSetState)
		self.network.putInt(0) # No data in this packet

	# () -> void
	def onEnvMessage(self):
		message = self.network.getString()
		reply = self.env.env_message(message)
		self.network.clearSendBuffer()
		self.network.putInt(Network.kEnvMessage)
		if reply == None:
			self.network.putInt(0)
		else:
			self.network.putInt(len(reply))
			self.network.putString(reply)

	# (string, int, int) -> void
	def connect(self, host, port, timeout):
		self.network.connect(host, port, timeout);
		self.network.clearSendBuffer()
		self.network.putInt(Network.kEnvironmentConnection)
		self.network.putInt(0) # No body to this packet
		self.network.send()

	# () -> void
	def close(self):
		self.network.close()

	# () -> void
	def runEnvironmentEventLoop(self):
		envState = 0
		dataSize = 0
		recvSize = 0
		remaining = 0

		while envState != Network.kRLTerm:
			self.network.clearRecvBuffer();
			recvSize = self.network.recv(8) - 8; # We may have received the header and part of the payload
											# We need to keep track of how much of the payload was recv'd
			envState = self.network.getInt()
			dataSize = self.network.getInt()
			
			remaining = dataSize - recvSize;
			if (remaining < 0):
				print("Remaining was less than 0!")
				remaining = 0

			amountReceived = self.network.recv(remaining)
			
			# Already read the header, discard it
			self.network.getInt()
			self.network.getInt()

			switch = {
				Network.kEnvInit: lambda self: self.onEnvInit(),
				Network.kEnvStart: lambda self: self.onEnvStart(),
				Network.kEnvStep: lambda self: self.onEnvStep(),
				Network.kEnvCleanup: lambda self: self.onEnvCleanup(),
				Network.kEnvGetRandomSeed: lambda self: self.onEnvGetRandomSeed(),
				Network.kEnvGetState: lambda self: self.onEnvGetState(),
				Network.kEnvSetRandomSeed: lambda self: self.onEnvSetRandomSeed(),
				Network.kEnvSetState: lambda self: self.onEnvSetState(),
				Network.kEnvMessage: lambda self: self.onEnvMessage() }
			if envState in switch:
				switch[envState](self)
			elif envState == Network.kRLTerm:
				pass
			else:
				sys.stderr.write(Network.kUnknownMessage % (str(envState)))
				sys.exit(1)

			self.network.send()
