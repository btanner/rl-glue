import java.util.Random;

import rlglue.*;
import rlglue.network.*;

public class SarsaAgent implements AgentInterface
{
    protected Random random;
    public SarsaAgent() 
    {
	random = new Random();
    }

    public void agent_init(final String taskSpec)
    {
    }

    public Action agent_start(Observation o)
    {	
	Action action = new Action(1, 0);
	action.intArray[0] = random.nextInt() % 4;

	return action;
    }

    public Action agent_step(double r, Observation o)
    {
	Action action = new Action(1, 0);
	action.intArray[0] = random.nextInt() % 4;

	return action;
    }
        
    public void agent_end(double r)
    { 
    }
    
    public void agent_cleanup() 
    {
    }
    
    public String agent_message(final String message)
    {
	return "This agent does not respond to any messages";
    }
    
    public void agent_freeze()
    {
    }
}
