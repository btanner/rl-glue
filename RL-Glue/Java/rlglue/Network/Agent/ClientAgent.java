package rlglue.network.agent;

import java.io.DataInputStream;

import rlglue.*;
import rlglue.network.*;

public class ClientAgent 
{
    protected static final String kUnknownMessage = "Unknown Message: ";
    protected Network network;
    protected AgentInterface agent;

    public ClientAgent(AgentInterface agent) 
    {
	this.agent = agent;
    }

    protected void onAgentInit() 
    {
	final int taskSpecLength = network.getDataInputStream().readInt();
	final String taskSpec = new String(network.read(taskSpecLength));
	
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
	final double reward = network.getDataInputStream().readDouble();
	final Observation observation = network.readObservation();
	final Action action = agent.agent_step(reward, observation);
	network.writeAction(action);
    }

    protected void onAgentEnd()
    {
	final double reward = network.getDataInputStream().readDouble();
	agent.agent_end(reward);
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
	final int recvMsgLength = network.getDataInputStream().readInt();
	final String recvMessage = new String(network.read(recvMsgLength));
	final String sendMessage = agent.agent_message(recvMessage);
	network.getDataOutputStream().writeInt(sendMessage.length());
	network.getDataOutputStream().writeBytes(sendMessage);
    }

    protected void connect(String host, int port, int timeout)
    {	
	network = new Network(host, port, timeout);
    }

    protected void runAgentEventLoop() 
    {
	int agentState = 0;

	DataInputStream input = network.getDataInputStream();
	agentState = input.readInt();

	do {

	    switch(agentState) {
	    case Network.kAgentInit:
		onAgentInit();
		break;
		
	    case Network.kAgentStart:
		onAgentStart();
		break;
		
	    case Network.kAgentStep:
		onAgentStep();
		break;
		
	    case Network.kAgentEnd:
		onAgentEnd();
		break;
		
	    case Network.kAgentCleanup:
		onAgentCleanup();
		break;
		
	    case Network.kAgentFreeze:
		onAgentFreeze();
		break;
		
	    case Network.kAgentMessage:
		onAgentMessage();
		break;
		
	    default:
		System.err.println(kUnknownMessage + agentState + "\n");
		System.exit(1);
		break;
	    };

	} while (agentState != Network.kAgentCleanup);
    }

    protected void close() 
    {
	network.close();
    }

    public static void main(String [] args) 
    {
	SarsaAgent agent = new SarsaAgent();
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
