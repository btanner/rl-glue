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
import os
import rlglue.network.Network as Network
from ClientAgent import ClientAgent

def main():
	usage = "PYTHONPATH=<Path to RLGlue> python AgentLoader <Agent>";

	envVars = "The following environment variables are used by the agent to control its function:\n" + \
	"RLGLUE_HOST  : If set the agent will use this ip or hostname to connect to rather than " + Network.kLocalHost + "\n" + \
	"RLGLUE_PORT  : If set the agent will use this port to connect on rather than " + str(Network.kDefaultPort) + "\n" + \
	"RLGLUE_AUTORECONNECT  : If set the agent will reconnect to the glue after an experiment has finished\n"

	if (len(sys.argv) < 2):
		print usage
		print envVars
		sys.exit(1)
	
	agentModule = __import__(sys.argv[1])
	agentClass = getattr(agentModule,sys.argv[1])
	agent = agentClass()

	client = ClientAgent(agent)
	autoReconnect = 0

	host = Network.kLocalHost
	port = Network.kDefaultPort

	hostString = os.getenv("RLGLUE_HOST")
	portString = os.getenv("RLGLUE_PORT")
	reconnect = os.getenv("RLGLUE_AUTORECONNECT")

	if (hostString != None):
		host = hostString

	try:
		port = int(portString)
	except TypeError:
		port = Network.kDefaultPort

	try:
		autoReconnect = int(reconnect)
	except TypeError:
		autoReconnect = 0

	print "Connecting to " + host + " on port " + str(port) + "...",
	sys.stdout.flush()

	while True:
		client.connect(host, port, Network.kRetryTimeout)
		print "Connected"
		client.runAgentEventLoop()
		client.close()
		if autoReconnect == 0:
			break

main()