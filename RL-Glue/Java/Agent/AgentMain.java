import rlglue.*;
import rlglue.network.*;
import rlglue.network.agent.*;

public class AgentMain
{
    public static void main(String [] args) throws Exception
    {
	AgentInterface agent = (AgentInterface)Class.forName(args[0]).newInstance();
	ClientAgent client = new ClientAgent(agent);
	boolean autoReconnect = false;

	String host = Network.kDefaultHost;
	int port = Network.kDefaultPort;

	do {
	    client.connect(host, port, Network.kRetryTimeout);
	    client.runAgentEventLoop();
	    client.close();
	} while (autoReconnect);
    }
}
