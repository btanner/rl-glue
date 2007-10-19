/* 
* Copyright (C) 2007, Brian Tanner
* 
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (at your option) any later version.
* 
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* 
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA. */

package rlglue.network;

import java.io.IOException;
import java.io.UnsupportedEncodingException;
import java.net.InetSocketAddress;
import java.nio.ByteBuffer;
import java.nio.channels.SocketChannel;

import rlglue.types.Action;
import rlglue.types.Observation;
import rlglue.types.Random_seed_key;
import rlglue.types.Reward_observation;
import rlglue.types.State_key;

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

	protected static final int kByteBufferDefaultSize = 4096;
	public static final int kIntSize = 4;
	protected static final int kDoubleSize = 8;
	
	protected SocketChannel socketChannel;
	protected ByteBuffer recvBuffer;
	protected ByteBuffer sendBuffer;

	public Network()
	{
		recvBuffer = ByteBuffer.allocateDirect(kByteBufferDefaultSize);
		sendBuffer = ByteBuffer.allocateDirect(kByteBufferDefaultSize);
	}

	public void connect(String host, int port, int retryTimeout)
	{
		boolean didConnect = false;

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
		this.ensureRecvCapacityRemains(size);

		int recvSize = 0;
		while (recvSize < size)
		{
			recvSize += socketChannel.read(recvBuffer);
		}
		return recvSize;
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
	
	public int getInt() 
	{
		return recvBuffer.getInt();
	}
	
	public int getInt(int index)
	{
		return recvBuffer.getInt(index);
	}
	
	public double getDouble()
	{
		return recvBuffer.getDouble();
	}
	
	public String getString() throws UnsupportedEncodingException
	{
		int recvLength = this.getInt();
		byte [] recvString = new byte[recvLength];
		recvBuffer.get(recvString);
		return new String(recvString, "UTF-8");
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
		// We don't want to have to deal null...
		if (message == null)
			message = "";
		
		this.ensureSendCapacityRemains(Network.kIntSize + message.length());
		this.putInt(message.length());

		if (message.length() > 0) 
		{
			sendBuffer.put(message.getBytes("UTF-8"));
		}
	}

	public void putObservation(Observation obs)
	{	
		this.ensureSendCapacityRemains(Network.sizeOf(obs));

		int numInts = 0;
		int numDoubles = 0;
		
		if (obs != null)
		{
			if (obs.intArray != null)
				numInts = obs.intArray.length;
			
			if (obs.doubleArray != null)
				numDoubles = obs.doubleArray.length;
		}
		

		this.putInt(numInts);
		this.putInt(numDoubles);
		
		for (int i = 0; i < numInts; ++i)
			this.putInt(obs.intArray[i]);
		
		for (int i = 0; i < numDoubles; ++i)
			this.putDouble(obs.doubleArray[i]);

	}

	public void putAction(Action action)
	{
		this.ensureSendCapacityRemains(Network.sizeOf(action));
		
		int numInts = 0;
		int numDoubles = 0;
		
		if (action != null)
		{
			if (action.intArray != null)
				numInts = action.intArray.length;
			
			if (action.doubleArray != null)
				numDoubles = action.doubleArray.length;
		}
		
		this.putInt(numInts);
		this.putInt(numDoubles);
		
		for (int i = 0; i < numInts; ++i)
			this.putInt(action.intArray[i]);
		
		for (int i = 0; i < numDoubles; ++i)
			this.putDouble(action.doubleArray[i]);
	}

	public void putStateKey(State_key key)
	{
		this.ensureSendCapacityRemains(Network.sizeOf(key));
		
		int numInts = 0;
		int numDoubles = 0;
		
		if (key != null)
		{
			if (key.intArray != null)
				numInts = key.intArray.length;
			
			if (key.doubleArray != null)
				numDoubles = key.doubleArray.length;
		}
		
		this.putInt(numInts);
		this.putInt(numDoubles);
		
		for (int i = 0; i < numInts; ++i)
			this.putInt(key.intArray[i]);
		
		for (int i = 0; i < numDoubles; ++i)
			this.putDouble(key.doubleArray[i]);
	}

	public void putRandomSeedKey(Random_seed_key key)
	{
		this.ensureSendCapacityRemains(Network.sizeOf(key));
		
		int numInts = 0;
		int numDoubles = 0;
		
		if (key != null)
		{
			if (key.intArray != null)
				numInts = key.intArray.length;
			
			if (key.doubleArray != null)
				numDoubles = key.doubleArray.length;
		}

		this.putInt(numInts);
		this.putInt(numDoubles);
		
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
		original.flip();
		clone.put(original);
		clone.position(original.position());
		return clone;
	}
	
	public static int sizeOf(int value)
	{
		return Network.kIntSize;
	}
	
	public static int sizeOf(double value)
	{
		return Network.kDoubleSize;
	}
	
	public static int sizeOf(String string)
	{
		int size = Network.kIntSize;
		if (string != null)
			size += string.length();
		return size;
	}
	
	public static int sizeOf(Action action)
	{
		int size = Network.kIntSize * 2;
		int intSize = 0;
		int doubleSize = 0;
		
		if (action != null)
		{
			if (action.intArray != null)
				intSize = Network.kIntSize * action.intArray.length;
			
			if (action.doubleArray != null)
				doubleSize = Network.kDoubleSize * action.doubleArray.length;
		}
		return size + intSize + doubleSize;
	}
	
	public static int sizeOf(Observation obs)
	{
		int size = Network.kIntSize * 2;
		int intSize = 0;
		int doubleSize = 0;
		
		if (obs != null)
		{
			if (obs.intArray != null)
				intSize = Network.kIntSize * obs.intArray.length;

			if (obs.doubleArray != null)
				doubleSize = Network.kDoubleSize * obs.doubleArray.length;
		}
		return size + intSize + doubleSize;
	}
	
	public static int sizeOf(State_key key)
	{
		int size = Network.kIntSize * 2;
		int intSize = 0;
		int doubleSize = 0;
		
		if (key != null)
		{
			if (key.intArray != null)
				intSize = Network.kIntSize * key.intArray.length;
			
			if (key.doubleArray != null)
				doubleSize = Network.kDoubleSize * key.doubleArray.length;
		}
		return size + intSize + doubleSize;
	}
	
	public static int sizeOf(Random_seed_key key)
	{
		int size = Network.kIntSize * 2;
		int intSize = 0;
		int doubleSize = 0;
		
		if (key != null)
		{
			if (key.intArray != null)
				intSize = Network.kIntSize * key.intArray.length;
			
			if (key.doubleArray != null)
				doubleSize = Network.kDoubleSize * key.doubleArray.length;
		}
		return size + intSize + doubleSize;
	}
	
	public static int sizeOf(Reward_observation rewardObservation)
	{
		return Network.kIntSize + // terminal
			Network.kDoubleSize + // reward
			Network.sizeOf(rewardObservation.o);
	}
}

