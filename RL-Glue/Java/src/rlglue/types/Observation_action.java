package rlglue.types;


public class Observation_action
{
	public Observation o;
	public Action a;

	public Observation_action()
	{
	}

	public Observation_action(Observation theObservation, Action theAction)
	{
		o = theObservation;
		a = theAction;
	}

}
