import rlglue.*;

public class AgentMain
{
    public static void main(String [] args) throws Exception
    {
	Agent agent = (Agent)Class.forName(args[0]).newInstance();
	ClientAgent client = new ClientAgent(agent);
	int autoReconnect = 0;

	String host = Network.kDefaultHost;
	int port = Network.kDefaultPort;

	String hostString = System.getenv("RLGLUE_HOST");
	String portString = System.getenv("RLGLUE_PORT");
	String reconnect = System.getenv("RLGLUE_AUTORECONNECT");

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
