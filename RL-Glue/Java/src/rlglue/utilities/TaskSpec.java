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
package rlglue.utilities;
/**
 * This class is used to store and parse the information given to an Agent in
 * the RL-Glue framework. The Task Spec stores information the Agent needs
 * regarding the environment, such as the number of actions, observations, and 
 * their ranges. For more information please read the
 * <a href="http://rlai.cs.ualberta.ca/RLBB/TaskSpecification.html"> 
 * RL-Glue Task Spec info</a>
 * <p>
 * This class was written to handle future revisions to the Task Spec while
 * providing backwards compatibility with old Task Spec versions. This is 
 * accomplished through the use of the factory design pattern. The 
 * TaskSpecDelegate object is the medium through which the TaskSpec communicates
 * with different implementations of the Task Spec versions. So far, each
 * Task Spec version has incrementally added functionality, but this might not
 * always be the case, so this framework was designed to robustly accomodate
 * future versions.
 * 
 * @author Matt Radkie
 */
public class TaskSpec {

    /**
     * Generic object extended by all versions of TaskSpec parsers 
     */
    private TaskSpecDelegate theTSO = null;
    /**
     * The version of the Task Spec.
     */
    private int TSVersion = 0;

    /**
     * Gets the Task Spec version.
     * 
     * @param none
     * @return Integer value of the Task Spec version.
     */
    public int getVersion() {
        return TSVersion;
    }

    /**
     * Constructor that takes a string adhereing to the Task Spec language
     * protocol. This string is parsed out by the appropriate version of the
     * Task Spec.
     * 
     * @param taskSpec String following the Task Spec language
     */
    public TaskSpec(String taskSpec) {
        String errorAccumulator = "Task Spec Parse Results:";
        try {
            theTSO = new TaskSpecV3(taskSpec);
            TSVersion = 3;
        } catch (Exception e) {
            errorAccumulator += "\nParsing as V3: " + e.toString();
        }

        if (theTSO == null) {
            try {
                theTSO = new TaskSpecV2(taskSpec);
                TSVersion = 2;
            } catch (Exception e) {
                errorAccumulator += "\nParsing as V2: " + e.toString();
            }
        }

        if (theTSO == null) {
            System.err.println("Task spec couldn't be parsed");
            throw new IllegalArgumentException(errorAccumulator);
        }

    }

    /**
     * Returns the string representation of the Task Spec object. This string
     * representation follows the Task Spec language as outlined
     * <a href="http://rlai.cs.ualberta.ca/RLBB/TaskSpecification.html"> 
     * here</a>
     * 
     * @param none
     * @return String representation of the Task Spec
     */
    public String toString() {
        return theTSO.getStringRepresentation();
    }

    /**
     * Returns a string containing debug information about the Task Spec. This
     * debug information is usually printed to the screen, but returning it as 
     * a string allows the caller to print it out to log files etc as well.
     * 
     * @param none
     * @return String containing debug information for the Task Spec.
     */
    public String dump() {
        return theTSO.dump();
    }

    /**
     * Checks if the observation min at index is negative infinity.
     * 
     * @param index Integer index of the obs_min array.
     * @return True if obs_min[index] is negative infinity, false otherwise.
     */
    public boolean isObsMinNegInfinity(int index) {
        return theTSO.isObsMinNegInfinity(index);
    }

    /**
     * Checks if the action min at index is negative infinity.
     * 
     * @param index - integer index of the action_mins array.
     * @return True if action_min[index] is negative infinity, false otherwise.
     */
    public boolean isActionMinNegInfinity(int index) {
        return theTSO.isActionMinNegInfinity(index);
    }

    /**
     * Checks if the observation max at index is positive infinity.
     * 
     * @param index Interger index of the obs_maxs array.
     * @return True if obs_max[index] is positive infinity, false otherwise.
     */
    public boolean isObsMaxPosInfinity(int index) {
        return theTSO.isObsMaxPosInfinity(index);
    }

    /**
     * Checks if the action max at index is positive infinity.
     * 
     * @param index Integer index of the action_maxs array.
     * @return True if action_max[index] is positive infinity, false otherwise.
     */
    public boolean isActionMaxPosInfinity(int index) {
        return theTSO.isActionMaxPosInfinity(index);
    }

    /**
     * Checks if the observation min at index is unknown.
     * 
     * @param index Integer index of the obs_mins array.
     * @return True if the min value for observation[index] is unknown, false 
     * otherwise.
     */
    public boolean isObsMinUnknown(int index) {
        return theTSO.isObsMinUnknown(index);
    }

    /**
     * Checks if the observation max at index is unknown.
     * 
     * @param index Integer index of the obs_max array.
     * @return True if the max value for observation[index] is unknown, false 
     * otherwise.
     */
    public boolean isObsMaxUnknown(int index) {
        return theTSO.isObsMaxUnknown(index);
    }

    /**
     * Checks if the min action at index is unknown.
     * 
     * @param index Integer index of the action_mins array.
     * @return True if the min value for action[index] is unknown, false 
     * otherwise.
     */
    public boolean isActionMinUnknown(int index) {
        return theTSO.isActionMinUnknown(index);
    }

    /**
     * Checks if the action max at index is unknown.
     * 
     * @param index Integer index of the action_maxs array.
     * @return True if the max value for action[index] is unknown, false 
     * otherwise.
     */
    public boolean isActionMaxUnknown(int index) {
        return theTSO.isActionMaxUnknown(index);
    }

    /**
     * Checks if the min reward is negative infinity.
     * 
     * @param none
     * @return True if the min reward is negative infinity, false 
     * otherwise.
     */
    public boolean isMinRewardNegInf() {
        return theTSO.isMinRewardNegInf();
    }

    /**
     * Checks if the max reward is positive infinity.
     * 
     * @param none
     * @return True if the max reward is positive infinity, false 
     * otherwise.
     */
    public boolean isMaxRewardInf() {
        return theTSO.isMaxRewardInf();
    }

    /**
     * Checks if the min reward is unknown.
     * 
     * @param none
     * @return True if the min reward is unknown, false 
     * otherwise.
     */
    public boolean isMinRewardUnknown() {
        return theTSO.isMinRewardUnknown();
    }

    /**
     * Checks if the max reward is unknown.
     * 
     * @param none
     * @return True if the max reward is unknown, false 
     * otherwise.
     */
    public boolean isMaxRewardUnknown() {
        return theTSO.isMaxRewardUnknown();
    }

    /**
     * Gets the version of the Task spec.
     * 
     * @param none
     * @return the version of the Task Spec used.
     */
    public double getTaskSpecVersion() {
        return theTSO.getVersion();
    }

    /**
     * Set the version of the Task Spec.
     * 
     * @param version Integer representing the version of the Task Spec.
     * @return none
     */
    public void setVersion(int version) {
        theTSO.setVersion(version);
    }

    /**
     * Gets the episodic characteristic of the Task Spec.
     * 
     * @param none
     * @return Char value representing if an environment is episodic
     */
    public char getEpisodic() {
        return theTSO.getEpisodic();
    }

    /**
     * Set the episodec character in the Task Spec.
     * 
     * @param episodic Character representing whether an environment is episodic.
     * @return none
     */
    public void setEpisodic(char episodic) {
        theTSO.setEpisodic(episodic);
    }

    /**
     * Gets the size of the observation array (Number of observations)
     * 
     * @param none
     * @return The size of the observation array (Number of observations)
     */
    public int getObsDim() {
        return theTSO.getObsDim();
    }

    /**
     * Set the size of the observation array.
     * 
     * @param dim Integer for the size of the observation array.
     * @return none
     */
    public void setobsDim(int dim) {
        theTSO.setObsDim(dim);
    }

    /**
     * Gets the number of descrete observations.
     * 
     * @param none
     * @return Integer value for the number of descrete observations
     */
    public int getNumDiscreteObsDims() {
        return theTSO.getNumDiscreteObsDims();
    }

    /**
     * Sets the number of descrete observations.
     * 
     * @param numDisc Integer number of descrete observations.
     * @return none
     */
    public void setNumDiscreteObsDims(int numDisc) {
        theTSO.setNumDiscreteObsDims(numDisc);
    }

    /**
     * Gets the number of continuous observations.
     * 
     * @param none
     * @return Integer value for the number of continuous observations.
     */
    public int getNumContinuousObsDims() {
        return theTSO.getNumContinuousObsDims();
    }

    /**
     * Sets the number of continuous observations.
     * 
     * @param numDisc Integer number of continuous observations.
     * @return none
     */
    public void setNumContinuousObsDims(int numCont) {
        theTSO.setNumContinuousObsDims(numCont);
    }

    /**
     * Gets the types for the observations.
     * 
     * @param none
     * @return Character array representing the types of the observations.
     */
    public char[] getObsTypes() {
        return theTSO.getObsTypes();
    }

    /**
     * Sets the types for the observations.
     * 
     * @param types Character array representing the types of the observations.
     * @return none
     */
    public void setObsTypes(char[] types) {
        theTSO.setObsTypes(types);
    }

    /**
     * Gets the array of mins for the observations.
     * 
     * @param none
     * @return double[] Array of the min values for the observations.
     */
    public double[] getObsMins() {
        return theTSO.getObsMins();
    }

    /**
     * Sets the array of mins for the observations.
     * 
     * @param mins array of doubles corresponding to the mins for the 
     * observations.
     * @return none
     */
    public void setObsMins(double[] mins) {
        theTSO.setObsMins(mins);
    }

    /**
     * Gets the array of maxs for the observations.
     * 
     * @param none
     * @return double[] Array of the maxs values for the observations.
     */
    public double[] getObsMaxs() {
        return theTSO.getObsMaxs();
    }

    /**
     * Sets the array of maxs for the observations.
     * 
     * @param mins array of doubles corresponding to the maxs for the 
     * observations.
     * @return none
     */
    public void setObsMaxs(double[] maxs) {
        theTSO.setObsMaxs(maxs);
    }

    /**
     * Gets the size of the action array (Number of actions)
     * 
     * @param none
     * @return The size of the action array (Number of actions)
     */
    public int getActionDim() {
        return theTSO.getActionDim();
    }

    /**
     * Set the size of the action array.
     * 
     * @param dim Integer for the size of the action array.
     * @return none
     */
    public void setActionDim(int dim) {
        theTSO.setActionDim(dim);
    }

    /**
     * Gets the number of descrete actions
     * 
     * @param none
     * @return Integer number of descrete actions.
     */
    public int getNumDiscreteActionDims() {
        return theTSO.getNumDiscreteActionDims();
    }

    /**
     * Sets the number of descrete actions.
     * 
     * @param numDisc Integer number of descrete actions.
     * @return none
     */
    public void setNumDiscreteActionDims(int numDisc) {
        theTSO.setNumDiscreteActionDims(numDisc);
    }

    /**
     * Gets the number of continous actions
     * 
     * @param none
     * @return Integer number of continous actions.
     */
    public int getNumContinuousActionDims() {
        return theTSO.getNumContinuousActionDims();
    }

    /**
     * Sets the number of continous actions.
     * 
     * @param numDisc Integer number of continous actions.
     * @return none
     */
    public void setNumContinuousActionDims(int numCont) {
        theTSO.setNumContinuousActionDims(numCont);
    }

    /**
     * Gets the types for the actions.
     * 
     * @param none
     * @return Character array representing the types of the actions.
     */
    public char[] getActionTypes() {
        return theTSO.getActionTypes();
    }

    /**
     * Sets the types for the actions.
     * 
     * @param types Character array representing the types of the actions.
     * @return none
     */
    public void setActionTypes(char[] types) {
        theTSO.setActionTypes(types);
    }

    /**
     * Gets the array of mins for the actions.
     * 
     * @param none
     * @return double[] Array of the min values for the actions.
     */
    public double[] getActionMins() {
        return theTSO.getActionMins();
    }

    /**
     * Sets the array of mins for the actions.
     * 
     * @param mins Double array of values corresponding to action min values.
     * @return none
     */
    public void setActionMins(double[] mins) {
        theTSO.setActionMins(mins);
    }

    /**
     * Gets the array of maxs for the actions.
     * 
     * @param none
     * @return double[] Array of the max values for the actions.
     */
    public double[] getActionMaxs() {
        return theTSO.getActionMaxs();
    }

    /**
     * Sets the array of maxs for the actions.
     * 
     * @param mins Double array of values corresponding to action max values.
     * @return none
     */
    public void setActionMaxs(double[] maxs) {
        theTSO.setActionMaxs(maxs);
    }

    /**
     * Gets the max reward.
     * 
     * @param none
     * @return Double value of the max reward.
     */
    public double getRewardMax() {
        return theTSO.getRewardMax();
    }

    /**
     * Sets the max reward.
     * 
     * @param max Double value of the max reward
     * @return none
     */
    public void setRewardMax(double max) {
        theTSO.setRewardMax(max);
    }

    /**
     * Gets the min reward.
     * 
     * @param none
     * @return Double value of the min reward.
     */
    public double getRewardMin() {
        return theTSO.getRewardMin();
    }

    /**
     * Sets the min reward.
     * 
     * @param min Double value of the min reward.
     * @return none
     */
    public void setRewardMin(double min) {
        theTSO.setRewardMin(min);
    }

    /**
     * Gets the string value for the ExtraString.
     * 
     * 'ExtraString' is new for Task Spec version 3. It allows additional
     * information to be appended to the end of the Task Spec. When environments
     * use this feature, agents will require special code to handle this.
     * 
     * @param none
     * @return String of additional information appended onto the end of the
     * Task Spec.
     */
    public String getExtraString() {
        return theTSO.getExtraString();
    }

    /**
     * Sets the string value for the ExtraString.
     * 
     * 'ExtraString' is new for Task Spec version 3. It allows additional
     * information to be appended to the end of the Task Spec. When environments
     * use this feature, agents will require special code to handle this.
     * 
     * @param newString the new string to be appended to the TaskSpec.
     * @return none
     */
    public void setExtraString(String newString) {
        theTSO.setExtraString(newString);
    }

    /**
     * Gets the version of the parser used on the Task Spec.
     * 
     * @param none
     * @return Integer version of the parser used on the Task Spec.
     */
    public int getParserVersion() {
        return theTSO.getParserVersion();
    }

    /**
     * Main has no purpose in this class other than for debugging. This should
     * have been deleted prior to release, but as it makes on going development
     * easier, it has been left for now. Ideally in the future, this code will
     * be removed and moved into test cases.
     * 
     * @param args
     * @return none
     */
    public static void main(String[] args) {
        /*
        String sampleTS = "2:e:2_[f,f]_[-1.2,0.6]_[-0.07,0.07]:1_[i]_[0,2]";
        TaskSpec theTSO = new TaskSpec(sampleTS);
        
        System.out.println(sampleTS+" is version: "+theTSO.getVersion());
        sampleTS="2:e:2_[f,f]_[-1.2,0.6]_[-0.07,0.07]:1_[i]_[0,2]:[]";
        theTSO=new TaskSpec(sampleTS);
        System.out.println(sampleTS+" is version: "+theTSO.getVersion());
        sampleTS="2:e:2_[f,f]_[-1.2,0.6]_[-0.07,0.07]:1_[i]_[0,2]:[0,3]";
        theTSO=new TaskSpec(sampleTS);
        System.out.println(sampleTS+" is version: "+theTSO.getVersion());
        sampleTS = "2:e:2_[f,f]_[-1.2,0.6]_[-0.07,0.07]:1_[i]_[0,2]:[0,3]:Extra strings and stuff here";
        theTSO = new TaskSpec(sampleTS);
        System.out.println(sampleTS + " is version: " + theTSO.getVersion() + "\n" + theTSO.toString());
        System.out.println(theTSO.dump());
        sampleTS="2:e:2_[f,f]_[-1.2,0.6]_[-0.07,0.07]:1_[i]_[0,2]:[0,3]:";
        theTSO=new TaskSpec(sampleTS);
        System.out.println(sampleTS+" is version: "+theTSO.getVersion());
        sampleTS="2:e:[0,3]";
        theTSO=new TaskSpec(sampleTS);
        System.out.println(sampleTS+" is version: "+theTSO.getVersion());
         */
    }
}