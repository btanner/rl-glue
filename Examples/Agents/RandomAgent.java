package RandomAgent;

import java.util.Random;

import rlVizLib.utilities.TaskSpecObject;
import rlglue.agent.Agent;
import rlglue.types.Action;
import rlglue.types.Observation;

public class RandomAgent implements Agent {
	private Action action;
	private int numInts =1;
	private int numDoubles =0;
	private Random random = new Random();

        TaskSpecObject TSO=null;
	

        
        public RandomAgent(){
        }

	public void agent_cleanup() {
            // TODO Auto-generated method stub
		
	}

	public void agent_end(double arg0) {
            // TODO Auto-generated method stub
		
	}

	public void agent_freeze() {
            // TODO Auto-generated method stub
		
	}

	public void agent_init(String taskSpec) {
            TSO = new TaskSpecObject(taskSpec);
            // TODO Auto-generated method stub
            action = new Action(TSO.num_discrete_action_dims,TSO.num_continuous_action_dims);	
	}

	public String agent_message(String arg0) {
            // TODO Auto-generated method stub
            return null;
	}

	public Action agent_start(Observation o) {
            randomify(action);
            //ask(o,action);
            return action;
	}

	public Action agent_step(double arg0, Observation o) {
            randomify(action);
            return action;
	}

	public double getValueForState(Observation theObservation) {
            // TODO Auto-generated method stub
            return 0;
	}
	private void randomify(Action action){
            for(int i=0;i<TSO.num_discrete_action_dims;i++){
                action.intArray[i]=random.nextInt(((int)TSO.action_maxs[i]+1)-(int)TSO.action_mins[i]) + ((int)TSO.action_mins[i]);
                // System.out.println(action.intArray[i]);
            }
            for(int i=0;i<TSO.num_continuous_action_dims;i++){
                action.doubleArray[i]=random.nextDouble()*(TSO.action_maxs[i] - TSO.action_mins[i]) + TSO.action_mins[i];
                //System.out.println(action.doubleArray[i]);
            }
       	}
	

      /*  
        public static void main(String [] args) throws Exception
	{
	    
		String taskSpec = "2:e:2_[f,f]_[-1.2,0.6]_[-0.07,0.07]:1_[f]_[-5,5]";
		
		TaskSpecObject taskObject = new TaskSpecObject(taskSpec);
		System.err.println(taskObject);
                
                TaskSpecObject specObj = new TaskSpecObject(taskSpec);
		// TODO Auto-generated method stub
		Action theaction = new Action(specObj.num_discrete_action_dims,specObj.num_continuous_action_dims);
                RandomAgent ra = new RandomAgent();
                ra.agent_init(taskSpec);
                Observation o = new Observation(1,1);
                for(int i=0; i< 50; i++){
                    ra.agent_step(i, o);
                }
                
			
	}*/
}
