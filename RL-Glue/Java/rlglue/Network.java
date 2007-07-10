package rlglue;

import java.io.IOException;
import java.io.UnsupportedEncodingException;
import java.net.InetSocketAddress;
import java.nio.ByteBuffer;
import java.nio.channels.SocketChannel;

public class Network
{
    public static final int kExperimentConnection  = 1;
    public static final int kAgentConnection       = 2;
    public static final int kEnvironmentConnection = 3;

    public static final int kAgentInit    = 4;
    public static final int kAgentStart   = 5;
    public static final int kAgentStep    = 6;
    public static final int kAgentEnd     = 7;
    public static final int kAgentCleanup = 8;
    public static final int kAgentFreeze  = 9;
    public static final int kAgentMessage = 10;

    public static final int kEnvInit          = 11;
    public static final int kEnvStart         = 12;
    public static final int kEnvStep          = 13;
    public static final int kEnvCleanup       = 14;
    public static final int kEnvSetState      = 15;
    public static final int kEnvSetRandomSeed = 16;
    public static final int kEnvGetState      = 17;
    public static final int kEnvGetRandomSeed = 18;
    public static final int kEnvMessage       = 19;

    public static final int kRLInit          = 20;
    public static final int kRLStart         = 21;
    public static final int kRLStep          = 22;
    public static final int kRLCleanup       = 23;
    public static final int kRLReturn        = 24;
    public static final int kRLNumSteps      = 25;
    public static final int kRLNumEpisodes   = 26;
    public static final int kRLEpisode       = 27;
    public static final int kRLSetState      = 28;
    public static final int kRLSetRandomSeed = 29;
    public static final int kRLGetState      = 30;
    public static final int kRLGetRandomSeed = 31;
    public static final int kRLFreeze        = 32;
    public static final int kRLAgentMessage  = 33;
    public static final int kRLEnvMessage    = 34;

    public static final String kDefaultHost = "127.0.0.1";
    public static final int kDefaultPort = 4096;
    public static final int kRetryTimeout = 10;

    protected SocketChannel socketChannel;

    public Network()
    {
    }

    public void connect(String host, int port, int retryTimeout) throws IOException
    {
	InetSocketAddress address = new InetSocketAddress(host, port);
	socketChannel = SocketChannel.open();
	socketChannel.configureBlocking(true);
	socketChannel.connect(address);
    }

    public void close() throws IOException
    {
	socketChannel.close();
    }

    public int send(ByteBuffer byteBuffer) throws IOException
    {
	return socketChannel.write(byteBuffer);
    }

    public int recv(ByteBuffer byteBuffer, int size) throws IOException
    {
	int recvSize = 0;
	while (recvSize < size)
	{
	    recvSize += socketChannel.read(byteBuffer);
	}
	return recvSize;
    }
    
    public static String getString(ByteBuffer byteBuffer) throws UnsupportedEncodingException
    {
	int length = byteBuffer.getInt();
	byte [] byteString = new byte[length];
	byteBuffer.get(byteString);
	return new String(byteString, "UTF-8").trim();
    }

    public static Observation getObservation(ByteBuffer byteBuffer)
    {
	final int numInts = byteBuffer.getInt();
	final int numDoubles = byteBuffer.getInt();

	Observation obs = new Observation(numInts, numDoubles);	
	for (int i = 0; i < numInts; ++i)
	    obs.intArray[i] = byteBuffer.getInt();
	for (int i = 0; i < numDoubles; ++i)
	    obs.doubleArray[i] = byteBuffer.getDouble();

	return obs;
    }

    public static Action getAction(ByteBuffer byteBuffer)
    {
	final int numInts = byteBuffer.getInt();
	final int numDoubles = byteBuffer.getInt();

	Action action = new Action(numInts, numDoubles);	
	for (int i = 0; i < numInts; ++i)
	    action.intArray[i] = byteBuffer.getInt();
	for (int i = 0; i < numDoubles; ++i)
	    action.doubleArray[i] = byteBuffer.getDouble();

	return action;
    }

    public static State_key getStateKey(ByteBuffer byteBuffer)
    {
	final int numInts = byteBuffer.getInt();
	final int numDoubles = byteBuffer.getInt();

	State_key key = new State_key(numInts, numDoubles);
	for (int i = 0; i < numInts; ++i)
	    key.intArray[i] = byteBuffer.getInt();
	for (int i = 0; i < numDoubles; ++i)
	    key.doubleArray[i] = byteBuffer.getDouble();

	return key;
    }

    public static Random_seed_key getRandomSeedKey(ByteBuffer byteBuffer)
    {
	final int numInts = byteBuffer.getInt();
	final int numDoubles = byteBuffer.getInt();

	Random_seed_key key = new Random_seed_key(numInts, numDoubles);
	
	for (int i = 0; i < numInts; ++i)
	    key.intArray[i] = byteBuffer.getInt();

	for (int i = 0; i < numDoubles; ++i)
	    key.doubleArray[i] = byteBuffer.getDouble();

	return key;
    }

    public static void putString(ByteBuffer byteBuffer, String message) throws UnsupportedEncodingException
    {
	byte [] byteString = message.getBytes("UTF-8");
	byteBuffer.putInt(message.length());
	if (message.length() > 0)
	    byteBuffer.put(byteString);
    }

    public static void putObservation(ByteBuffer byteBuffer, Observation obs)
    {
	byteBuffer.putInt(obs.intArray.length);
	byteBuffer.putInt(obs.doubleArray.length);
	for (int i = 0; i < obs.intArray.length; ++i)
	    byteBuffer.putInt(obs.intArray[i]);
	for (int i = 0; i < obs.doubleArray.length; ++i)
	    byteBuffer.putDouble(obs.doubleArray[i]);
    }

    public static void putAction(ByteBuffer byteBuffer, Action action)
    {
	byteBuffer.putInt(action.intArray.length);
	byteBuffer.putInt(action.doubleArray.length);
	for (int i = 0; i < action.intArray.length; ++i)
	    byteBuffer.putInt(action.intArray[i]);
	for (int i = 0; i < action.doubleArray.length; ++i)
	    byteBuffer.putDouble(action.doubleArray[i]);
    }

    public static void putStateKey(ByteBuffer byteBuffer, State_key key)
    {
	byteBuffer.putInt(key.intArray.length);
	byteBuffer.putInt(key.doubleArray.length);
	for (int i = 0; i < key.intArray.length; ++i)
	    byteBuffer.putInt(key.intArray[i]);
	for (int i = 0; i < key.doubleArray.length; ++i)
	    byteBuffer.putDouble(key.doubleArray[i]);
    }

    public static void putRandomSeedKey(ByteBuffer byteBuffer, Random_seed_key key)
    {
	byteBuffer.putInt(key.intArray.length);
	byteBuffer.putInt(key.doubleArray.length);
	for (int i = 0; i < key.intArray.length; ++i)
	    byteBuffer.putInt(key.intArray[i]);
	for (int i = 0; i < key.doubleArray.length; ++i)
	    byteBuffer.putDouble(key.doubleArray[i]);
    }
}

