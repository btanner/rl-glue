package rlglue.network.environment;

import java.io.*;
import java.nio.ByteBuffer;

import rlglue.*;
import rlglue.network.*;

public class ClientEnvironment 
{
    protected static final String kUnknownMessage = "Unknown Message: ";
    protected Network network;
    protected ByteBuffer headerBuffer;
    protected ByteBuffer byteBuffer;
    protected EnvironmentInterface environment;

    public ClientEnvironment(EnvironmentInterface env) 
    {
	this.environment = env;
	this.network = new Network();
	this.headerBuffer = ByteBuffer.allocate(8);
	this.byteBuffer = ByteBuffer.allocate(65536);
    }

    protected void onEnvInit()
    {
       	String taskSpec = environment.env_init();

	byteBuffer.clear();
	byteBuffer.putInt(Network.kEnvInit);
	byteBuffer.putInt(taskSpec.length());
	Network.putString(byteBuffer, taskSpec);
    }

    protected void onEnvStart()
    {
    }

    protected void onEnvStep()
    {
    }

    protected void onEnvCleanup()
    {
    }

    protected void onEnvMessage() throws UnsupportedEncodingException
    {
    }

    public void connect(String host, int port, int timeout) throws Exception
    {	
	network.connect(host, port, timeout);

	byteBuffer.clear();
	byteBuffer.putInt(Network.kEnvConnection);
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
	int envState = 0;
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
	    case Network.kEnvInit:
		onEnvInit();
		break;
		
	    case Network.kEnvStart:
		onEnvStart();
		break;
		
	    case Network.kEnvStep:
		onEnvStep();
		break;
				
	    case Network.kEnvCleanup:
		onEnvCleanup();
		break;
			
	    case Network.kEnvMessage:
		onEnvMessage();
		break;
		
	    default:
		System.err.println(kUnknownMessage + agentState);
		System.exit(1);
		break;
	    };

	    byteBuffer.flip();
	    network.send(byteBuffer);

	} while (envState != Network.kEnvCleanup);
    }
}
