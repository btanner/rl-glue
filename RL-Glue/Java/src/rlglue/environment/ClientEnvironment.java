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

package rlglue.environment;

import java.io.*;

import rlglue.network.Network;
import rlglue.types.Action;
import rlglue.types.Observation;
import rlglue.types.Reward_observation;
import rlglue.types.Random_seed_key;
import rlglue.types.State_key;

public class ClientEnvironment
{
	protected static final String kUnknownMessage = "ClientEnvironment.java :: Unknown Message: ";
	protected Network network;
	protected Environment env;

	public ClientEnvironment(Environment env) 
	{
		this.env = env;
		this.network = new Network();
	}

	protected void onEnvInit() throws UnsupportedEncodingException 
	{
		String taskSpec = env.env_init();

		network.clearSendBuffer();
		network.putInt(Network.kEnvInit);
		network.putInt(Network.sizeOf(taskSpec)); // This is different than taskSpec.length(). It also includes
												  // the four bytes that describe the length of the string
												  // that is inserted by network.putString()
		network.putString(taskSpec);
	}

	protected void onEnvStart()
	{
		Observation obs = env.env_start();

		network.clearSendBuffer();
		network.putInt(Network.kEnvStart);
		network.putInt(Network.sizeOf(obs));
		network.putObservation(obs);
	}

	protected void onEnvStep()
	{
		Action action = network.getAction();
		Reward_observation rewardObservation = env.env_step(action);	
		
		network.clearSendBuffer();
		network.putInt(Network.kEnvStep);
		network.putInt(Network.sizeOf(rewardObservation));

		network.putRewardObservation(rewardObservation);
	}

	protected void onEnvCleanup()
	{
		network.clearSendBuffer();
		network.putInt(Network.kEnvCleanup);
		network.putInt(0);
	}

	protected void onEnvGetRandomSeed()
	{
		Random_seed_key key = env.env_get_random_seed();
		
		network.clearSendBuffer();
		network.putInt(Network.kEnvGetRandomSeed);
		network.putInt(Network.sizeOf(key));
		network.putRandomSeedKey(key);
	}
	
	protected void onEnvGetState()
	{
		State_key key = env.env_get_state();
		
		network.clearSendBuffer();
		network.putInt(Network.kEnvGetState);
		network.putInt(Network.sizeOf(key));
		network.putStateKey(key);
	}
	
	protected void onEnvSetRandomSeed()
	{
		Random_seed_key key = network.getRandomSeedKey();
		env.env_set_random_seed(key);
			
		network.clearSendBuffer();
		network.putInt(Network.kEnvSetRandomSeed);
		network.putInt(0);
	}
	
	protected void onEnvSetState()
	{
		State_key key = network.getStateKey();
		env.env_set_state(key);

		network.clearSendBuffer();
		network.putInt(Network.kEnvSetState);
		network.putInt(0);
	}
	
	protected void onEnvMessage() throws UnsupportedEncodingException
	{
		String message = network.getString();
		String reply = env.env_message(message);

		network.clearSendBuffer();
		network.putInt(Network.kEnvMessage);
		network.putInt(Network.sizeOf(reply));
		network.putString(reply);
	}

	public void connect(String host, int port, int timeout) throws Exception
	{	
		network.connect(host, port, timeout);

		network.clearSendBuffer();
		network.putInt(Network.kEnvironmentConnection);
		network.putInt(0); // No body to this packet
		network.flipSendBuffer();
		network.send();
	}

	public void close() throws IOException
	{
		network.close();
	}

	public void runEnvironmentEventLoop() throws Exception
	{
		int envState = 0;
		int dataSize = 0;
		int recvSize = 0;
		int remaining = 0;

		do {
			network.clearRecvBuffer();
			recvSize = network.recv(8) - 8; // We may have received the header and part of the payload
											// We need to keep track of how much of the payload was recv'd

			envState = network.getInt(0);
			dataSize = network.getInt(Network.kIntSize);

			remaining = dataSize - recvSize;
			if (remaining < 0)
				remaining = 0;
			
			network.recv(remaining);			
			network.flipRecvBuffer();
			
			// We have already received the header, now we need to discard it.
			network.getInt();
			network.getInt();
			
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

			case Network.kEnvSetState:
				onEnvSetState();
				break;
				
			case Network.kEnvSetRandomSeed:
				onEnvSetRandomSeed();
				break;

			case Network.kEnvGetState:
				onEnvGetState();
				break;
				
			case Network.kEnvGetRandomSeed:
				onEnvGetRandomSeed();
				break;
				
			case Network.kEnvMessage:
				onEnvMessage();
				break;
				
			case Network.kRLTerm:
				break;

			default:
				System.err.println(kUnknownMessage + envState);
				System.exit(1);
				break;
			};

			network.flipSendBuffer();
			network.send();

		} while (envState != Network.kRLTerm);
	}
}
