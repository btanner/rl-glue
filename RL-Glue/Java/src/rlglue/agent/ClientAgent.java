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

package rlglue.agent;

import java.io.*;

import rlglue.network.Network;
import rlglue.types.Action;
import rlglue.types.Observation;

public class ClientAgent 
{
	protected static final String kUnknownMessage = "Unknown Message: ";
	protected Network network;
	protected Agent agent;

	public ClientAgent(Agent agent) 
	{
		this.agent = agent;
		this.network = new Network();
	}

	protected void onAgentInit() throws UnsupportedEncodingException
	{
		String taskSpec = network.getString();

		agent.agent_init(taskSpec);

		network.clearSendBuffer();
		network.putInt(Network.kAgentInit);
		network.putInt(0); // No data following this header
	}

	protected void onAgentStart()
	{
		Observation observation = network.getObservation();
		Action action = agent.agent_start(observation);

		int size = Network.sizeOf(action); 

		network.clearSendBuffer();
		network.putInt(Network.kAgentStart);
		network.putInt(size);
		network.putAction(action);
	}

	protected void onAgentStep()
	{
		double reward = network.getDouble();
		Observation observation = network.getObservation();
		Action action = agent.agent_step(reward, observation);

		int size = Network.sizeOf(action); 
		network.clearSendBuffer();
		network.putInt(Network.kAgentStep);
		network.putInt(size);
		network.putAction(action);
	}

	protected void onAgentEnd()
	{
		//int size = network.getInt();
		double reward = network.getDouble();
		
		agent.agent_end(reward);

		network.clearSendBuffer();
		network.putInt(Network.kAgentEnd);
		network.putInt(0); // No data in this packet
	}

	protected void onAgentCleanup()
	{
		agent.agent_cleanup();

		network.clearSendBuffer();
		network.putInt(Network.kAgentCleanup);
		network.putInt(0); // No data in this packet
	}

	protected void onAgentFreeze()
	{
		agent.agent_freeze();

		network.clearSendBuffer();
		network.putInt(Network.kAgentFreeze);
		network.putInt(0); // No data in this packet
	}

	protected void onAgentMessage() throws UnsupportedEncodingException
	{
		String message = network.getString();
		String reply = agent.agent_message(message);
		
		network.clearSendBuffer();
		network.putInt(Network.kAgentMessage);
		network.putInt(Network.sizeOf(reply));
		network.putString(reply);
	}

	public void connect(String host, int port, int timeout) throws Exception
	{	
		network.connect(host, port, timeout);

		network.clearSendBuffer();
		network.putInt(Network.kAgentConnection);
		network.putInt(0); // No body to this packet
		network.flipSendBuffer();
		network.send();
	}

	public void close() throws IOException
	{
		network.close();
	}

	public void runAgentEventLoop() throws Exception
	{		
		int agentState = 0;
		int dataSize = 0;
		int recvSize = 0;
		int remaining = 0;
		
		do {
			
			network.clearRecvBuffer();

			recvSize = network.recv(8) - 8; // We may have received the header and part of the payload
											// We need to keep track of how much of the payload was recv'd

			agentState = network.getInt(0);
			dataSize = network.getInt(Network.kIntSize);
			
			remaining = dataSize - recvSize;
			if (remaining < 0){
				System.out.println("Remaining was less than 0!");
				remaining = 0;
			}
			

			int amountReceived = network.recv(remaining);			

			network.flipRecvBuffer();
			
			// We have already received the header, now we need to discard it.
			network.getInt();
			network.getInt();

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

			case Network.kRLTerm:
				break;

			default:
				System.err.println(kUnknownMessage + agentState);
				System.exit(1);
			break;
			};

			network.flipSendBuffer();
			network.send();
		} while (agentState != Network.kRLTerm);
	}
}
