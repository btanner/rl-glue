import rlglue.*;
import rlglue.network.*;

public class SarsaAgent 
{
    void agent_init(final String task_spec)
    {  
    }

    Action agent_start(Observation o)
    {	
	Action action = new Action(1, 0);
	action.intArray[0] = 0;
	
	return action;
    }

    Action agent_step(Reward r, Observation o)
    {
	Action action = new Action(1, 0);
	action.intArray[0] = 0;
	
    return action;
    }
    
    
    void agent_end(Reward r)
    { 
    }
    
    void agent_cleanup() 
    {
    }
    
    Message agent_message(const Message message)
    {
	return "This agent does not respond to any messages";
    }
    
    void agent_freeze()
    {
    }
}
