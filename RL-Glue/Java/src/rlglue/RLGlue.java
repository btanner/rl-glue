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

package rlglue;

import java.io.*;

import rlglue.network.Network;
import rlglue.types.Observation_action;
import rlglue.types.Random_seed_key;
import rlglue.types.Reward_observation_action_terminal;
import rlglue.types.State_key;

public class RLGlue
{

	protected static Network network = null;

	private static void forceConnection()
	{
		if (network == null)
		{   
			network = new Network();

			// Connect
			network.connect(Network.kDefaultHost, 
					Network.kDefaultPort, 
					Network.kRetryTimeout);

			network.clearSendBuffer();
			network.putInt(Network.kExperimentConnection);
			network.putInt(0);
			network.flipSendBuffer();

			try 
			{
				network.send();
			}
			catch(IOException ioException)
			{
				ioException.printStackTrace();
				System.exit(1);
			}
		}
	}

	private static synchronized void doStandardRecv(int state) throws IOException
	{
		network.clearRecvBuffer();
		
		int recvSize = network.recv(8) - 8;

		int glueState = network.getInt(0);
		int dataSize = network.getInt(Network.kIntSize);
		int remaining = dataSize - recvSize;

		if (remaining < 0)
			remaining = 0;
		
		int remainingReceived=network.recv(remaining);

		network.flipRecvBuffer();	
		
		// Discard the header - we should have a more elegant method for doing this.
		network.getInt();
		network.getInt();
		
		if (glueState != state)
		{
			System.err.println("Not synched with server. glueState = " + glueState + " but should be " + state);
			System.exit(1);
		}
	}
	
	private static synchronized void doCallWithNoParams(int state) throws IOException
	{		
		network.clearSendBuffer();
		network.putInt(state);
		network.putInt(0);
		network.flipSendBuffer();
		network.send();
	}
	
	public static synchronized void RL_init()
	{
		forceConnection();

		try
		{
			doCallWithNoParams(Network.kRLInit);
			doStandardRecv(Network.kRLInit);
		}
		catch(IOException ioException)
		{
			ioException.printStackTrace();
			System.exit(1);
		}
	}

	public static synchronized Observation_action RL_start()
	{
		Observation_action obsact = null;
		try
		{

			doCallWithNoParams(Network.kRLStart);

			doStandardRecv(Network.kRLStart);
			
			obsact = new Observation_action();

			obsact.o = network.getObservation();

			obsact.a = network.getAction();
		}
		catch (IOException ioException)
		{
			ioException.printStackTrace();
			System.exit(1);
		}
		catch (NullPointerException nullException)
		{
			System.err.println("You must call RL_init before calling RL_start");
			nullException.printStackTrace();
			System.exit(1);
		}


		return obsact;
	}

	public static synchronized Reward_observation_action_terminal RL_step()
	{
		Reward_observation_action_terminal roat = null;
		try 
		{
			doCallWithNoParams(Network.kRLStep);
			doStandardRecv(Network.kRLStep);
			
			roat = new Reward_observation_action_terminal();
			roat.terminal = network.getInt();
			roat.r = network.getDouble();
			roat.o = network.getObservation();
			roat.a = network.getAction();

			return roat;
		}
		catch (IOException ioException)
		{
			ioException.printStackTrace();
			System.exit(1);
		}
		catch (NullPointerException nullException)
		{
			System.err.println("You must call RL_init before calling RL_step");
			nullException.printStackTrace();
			System.exit(1);
		}
		return roat;
	}

	public static synchronized void RL_cleanup()
	{
		try
		{
			doCallWithNoParams(Network.kRLCleanup);
			doStandardRecv(Network.kRLCleanup);

			//network.close(); // Cleanup no longer closes the connection.  
			
			//We need to be able to run multiple RL_init/RL_cleanup's without killing the 
			//connection between.  Since this code is running from the user experiment
			//the connection never gets explicitly closed.  :(
			//The VM/OS will clean this up and close the connection when the program exits.
		}
		catch (IOException ioException)
		{
			ioException.printStackTrace();
			System.exit(1);
		}
		catch (NullPointerException nullException)
		{
			System.err.println("You must call RL_init before calling RL_cleanup");
			nullException.printStackTrace();
			System.exit(1);
		}
	}

	public static synchronized String RL_agent_message(String message)
	{		
		String response = "";
		forceConnection();

		try
		{
			network.clearSendBuffer();
			network.putInt(Network.kRLAgentMessage);
			network.putInt(Network.sizeOf(message));
			network.putString(message);
			network.flipSendBuffer();
			network.send();

			doStandardRecv(Network.kRLAgentMessage);
			response = network.getString();
		}
		catch (IOException ioException)
		{
			ioException.printStackTrace();
			System.exit(1);
		}
		return response;
	}

	public static synchronized String RL_env_message(String message)
	{
		String response = "";
		forceConnection();
				
		try
		{

			network.clearSendBuffer();
			network.putInt(Network.kRLEnvMessage);
			network.putInt(Network.sizeOf(message));
			network.putString(message);
			network.flipSendBuffer();
			network.send();

			doStandardRecv(Network.kRLEnvMessage);
			response = network.getString();

		}
		catch (IOException ioException)
		{
			ioException.printStackTrace();
			System.exit(1);
		}
		return response;
	}

	public static synchronized double RL_return()
	{
		double reward = 0.0;

		try
		{
			doCallWithNoParams(Network.kRLReturn);
			doStandardRecv(Network.kRLReturn);
			reward = network.getDouble();
		}
		catch (IOException ioException)
		{
			ioException.printStackTrace();
			System.exit(1);
		}
		catch (NullPointerException nullException)
		{
			System.err.println("You must call RL_init before calling RL_return");
			nullException.printStackTrace();
			System.exit(1);
		}
		return reward;
	}

	public static synchronized int RL_num_steps()
	{
		int numSteps = 0;
		
		try
		{
			doCallWithNoParams(Network.kRLNumSteps);
			doStandardRecv(Network.kRLNumSteps);
			
			numSteps = network.getInt();
		}
		catch (IOException ioException)
		{
			ioException.printStackTrace();
			System.exit(1);
		}
		catch (NullPointerException nullException)
		{
			System.err.println("You must call RL_init before calling RL_num_steps");
			nullException.printStackTrace();
			System.exit(1);
		}
		
		return numSteps;
	}

	public static synchronized int RL_num_episodes()
	{
		int numEpisodes = 0;
		
		try
		{
			doCallWithNoParams(Network.kRLNumEpisodes);
			doStandardRecv(Network.kRLNumEpisodes);
			numEpisodes = network.getInt();
		}
		catch (IOException ioException)
		{
			ioException.printStackTrace();
			System.exit(1);
		}
		catch (NullPointerException nullException)
		{
			System.err.println("You must call RL_init before calling RL_num_episodes");
			nullException.printStackTrace();
			System.exit(1);
		}
		return numEpisodes;
	}

	public static synchronized void RL_episode(int numSteps)
	{
		try
		{
			network.clearSendBuffer();
			network.putInt(Network.kRLEpisode);
			network.putInt(Network.kIntSize);
			network.putInt(numSteps);
			network.flipSendBuffer();
			network.send();

			doStandardRecv(Network.kRLEpisode);
		}
		catch (IOException ioException)
		{
			ioException.printStackTrace();
			System.exit(1);
		}
		catch (NullPointerException nullException)
		{
			System.err.println("You must call RL_init before calling RL_episode");
			nullException.printStackTrace();
			System.exit(1);
		}
	}

	public static synchronized void RL_freeze()
	{
		try
		{
			doCallWithNoParams(Network.kRLFreeze);
			doStandardRecv(Network.kRLFreeze);
		}
		catch (IOException ioException)
		{
			ioException.printStackTrace();
			System.exit(1);
		}
		catch (NullPointerException nullException)
		{
			System.err.println("You must call RL_init before calling RL_freeze");
			nullException.printStackTrace();
			System.exit(1);
		}
	}

	public static synchronized void RL_set_state(State_key sk)
	{
		try
		{
			network.clearSendBuffer();
			network.putInt(Network.kRLSetState);
			network.putInt(Network.sizeOf(sk));
			network.putStateKey(sk);
			network.flipSendBuffer();
			network.send();

			doStandardRecv(Network.kRLSetState);
		}
		catch (IOException ioException)
		{
			ioException.printStackTrace();
			System.exit(1);
		}
		catch (NullPointerException nullException)
		{
			System.err.println("You must call RL_init before calling RL_set_state");
			nullException.printStackTrace();
			System.exit(1);
		}
	}

	public static synchronized void RL_set_random_seed(Random_seed_key rsk)
	{
		try
		{
			network.clearSendBuffer();
			network.putInt(Network.kRLSetRandomSeed);
			network.putInt(Network.sizeOf(rsk));
			network.putRandomSeedKey(rsk);
			network.flipSendBuffer();
			network.send();

			doStandardRecv(Network.kRLSetRandomSeed);
		}
		catch (IOException ioException)
		{
			ioException.printStackTrace();
			System.exit(1);
		}
		catch (NullPointerException nullException)
		{
			System.err.println("You must call RL_init before calling RL_set_random_seed");
			nullException.printStackTrace();
			System.exit(1);
		}
	}

	public static synchronized State_key RL_get_state()
	{
		State_key key = null;
		
		try
		{
			doCallWithNoParams(Network.kRLGetState);
			doStandardRecv(Network.kRLGetState);
			
			key = network.getStateKey();
		}
		catch (IOException ioException)
		{
			ioException.printStackTrace();
			System.exit(1);
		}
		catch (NullPointerException nullException)
		{
			System.err.println("You must call RL_init before calling RL_get_state_key");
			nullException.printStackTrace();
			System.exit(1);
		}
		return key;
	}

	public static synchronized Random_seed_key RL_get_random_seed()
	{
		Random_seed_key key = null;
		
		try
		{
			doCallWithNoParams(Network.kRLGetRandomSeed);
			doStandardRecv(Network.kRLGetRandomSeed);
			
			key = network.getRandomSeedKey();
		}
		catch (IOException ioException)
		{
			ioException.printStackTrace();
			System.exit(1);
		}
		catch (NullPointerException nullException)
		{
			System.err.println("You must call RL_init before calling RL_get_state_key");
			nullException.printStackTrace();
			System.exit(1);
		}
		
		return key;
	}
}
