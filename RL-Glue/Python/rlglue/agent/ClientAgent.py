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

class ClientAgent:
	kUnknownMessage = "Unknown Message: "
	network = None
	agent = None

	# (agent) -> void
	def __init__(self, agent):
		self.agent = agent
		self.network = Network.Network()

	# () -> void
	def onAgentInit(self):
		taskSpec = self.network.getString()
		self.agent.agent_init(taskSpec)
		self.network.clearSendBuffer()
		self.network.putInt(Network.kAgentInit)
		self.network.putInt(0) # No data following this header

	# () -> void
	def onAgentStart(self):
		observation = self.network.getObservation()
		action = self.agent.agent_start(observation)
		size = self.network.sizeOfAction(action)
		self.network.clearSendBuffer()
		self.network.putInt(Network.kAgentStart)
		self.network.putInt(size)
		self.network.putAction(action)

	# () -> void
	def onAgentStep(self):
		reward = self.network.getDouble()
		observation = self.network.getObservation()
		action = self.agent.agent_step(reward, observation)
		size = self.network.sizeOfAction(action)
		self.network.clearSendBuffer()
		self.network.putInt(Network.kAgentStep)
		self.network.putInt(size)
		self.network.putAction(action)

	# () -> void
	def onAgentEnd(self):
		reward = self.network.getDouble()
		self.agent.agent_end(reward)
		self.network.clearSendBuffer()
		self.network.putInt(Network.kAgentEnd)
		self.network.putInt(0) # No data in this packet

	# () -> void
	def onAgentCleanup(self):
		self.agent.agent_cleanup()
		self.network.clearSendBuffer()
		self.network.putInt(Network.kAgentCleanup)
		self.network.putInt(0) # No data in this packet

	# () -> void
	def onAgentFreeze(self):
		self.agent.agent_freeze()
		self.network.clearSendBuffer()
		self.network.putInt(Network.kAgentFreeze)
		self.network.putInt(0) # No data in this packet

	# () -> void
	def onAgentMessage(self):
		message = self.network.getString()
		reply = self.agent.agent_message(message)
		self.network.clearSendBuffer()
		self.network.putInt(Network.kAgentMessage)
		if reply == None:
			self.network.putInt(0)
		else:
			self.network.putInt(len(reply))
			self.network.putString(reply)

	# (string, int, int) -> void
	def connect(self, host, port, timeout):
		self.network.connect(host, port, timeout);
		self.network.clearSendBuffer()
		self.network.putInt(Network.kAgentConnection)
		self.network.putInt(0) # No body to this packet
		self.network.send()

	# () -> void
	def close(self):
		self.network.close()

	# () -> void
	def runAgentEventLoop(self):
		agentState = 0
		dataSize = 0
		recvSize = 0
		remaining = 0

		while agentState != Network.kRLTerm:
			self.network.clearRecvBuffer();
			recvSize = self.network.recv(8) - 8; # We may have received the header and part of the payload
											# We need to keep track of how much of the payload was recv'd
			agentState = self.network.getInt()
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
				Network.kAgentInit: lambda self: self.onAgentInit(),
				Network.kAgentStart: lambda self: self.onAgentStart(),
				Network.kAgentStep: lambda self: self.onAgentStep(),
				Network.kAgentEnd: lambda self: self.onAgentEnd(),
				Network.kAgentCleanup: lambda self: self.onAgentCleanup(),
				Network.kAgentFreeze: lambda self: self.onAgentFreeze(),
				Network.kAgentMessage: lambda self: self.onAgentMessage() }
			if agentState in switch:
				switch[agentState](self)
			elif agentState == Network.kRLTerm:
				pass
			else:
				sys.stderr.write(Network.kUnknownMessage % (str(agentState)))
				sys.exit(1)

			self.network.send()
