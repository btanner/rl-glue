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

	print "Connecting to " + host + " on port " + str(port) + " with autoreconnect=" + str(autoReconnect)

	while True:
		client.connect(host, port, Network.kRetryTimeout)
		client.runAgentEventLoop()
		client.close()
		if autoReconnect == 0:
			break

main()