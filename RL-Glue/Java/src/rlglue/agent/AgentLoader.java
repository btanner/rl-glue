package rlglue.agent;
import rlglue.network.Network;

public class AgentLoader
{
	public static void main(String [] args) throws Exception
	{
		String usage = "java AgentLoader <Agent> -classpath <Path To RLGlue>";

		String envVars = "The following environment variables are used by the agent to control its function:\n" + 
		"RLGLUE_HOST  : If set the agent will use this ip or hostname to connect to rather than " + Network.kDefaultHost + "\n" + 
		"RLGLUE_PORT  : If set the agent will use this port to connect on rather than " + Network.kDefaultPort + "\n" +
		"RLGLUE_AUTORECONNECT  : If set the agent will reconnect to the glue after an experiment has finished\n";

		if (args.length < 1) {
			System.out.println(usage);
			System.out.println(envVars);
			System.exit(1);
		}

		Agent agent = (Agent)Class.forName(args[0]).newInstance();
		ClientAgent client = new ClientAgent(agent);
		int autoReconnect = 0;

		String host = Network.kDefaultHost;
		int port = Network.kDefaultPort;

		String hostString = System.getenv("RLGLUE_HOST");
		String portString = System.getenv("RLGLUE_PORT");
		String reconnect = System.getenv("RLGLUE_AUTORECONNECT");

		if (hostString != null)
			host = hostString;

		try {
			port = Integer.parseInt(portString);
		}
		catch (Exception e) {
			port = Network.kDefaultPort;
		}

		try {
			autoReconnect = Integer.parseInt(reconnect);
		}
		catch (Exception e) {
			autoReconnect = 0;
		}

		System.out.println("Connecting to " + host + " on port " + port + " with autoreconnect=" + autoReconnect);

		do {
			client.connect(host, port, Network.kRetryTimeout);
			client.runAgentEventLoop();
			client.close();
		} while (autoReconnect == 1);
	}
}
