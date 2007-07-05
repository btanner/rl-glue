package rlglue.network.agent;

import java.io.*;
import java.nio.ByteBuffer;

import rlglue.*;
import rlglue.network.*;

public class ClientAgent 
{
    protected static final String kUnknownMessage = "Unknown Message: ";
    protected Network network;
    protected ByteBuffer headerBuffer;
    protected ByteBuffer byteBuffer;
    protected AgentInterface agent;

    public ClientAgent(AgentInterface agent) 
    {
	this.agent = agent;
	this.network = new Network();
	this.headerBuffer = ByteBuffer.allocate(8);
	this.byteBuffer = ByteBuffer.allocate(65536);
    }

    protected void onAgentInit()
    {
	String taskSpec = Network.getString(byteBuffer);

       	agent.agent_init(taskSpec);

	byteBuffer.clear();
	byteBuffer.putInt(Network.kAgentInit);
	byteBuffer.putInt(0);
    }

    protected void onAgentStart()
    {
	Observation observation = Network.getObservation(byteBuffer);
	Action action = agent.agent_start(observation);

	/* In java ints and doubles are 32 bits (4 bytes) and 64 bits (8 bytes) respectively */
	int size = (action.intArray.length * 4 + 4) + (action.doubleArray.length * 8 + 4); 

	byteBuffer.clear();
	byteBuffer.putInt(Network.kAgentStart);
	byteBuffer.putInt(size);
	
	Network.putAction(byteBuffer, action);
    }

    protected void onAgentStep()
    {
	double reward = byteBuffer.getDouble();
	Observation observation = Network.getObservation(byteBuffer);
	Action action = agent.agent_step(reward, observation);
	
	/* In Java ints and doubles are 32 bits (4 bytes) and 64 bits (8 bytes) respectively */
	int size = (action.intArray.length * 4 + 4) + (action.doubleArray.length * 8 + 4); 

	byteBuffer.clear();
	byteBuffer.putInt(Network.kAgentStep);
	byteBuffer.putInt(size);
	
	Network.putAction(byteBuffer, action);
    }

    protected void onAgentEnd()
    {
	int size = byteBuffer.getInt();
	double reward = byteBuffer.getDouble();

	agent.agent_end(reward);
	
	byteBuffer.clear();
	byteBuffer.putInt(Network.kAgentEnd);
	byteBuffer.putInt(0);
    }

    protected void onAgentCleanup()
    {
	agent.agent_cleanup();

	byteBuffer.clear();
	byteBuffer.putInt(Network.kAgentCleanup);
	byteBuffer.putInt(0);
    }

    protected void onAgentFreeze()
    {
	agent.agent_freeze();

	byteBuffer.clear();
	byteBuffer.putInt(Network.kAgentFreeze);
	byteBuffer.putInt(0);
    }

    protected void onAgentMessage() throws UnsupportedEncodingException
    {
	String message = Network.getString(byteBuffer);
	String reply = agent.agent_message(message);

	byteBuffer.clear();
	byteBuffer.putInt(Network.kAgentMessage);
	byteBuffer.putInt(reply.length() + 4); // Sizeof int == 4 
	Network.putString(byteBuffer, reply);  // send the length of the string and the string.
    }

    public void connect(String host, int port, int timeout) throws Exception
    {	
	network.connect(host, port, timeout);

	byteBuffer.clear();
	byteBuffer.putInt(Network.kAgentConnection);
	byteBuffer.putInt(0); // No body to this packet
	byteBuffer.flip();
	network.send(byteBuffer);
    }

    public void close() throws IOException
    {
	byteBuffer.clear();
	network.close();
    }

    public void runAgentEventLoop() throws Exception
    {
	int agentState = 0;
	int dataSize = 0;
	int recvSize = 0;

	do {
	    headerBuffer.clear();

	    recvSize = 0;
	    while (recvSize < 8) 
	    {
		recvSize += network.recv(headerBuffer);
	    }

	    headerBuffer.flip();

	    agentState = headerBuffer.getInt();
	    dataSize = headerBuffer.getInt();

	    byteBuffer.clear();
	    
	    recvSize = 0;
	    while (recvSize < dataSize) 
	    {
		recvSize += network.recv(byteBuffer);
	    }

	    byteBuffer.flip();
	    
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

	    byteBuffer.flip();
	    network.send(byteBuffer);

	} while (agentState != Network.kAgentCleanup);
    }
}
