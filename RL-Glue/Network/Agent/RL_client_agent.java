public class RL_client_agent 
{
    public static final String kLocalHost = "127.0.0.1";
    public static final int kDefaultPort  = 4096;
    public static final int kRetryTimeout = 10;

    protected void runAgentEventLoop(Socket theSocket) 
    {
	int agentState = 0;

	do {
	} while (agentState != kAgentCleanup);
    }

    public static void main(String [] args) 
    {
	RL_client_agent agent = new RL_client_agent();
	boolean autoReconnect = false;

	do {
	    agent.connect(host, port, kRetryTimeout);
	    agent.runAgentEventLoop();
	    agent.close();
	} while (autoReconnect);
    }
}