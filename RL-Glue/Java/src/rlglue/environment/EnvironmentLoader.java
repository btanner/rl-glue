package rlglue.environment;
import rlglue.network.Network;

public class EnvironmentLoader
{
	public static void main(String [] args) throws Exception
	{
		String usage = "java EnvironmentLoader <Environment> -classpath <Path To RLGlue>";

		String envVars = "The following environment variables are used by the environment to control its function:\n" + 
		"RLGLUE_HOST  : If set the environment will use this ip or hostname to connect to rather than " + Network.kDefaultHost + "\n" +
		"RLGLUE_PORT  : If set the environment will use this port to connect on rather than " + Network.kDefaultPort + "\n" +
		"RLGLUE_AUTORECONNECT  : If set the enviroment will reconnect to the glue after an experiment has finished\n";

		if (args.length < 1) {
			System.out.println(usage);
			System.out.println(envVars);
			System.exit(1);
		}

		Environment env = (Environment)Class.forName(args[0]).newInstance();
		ClientEnvironment client = new ClientEnvironment(env);
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
			client.runEnvironmentEventLoop();
			client.close();
		} while (autoReconnect == 1);
	}
}
