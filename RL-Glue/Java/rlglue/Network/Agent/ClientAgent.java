package rlglue.network.agent;

import java.io.*;

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
	this.network = new Network();
    }

    protected void onAgentInit()
    {
	int target = Network.kAgentInit;
	int size = 0;

	String taskSpec = network.getString();
	/*printBuffer(network);*/

	System.out.println(taskSpec);
       	agent.agent_init(taskSpec);

	network.clearBuffer();
	/*printBuffer(network);*/

	network.putInt(target);
	/*printBuffer(network);*/

	network.putInt(size); // No data in this reply
	/*printBuffer(network);*/
    }

    protected void onAgentStart()
    {
	int target = Network.kAgentStart;
	int size = network.getInt();
	/*printBuffer(network);*/

	Observation observation = network.getObservation();
	/*printBuffer(network);*/

	Action action = agent.agent_start(observation);
	
	/* In java ints and doubles are 32 bits (4 bytes) and 64 bits (8 bytes) respectively */
	size = (action.intArray.length * 4 + 4) + (action.doubleArray.length * 8 + 4); 

	network.clearBuffer();
	/*printBuffer(network);*/

	network.putInt(target);
	/*printBuffer(network);*/

	network.putInt(size);
	/*printBuffer(network);*/

	network.putAction(action);
	/*printBuffer(network);*/
    }

    protected void onAgentStep()
    {
	int target = Network.kAgentStep;
	/*printBuffer(network);*/

	int size = network.getInt();
	/*printBuffer(network);*/

	double reward = network.getDouble();
	/*printBuffer(network);*/

	Observation observation = network.getObservation();
	/*printBuffer(network);*/

	Action action = agent.agent_step(reward, observation);
	
	/* In Java ints and doubles are 32 bits (4 bytes) and 64 bits (8 bytes) respectively */
	size = (action.intArray.length * 4 + 4) + (action.doubleArray.length * 8 + 4); 

	network.clearBuffer();
	/*printBuffer(network);*/

	network.putInt(target);
	/*printBuffer(network);*/

	network.putInt(size);
	/*printBuffer(network);*/

	network.putAction(action);
	/*printBuffer(network);*/
    }

    protected void onAgentEnd()
    {
	int target = Network.kAgentEnd;
	int size = network.getInt();

	double reward = network.getDouble();

	agent.agent_end(reward);
	
	size = 0;
	network.clearBuffer();
	network.putInt(target);
	network.putInt(size);
    }

    protected void onAgentCleanup()
    {
	int target = Network.kAgentCleanup;
	int size = network.getInt();

	agent.agent_cleanup();

	size = 0;
	network.clearBuffer();
	network.putInt(target);
	network.putInt(size);
    }

    protected void onAgentFreeze()
    {
	int target = Network.kAgentFreeze;
	int size = network.getInt();

	agent.agent_freeze();

	size = 0;
	network.clearBuffer();
	network.putInt(target);
	network.putInt(size);
    }

    protected void onAgentMessage() throws UnsupportedEncodingException
    {
	int target = Network.kAgentMessage;
	int size = network.getInt();

	String message = network.getString();
	String reply = agent.agent_message(message);

	network.clearBuffer();
	network.putInt(target);
	network.putInt(reply.length() + 4); // Sizeof int == 4 
	network.putString(reply); // send the length of the string and the string.
    }

    public void connect(String host, int port, int timeout) throws Exception
    {	
	int target = Network.kAgentConnection;
	int size = 0;

	network.connect(host, port, timeout);

	network.clearBuffer();
	network.putInt(target);
	network.putInt(size); // No body to this packet
	network.flipBuffer();
	network.sendBuffer();
    }

    public void close() throws IOException
    {
	network.clearBuffer();
	network.close();
    }

    public void printBuffer(Network network)
    {
	System.out.println("Position = " + network.getBuffer().position() +
			   " Limit = " + network.getBuffer().limit() + 
			   " Capacity = " + network.getBuffer().capacity());
    }

    public void runAgentEventLoop() throws Exception
    {
	int agentState = 0;
	int dataSize = 0;

	do {
	    network.clearBuffer();
	    network.recvBuffer();
	    network.flipBuffer();

	    /*printBuffer(network);*/

	    agentState = network.getInt();
	    dataSize = network.getInt();

	    System.out.println("agentState = " + agentState);

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
		System.err.println(kUnknownMessage + agentState);
		System.exit(1);
		break;
	    };

	    network.flipBuffer();
	    network.sendBuffer();

	} while (agentState != Network.kAgentCleanup);
    }
}
