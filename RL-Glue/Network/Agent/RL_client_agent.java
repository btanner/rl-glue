public class RL_client_agent 
{
    protected static final String kUnknownMessage = "Unknown Message: ";
    protected RL_netlib netlib;
    protected AgentType agent;

    public RL_client_agent() 
    {
	agent = new AgentType();
    }

    protected void onAgentInit() 
    {
	String taskSpec = netlib.getInputStream().readString();
	agent.agent_init(taskSpec);
    }

    protected void onAgentStart() 
    {
	final RLAbstractType observation = netlib.readAbstractType();
	final RLAbstractType action = agent.agent_start(observation);
	netlib.writeAbstractType(action);
    }

    protected void onAgentStep()
    {
	final double reward = netlib.readDouble();
	final RLAbstractType observation = netlib.readAbstractType();
	final RLAbstractType action = agent.agent_step(reward, observation);
	netlib.writeAbstractType(action);
    }

    protected void onAgentEnd()
    {
	final double reward = netlib.readDouble();
	agent.agent_end();
    }

    protected void onAgentCleanup()
    {
	agent.agent_cleanup();
    }

    protected void onAgentFreeze() 
    {
	agent.agent_freeze();
    }

    protected void onAgentMessage()
    {
	final String recvMessage = netlib.readString();
	final String sendMessage = agent.agent_message(recvMessage);

	/* this is not finished */
    }

    /* Should fix this and the RL_netlib constructor to throw more specific exceptions */
    protected void connect(String host, int port, int timeout) throws Exception
    {	
	netlib = new RL_netlib(host, port, timeout);
    }

    protected void runAgentEventLoop(Socket theSocket) 
    {
	int agentState = 0;

	InputDataStream input = theSocket.getInputDataStream();
	agentState = input.readInt();

	do {

	    switch(agentState) {
	    case kAgentInit:
		onAgentInit();
		break;
		
	    case kAgentStart:
		onAgentStart();
		break;
		
	    case kAgentStep:
		onAgentStep();
		break;
		
	    case kAgentEnd:
		onAgentEnd();
		break;
		
	    case kAgentCleanup:
		onAgentCleanup();
		break;
		
	    case kAgentFreeze:
		onAgentFreeze();
		break;
		
	    case kAgentMessage:
		onAgentMessage();
		break;
		
	    default:
		System.err.println(kUnknownMessage + agentState + "\n");
		System.exit(1);
		break;
	    };

	} while (agentState != kAgentCleanup);
    }

    protected void close() 
    {
	netlib.close();
    }

    public static void main(String [] args) 
    {
	RL_client_agent client = new RL_client_agent();
	boolean autoReconnect = false;

	do {
	    client.connect(host, port, kRetryTimeout);
	    client.runAgentEventLoop();
	    client.close();
	} while (autoReconnect);
    }
}