import rlglue.*;

public class EnvironmentLoader
{
    public static void main(String [] args) throws Exception
    {
	Environment env = (Environment)Class.forName(args[0]).newInstance();
	ClientEnvironment client = new ClientEnvironment(env);
	boolean autoReconnect = false;

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
	    client.runEnvironmentEventLoop();
	    client.close();
	} while (autoReconnect);
    }
}
