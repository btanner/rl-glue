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

package rlglue.types;


public class Reward_observation_action_terminal
{
	public double r;
	public Observation o;
	public Action a;
	public int terminal;

	public Reward_observation_action_terminal()
	{
	}

	public Reward_observation_action_terminal(double reward, Observation observation, Action action, int terminal)
	{
		this.r = reward;
		this.o = observation;
		this.a = action;
		this.terminal = terminal;
	}
}
