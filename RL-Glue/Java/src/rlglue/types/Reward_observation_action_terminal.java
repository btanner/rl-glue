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
