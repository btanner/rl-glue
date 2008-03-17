/* 
* Copyright (C) 2007, Brian Tanner
* 
http://rl-glue.googlecode.com/

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 */

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
        
    public Reward_observation_action_terminal(Reward_observation_action_terminal src) {
        this.r = src.r;
        this.o = src.o.duplicate();
        this.a = src.a.duplicate();
        this.terminal = src.terminal;
    }

    public Reward_observation_action_terminal duplicate() {
        return new Reward_observation_action_terminal(this);
    }

}
