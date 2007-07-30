import rlglue.agent.Agent;
import rlglue.types.Action;
import rlglue.types.Observation;

import java.util.Random;

public class SarsaAgent implements Agent
{
    protected static final int kNumStates = 100;
    protected static final int kNumActions = 4;

    protected Random rand;
    protected Action previousAction;
    protected Observation previousObservation;

    double [][] value;
    double sarsaAlpha = 0.1;
    double sarsaGamma = 0.9;
    boolean freeze = false;

    public SarsaAgent() 
    {
	rand = new Random();
    }

    public void agent_init(final String taskSpec)
    {
    }

    public Action agent_start(Observation o)
    {	
	Action action = new Action(1, 0);
	value = new double[kNumStates][kNumActions];

	action.intArray[0] = egreedy(o);
	previousAction = action;
	previousObservation = o;

	return action;
    }

    public Action agent_step(double r, Observation o)
    {
	Action a = new Action(1, 0);

	double oldQ = 0; 
	double newQ = 0; 
	
	int newState = o.intArray[0];
	int oldState = previousObservation.intArray[0];
	int oldAction  = previousAction.intArray[0];
	int newAction = egreedy(o);

	/*pick the next action. Note: the policy is frozen internally. We haven't written an egreedy
	 *and a greedy method like in sample_agent.c The epsilon is removed within the egreedy method if
	 *the policy is frozen*/
	a.intArray[0] = newAction;
	
	oldQ = value[oldState][oldAction];
	newQ = value[newState][newAction];
		
	/*if we haven't frozen the agent, we should improve our value function with each step*/
	if(!freeze) {
	    value[oldState][oldAction] = oldQ + sarsaAlpha*(r + sarsaGamma*newQ - oldQ);
	    
	    /* We need to copy the action if we want to retain the values in it */
	    previousAction = a;

	    /* We need to copy the Observation if we want to retain the values in it */   
	    previousObservation = o;
	}
	
	return a;
    }
    
    public void agent_end(double r)
    {
	int oldState = previousObservation.intArray[0];
	int oldAction = previousAction.intArray[0];
    
	double oldQ = value[oldState][oldAction];

	/*if the agent isn't frozen, do the last update to the value function, in sarsa this
	 *means the currentQ is zero for this calculation*/
	if(!freeze) {
	    value[oldState][oldAction] = oldQ + sarsaAlpha*(r - oldQ);
	}
    }
    
    public void agent_cleanup() 
    {
    }
    
    public String agent_message(final String message)
    {
	return "This agent does not respond to any messages.";
    }
    
    public void agent_freeze()
    {
	freeze = true;
    }

    protected int egreedy(Observation o) {
	int state = o.intArray[0];
	int max = 0;

	/*If NOT frozen, with epsilon = 0.1 probability, act randomly*/
	if((!freeze) && (rand.nextInt(10) == 1)) {
	    return rand.nextInt(4);
	}
	/*else choose the greedy action*/
	max = 0;
	for(int i = 1; i < 4; i++) {
	    if(value[state][i] > value[state][max]) {
		max = i;
	    }
	}	
	return max;
    }
}
