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

	public static final int kRLTerm          = 35;

	public static final String kDefaultHost = "127.0.0.1";
	public static final int kDefaultPort = 4096;
	public static final int kRetryTimeout = 10;

	protected static final int kByteBufferDefaultSize = 65536;
	protected static final int kIntSize = 4;
	protected static final int kDoubleSize = 8;
	
	protected SocketChannel socketChannel;
	protected ByteBuffer recvBuffer;
	protected ByteBuffer sendBuffer;
	
	protected int bytesInRecvBuffer = 0;

	public Network()
	{
		recvBuffer = ByteBuffer.allocateDirect(kByteBufferDefaultSize);
		sendBuffer = ByteBuffer.allocateDirect(kByteBufferDefaultSize);
	}

	public void connect(String host, int port, int retryTimeout)
	{
		boolean didConnect = false;
		int count = 0;

		while (!didConnect)
		{
			try 
			{	
				InetSocketAddress address = new InetSocketAddress(host, port);
				socketChannel = SocketChannel.open();
				socketChannel.configureBlocking(true);
				socketChannel.connect(address);
				didConnect = true;
			}
			catch (IOException ioException)
			{
				try
				{
					Thread.sleep(retryTimeout);
				}
				catch (InterruptedException ieException)
				{
				}
			}
		}
	}

	public void close() throws IOException
	{
		socketChannel.close();
	}

	public int send() throws IOException
	{
		return socketChannel.write(sendBuffer);
	}

	public int recv(int size) throws IOException
	{
		int recvSize = 0;
		while (recvSize < size)
		{
			recvSize += socketChannel.read(recvBuffer);
		}
		return recvSize;
	}
	
	public int recvBuffered(int requestSize)
	{			
		/* When doing a receive, we often get more data than we asked for.
		 * If there is enough data left in the buffer, we can immediately read it out,
		 * otherwise we need to do a read, and update our book keeping.
		 */

		int remaining = requestSize - bytesInRecvBuffer;
		 		
		if (remaining > 0)
		{
			System.err.println("Buffer Empty. Reading From Socket...")
			this.ensureRecvCapacityRemains(remaining);
					
			bytesInRecvBuffer -= getRecvPosition();
			compactRecvBuffer();
			setRecvPosition(bytesInRecvBuffer);

			try 
			{
				bytesInRecvBuffer += recv(remaining);
			}
			catch (IOException ioException)
			{
				ioException.printStackTrace();
				System.exit(1);
			}
			
			setRecvPosition(0);
			
			System.err.println("bytesInRecvBuffer: " + bytesInRecvBuffer + " pos: " + getRecvPosition());
		}
		
//		network.setRecvPosition(0);
//		
//		int glueState = network.getInt();
//		int dataSize = network.getInt();
//		recvPointer -= network.getRecvPosition();
//		
//		network.compactRecvBuffer();
//		network.setRecvPosition(recvPointer);
//		int remaining = dataSize - recvSize;
//		if (remaining < 0)
//			remaining = 0;
//		
//		recvSize = network.recv(remaining);
//		recvPointer += recvSize;
//		network.setRecvPosition(0);
		
		return remaining;
	}

	public void clearSendBuffer()
	{
		sendBuffer.clear();
	}
	
	public void clearRecvBuffer()
	{
		recvBuffer.clear();
	}
	
	public void flipSendBuffer()
	{
		sendBuffer.flip();
	}
	
	public void flipRecvBuffer()
	{
		recvBuffer.flip();
	}
	
	public void compactRecvBuffer()
	{
		recvBuffer.compact();
	}
	
	public void setRecvPosition(int position)
	{
		recvBuffer.position(position);
	}
	
	public int getRecvPosition()
	{
		return recvBuffer.position();
	}
	
	public int getInt()
	{
		recvBuffered(Network.kIntSize);
		return recvBuffer.getInt();
	}
	
	public int getInt(int index)
	{
		return recvBuffer.getInt(index);
	}
	
	public double getDouble()
	{
		recvBuffered(Network.kDoubleSize);
		return recvBuffer.getDouble();
	}
	
	public String getString() throws UnsupportedEncodingException
	{
		int recvLength = this.getInt();
		recvBuffered(recvLength);

		byte [] recvString = new byte[recvLength];
		recvBuffer.get(recvString);
	
		String response = new String(recvString, "UTF-8");
		System.err.println("length:\t" + response.length() + "\t|" + response + "|");
		
		return response;
	}

	public Observation getObservation()
	{
		final int numInts = this.getInt();
		final int numDoubles = this.getInt();

		Observation obs = new Observation(numInts, numDoubles);	
		
		for (int i = 0; i < numInts; ++i)
			obs.intArray[i] = this.getInt();
		
		for (int i = 0; i < numDoubles; ++i)
			obs.doubleArray[i] = this.getDouble();

		return obs;
	}

	public Action getAction()
	{
		final int numInts = this.getInt();
		final int numDoubles = this.getInt();

		Action action = new Action(numInts, numDoubles);
		
		for (int i = 0; i < numInts; ++i)
			action.intArray[i] = this.getInt();
		
		for (int i = 0; i < numDoubles; ++i)
			action.doubleArray[i] = this.getDouble();

		return action;
	}

	public State_key getStateKey()
	{
		final int numInts = this.getInt();
		final int numDoubles = this.getInt();

		State_key key = new State_key(numInts, numDoubles);
		
		for (int i = 0; i < numInts; ++i)
			key.intArray[i] = this.getInt();
		
		for (int i = 0; i < numDoubles; ++i)
			key.doubleArray[i] = this.getDouble();

		return key;
	}

	public Random_seed_key getRandomSeedKey()
	{
		final int numInts = this.getInt();
		final int numDoubles = this.getInt();

		Random_seed_key key = new Random_seed_key(numInts, numDoubles);

		for (int i = 0; i < numInts; ++i)
			key.intArray[i] = this.getInt();

		for (int i = 0; i < numDoubles; ++i)
			key.doubleArray[i] = this.getDouble();

		return key;
	}
	
	public void putInt(int value)
	{
		this.ensureSendCapacityRemains(Network.kIntSize);
		this.sendBuffer.putInt(value);
	}
	
	public void putDouble(double value)
	{
		this.ensureSendCapacityRemains(Network.kDoubleSize);
		this.sendBuffer.putDouble(value);
	}

	public void putString(String message) throws UnsupportedEncodingException
	{
		this.ensureSendCapacityRemains(Network.kIntSize + message.length());
		this.putInt(message.length());

		if (message.length() > 0)
			sendBuffer.put(message.getBytes("UTF-8"));
	}

	public void putObservation(Observation obs)
	{
		this.ensureSendCapacityRemains(Network.sizeOf(obs));
		
		this.putInt(obs.intArray.length);
		this.putInt(obs.doubleArray.length);
		
		for (int i = 0; i < obs.intArray.length; ++i)
			this.putInt(obs.intArray[i]);
		
		for (int i = 0; i < obs.doubleArray.length; ++i)
			this.putDouble(obs.doubleArray[i]);
	}

	public void putAction(Action action)
	{
		this.ensureSendCapacityRemains(Network.sizeOf(action));
		
		this.putInt(action.intArray.length);
		this.putInt(action.doubleArray.length);
		
		for (int i = 0; i < action.intArray.length; ++i)
			this.putInt(action.intArray[i]);
		
		for (int i = 0; i < action.doubleArray.length; ++i)
			this.putDouble(action.doubleArray[i]);
	}

	public void putStateKey(State_key key)
	{
		this.ensureSendCapacityRemains(Network.sizeOf(key));
		
		this.putInt(key.intArray.length);
		this.putInt(key.doubleArray.length);
		
		for (int i = 0; i < key.intArray.length; ++i)
			this.putInt(key.intArray[i]);
		
		for (int i = 0; i < key.doubleArray.length; ++i)
			this.putDouble(key.doubleArray[i]);
	}

	public void putRandomSeedKey(Random_seed_key key)
	{
		this.ensureSendCapacityRemains(Network.sizeOf(key));
		
		this.putInt(key.intArray.length);
		this.putInt(key.doubleArray.length);
		
		for (int i = 0; i < key.intArray.length; ++i)
			this.putInt(key.intArray[i]);
		
		for (int i = 0; i < key.doubleArray.length; ++i)
			this.putDouble(key.doubleArray[i]);
	}
	
	public void putRewardObservation(Reward_observation rewardObservation)
	{
		this.ensureSendCapacityRemains(Network.sizeOf(rewardObservation));
		
		this.putInt(rewardObservation.terminal);
		this.putDouble(rewardObservation.r);
		this.putObservation(rewardObservation.o);
	}

	protected void ensureSendCapacityRemains(int capacity)
	{
		if (sendBuffer.capacity() - sendBuffer.position() < capacity)
			sendBuffer = Network.cloneWithCapacity(sendBuffer, sendBuffer.capacity() + capacity);
	}
	
	protected void ensureRecvCapacityRemains(int capacity)
	{
		if (recvBuffer.capacity() - recvBuffer.position() < capacity)
			recvBuffer = Network.cloneWithCapacity(recvBuffer, recvBuffer.capacity() + capacity);
	}
	
	protected static ByteBuffer cloneWithCapacity(ByteBuffer original, int capacity)
	{
		ByteBuffer clone = ByteBuffer.allocateDirect(capacity);
		clone.put(original);
		clone.position(original.position());
		return clone;
	}
	
	protected static int sizeOf(int value)
	{
		return Network.kIntSize;
	}
	
	protected static int sizeOf(double value)
	{
		return Network.kDoubleSize;
	}
	
	protected static int sizeOf(String string)
	{
		return Network.kIntSize + string.length();
	}
	
	protected static int sizeOf(Action action)
	{
		return Network.kIntSize * 2 + 
				Network.kIntSize * action.intArray.length + 
				Network.kDoubleSize * action.doubleArray.length;
	}
	
	protected static int sizeOf(Observation obs)
	{
		return Network.kIntSize * 2 + 
				Network.kIntSize * obs.intArray.length + 
				Network.kDoubleSize * obs.doubleArray.length;
	}
	
	protected static int sizeOf(State_key key)
	{
		return Network.kIntSize * 2 + 
				Network.kIntSize * key.intArray.length + 
				Network.kDoubleSize * key.doubleArray.length;
	}
	
	protected static int sizeOf(Random_seed_key key)
	{
		return Network.kIntSize * 2 + 
				Network.kIntSize * key.intArray.length + 
				Network.kDoubleSize * key.doubleArray.length;
	}
	
	protected static int sizeOf(Reward_observation rewardObservation)
	{
		return Network.kIntSize +      // terminal
				Network.kDoubleSize +  // reward
				Network.sizeOf(rewardObservation.o);
	}
}

