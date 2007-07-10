import rlglue.*;

public class EnvironmentMain
{
    public static void main(String [] args) throws Exception
    {
	Environment env = (Environment)Class.forName(args[0]).newInstance();
	ClientEnvironment client = new ClientEnvironment(env);
	boolean autoReconnect = false;

	String host = Network.kDefaultHost;
	int port = Network.kDefaultPort;

	do {
	    client.connect(host, port, Network.kRetryTimeout);
	    client.runEnvironmentEventLoop();
	    client.close();
	} while (autoReconnect);
    }
}
