package rlglue.types;

public class Reward_observation 
{
	public double r;
	public Observation o;
	public int terminal;

	public Reward_observation()
	{
	}

	public Reward_observation(double reward, Observation observation, int terminal)
	{
		this.r = reward;
		this.o = observation;
		this.terminal = terminal;
	}
}
