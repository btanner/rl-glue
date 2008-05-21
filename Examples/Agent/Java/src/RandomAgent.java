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

package RandomAgent;

import java.util.Random;

//import rlVizLib.utilities.TaskSpecObject;
import rlglue.utilities.TaskSpec;
import rlglue.agent.Agent;
import rlglue.types.Action;
import rlglue.types.Observation;

public class RandomAgent implements Agent {
	private Action action;
	private int numInts =1;
	private int numDoubles =0;
	private Random random = new Random();

        TaskSpec TSO=null;
	

        
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

	public void agent_init(String taskSpecString) {
            TSO = new TaskSpec(taskSpecString);
            // TODO Auto-generated method stub
            action = new Action(TSO.getNumDiscreteActionDims(),TSO.getNumContinuousActionDims());	
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
            for(int i=0;i<TSO.getNumDiscreteActionDims();i++){
                action.intArray[i]=random.nextInt(((int)TSO.getActionMaxs()[i]+1)-(int)TSO.getActionMins()[i]) + ((int)TSO.getActionMins()[i]);
                // System.out.println(action.intArray[i]);
            }
            for(int i=0;i<TSO.getNumContinuousActionDims();i++){
                action.doubleArray[i]=random.nextDouble()*(TSO.getActionMaxs()[i] - TSO.getActionMins()[i]) + TSO.getActionMins()[i];
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
