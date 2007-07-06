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
    protected EnvironmentInterface env;

    public ClientEnvironment(EnvironmentInterface env) 
    {
	this.env = env;
	this.network = new Network();
	this.headerBuffer = ByteBuffer.allocate(8);
	this.byteBuffer = ByteBuffer.allocate(65536);
    }

    protected void onEnvInit()
    {
	String taskSpec = env.env_init();

	byteBuffer.clear();
	byteBuffer.putInt(Network.kEnvInit);
	byteBuffer.putInt(taskSpec.length());
	Network.putString(byteBuffer, taskSpec);
    }

    protected void onEnvStart()
    {
	Observation obs = env.env_start();
	int size = (obs.intArray.length * 4 + 4) + (obs.doubleArray.length * 8 + 4);

	byteBuffer.clear();
	byteBuffer.putInt(Network.kEnvStart);
	byteBuffer.putInt(size);
	Network.putObservation(byteBuffer, obs);	
    }

    protected void onEnvStep()
    {
	Action action = Network.getAction(byteBuffer);
	Reward_observation rewardObservation = env.env_step(action);	
	int sendSize = 4 // terminal
	    + 8 // reward
	    + (rewardObservation.o.intArray.length * 4 + 4) // intArray data + size
	    + (rewardObservation.o.doubleArray.length * 8 + 4); // doubleArray data + size

	byteBuffer.clear();
	byteBuffer.putInt(Network.kEnvStep);
	byteBuffer.putInt(sendSize);

	byteBuffer.putInt(rewardObservation.terminal);
	byteBuffer.putDouble(rewardObservation.r);
	Network.putObservation(byteBuffer, rewardObservation.o);
    }

    protected void onEnvCleanup()
    {
	byteBuffer.clear();
	byteBuffer.putInt(Network.kEnvCleanup);
	byteBuffer.putInt(0);
    }

    protected void onEnvMessage() throws UnsupportedEncodingException
    {
	String recvMessage = Network.getString(byteBuffer);
	String sendMessage = env.env_message(recvMessage);

	byteBuffer.clear();
	byteBuffer.putInt(Network.kEnvMessage);
	Network.putString(byteBuffer, sendMessage);
    }

    public void connect(String host, int port, int timeout) throws Exception
    {	
	network.connect(host, port, timeout);

	byteBuffer.clear();
	byteBuffer.putInt(Network.kEnvironmentConnection);
	byteBuffer.putInt(0); // No body to this packet
	byteBuffer.flip();
	network.send(byteBuffer);
    }

    public void close() throws IOException
    {
	byteBuffer.clear();
	network.close();
    }

    public void runEnvironmentEventLoop() throws Exception
    {
	int envState = 0;
	int dataSize = 0;
	int recvSize = 0;

	do {
	    headerBuffer.clear();
	    recvSize = 0;
	    while (recvSize < 8) 
		recvSize += network.recv(headerBuffer);
	    headerBuffer.flip();

	    envState = headerBuffer.getInt();
	    dataSize = headerBuffer.getInt();

	    byteBuffer.clear();    
	    recvSize = 0;
	    while (recvSize < dataSize) 
		recvSize += network.recv(byteBuffer);
	    byteBuffer.flip();
	    
	    switch(envState) {
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
		System.err.println(kUnknownMessage + envState);
		System.exit(1);
		break;
	    };

	    byteBuffer.flip();
	    network.send(byteBuffer);

	} while (envState != Network.kEnvCleanup);
    }
}
