/*
Copyright 2008 Matt Radkie
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
package rlglue.utils;

/**
 * Here's the trick.  We'll write this class to be the go-to class, that the
 * task spec object will talk to.  We'll extend this class over time, adding more 
 * stuff to it, but we'll be careful such that we don't need to *ever* change
 * existing subclasses.  Lets pretend that's what I mean.  
 * @author mradkie
 */
public class TaskSpecDelegate {

    String dump() {
        throw new UnsupportedOperationException("Not yet implemented");
    }

    String getStringRepresentation() {
        throw new UnsupportedOperationException("Not yet implemented");
    }
    
    
    	//check if obs_min[index] is negative infinity
	public boolean isObsMinNegInfinity(int index)
	{
            throw new NoSuchMethodError("This version of the task spec does not support: isObsMinNegInfinity");
	}
	//check if action_min[index] is negative infinity
	public boolean isActionMinNegInfinity(int index)
	{
            throw new NoSuchMethodError("This version of the task spec does not support: isActionMinNegInfinity");
	}
	//check if obs_max[index] is positive infinity
	public boolean isObsMaxPosInfinity(int index)
	{
            throw new NoSuchMethodError("This version of the task spec does not support: isObsMaxPosInfinity");
	}
	//check if action_max[index] is positive infinity
	public boolean isActionMaxPosInfinity(int index)
	{
            throw new NoSuchMethodError("This version of the task spec does not support: isActionMaxPosInfinity");
	}
	//check if the value range for observation[index] is known
	public boolean isObsMinUnknown(int index)
	{
            throw new NoSuchMethodError("This version of the task spec does not support: isObsMinUnknown");
	}
	public boolean isObsMaxUnknown(int index)
	{
            throw new NoSuchMethodError("This version of the task spec does not support: isObsMaxUnknown");
	}
	//check if the value range for action[index] is known
	public boolean isActionMinUnknown(int index)
	{
            throw new NoSuchMethodError("This version of the task spec does not support: isActionMinUnknown");
	}
	public boolean isActionMaxUnknown(int index)
	{
            throw new NoSuchMethodError("This version of the task spec does not support: isActionMaxUnknown");
	}
	public boolean isMinRewardNegInf()
	{
            throw new NoSuchMethodError("This version of the task spec does not support: isMinRewardNegInf");		
	}
	public boolean isMaxRewardInf()
	{
            throw new NoSuchMethodError("This version of the task spec does not support: isMaxRewardInf");
	}
	public boolean isMinRewardUnknown()
	{
            throw new NoSuchMethodError("This version of the task spec does not support: isMinRewardUnknown");
	}
	public boolean isMaxRewardUnknown()
	{
            throw new NoSuchMethodError("This version of the task spec does not support: isMaxRewardUnknown");
	}
	

}
