package rlglue.network.agent;

import rlglue.*;
import rlglue.network.*;

public class ClientAgent 
{
    protected static final String kUnknownMessage = "Unknown Message: ";
    protected RLNetwork network;
    protected RLAgentInterface agent;

    public ClientAgent(AgentInterface agent) 
    {
	this.agent = agent;
	network = new rlglue.network.Network();
    }

    protected void onAgentInit() 
    {
	String taskSpec = network.getInputStream().readString();
	agent.agent_init(taskSpec);
    }

    protected void onAgentStart() 
    {
	final Observation observation = network.readObservation();
	final Action action = agent.agent_start(observation);
	network.writeAction(action);
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
	RLAgentType agent = new RLAgentType();
	RLClientAgent client = new RLClientAgent(agent);
	boolean autoReconnect = false;

	do {
	    client.connect(host, port, kRetryTimeout);
	    client.runAgentEventLoop();
	    client.close();
	} while (autoReconnect);
    }
}