import rlglue.*;
import rlglue.network.*;

public class SarsaAgent 
{
    void agent_init(final String taskSpec)
    {  
    }

    Action agent_start(Observation o)
    {	
	Action action = new Action(1, 0);
	action.intArray[0] = 0;
	
	return action;
    }

    Action agent_step(double r, Observation o)
    {
	Action action = new Action(1, 0);
	action.intArray[0] = 0;
	
    return action;
    }
    
    
    void agent_end(double r)
    { 
    }
    
    void agent_cleanup() 
    {
    }
    
    String agent_message(final String message)
    {
	return "This agent does not respond to any messages";
    }
    
    void agent_freeze()
    {
    }
}
