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
 limitations under the License.rlglue
*/


package rlglue.utilities;

/**
 * TaskSpecDelegate was written to be the medium between the Task Spec object
 * agents and environments interact with, and the implementation of each version
 * of the Task Spec. This allows for the addition of functionality to the Task
 * Spec by adding new versions without breaking previous Versions.We'll extend 
 * this class over time, adding more stuff to it, but we'll be careful such that
 * we don't need to *ever* change existing subclasses.
 * <p>
 * TaskSpecDelegate does not implement any of these functions, it acts as an 
 * interface.
 *   
 * @author Matt Radkie
 */
public abstract class TaskSpecDelegate {

    /**
     * @see rlglue.utilities.TaskSpec#dump()
     */
    protected String dump() {
        throw new UnsupportedOperationException("Not yet implemented");
    }

    /**
     * @see rlglue.utilities.TaskSpec#getStringRepresentation()
     */
    protected String getStringRepresentation() {
        throw new UnsupportedOperationException("Not yet implemented");
    }

    /**
     * @see rlglue.utilities.TaskSpec#isObsMinNegInfinity(int index)
     */
    public boolean isObsMinNegInfinity(int index) {
        throw new NoSuchMethodError("This version of the task spec does not support: isObsMinNegInfinity");
    }

    /**
     * @see rlglue.utilities.TaskSpec#isActionMinNegInfinity(int index)
     */
    public boolean isActionMinNegInfinity(int index) {
        throw new NoSuchMethodError("This version of the task spec does not support: isActionMinNegInfinity");
    }

    /**
     * @see rlglue.utilities.TaskSpec#isObsMaxPosInfinity(int index)
     */
    public boolean isObsMaxPosInfinity(int index) {
        throw new NoSuchMethodError("This version of the task spec does not support: isObsMaxPosInfinity");
    }

    /**
     * @see rlglue.utilities.TaskSpec#isActionMaxPosInfinity(int index)
     */
    public boolean isActionMaxPosInfinity(int index) {
        throw new NoSuchMethodError("This version of the task spec does not support: isActionMaxPosInfinity");
    }

    /**
     * @see rlglue.utilities.TaskSpec#isObsMinUnknown(int index)
     */
    public boolean isObsMinUnknown(int index) {
        throw new NoSuchMethodError("This version of the task spec does not support: isObsMinUnknown");
    }

    /**
     * @see rlglue.utilities.TaskSpec#isObsMaxUnknown(int index)
     */
    public boolean isObsMaxUnknown(int index) {
        throw new NoSuchMethodError("This version of the task spec does not support: isObsMaxUnknown");
    }

    /**
     * @see rlglue.utilities.TaskSpec#isActionMinUnknown(int index)
     */
    public boolean isActionMinUnknown(int index) {
        throw new NoSuchMethodError("This version of the task spec does not support: isActionMinUnknown");
    }

    /**
     * @see rlglue.utilities.TaskSpec#isActionMaxUnknown(int index)
     */
    public boolean isActionMaxUnknown(int index) {
        throw new NoSuchMethodError("This version of the task spec does not support: isActionMaxUnknown");
    }

    /**
     * @see rlglue.utilities.TaskSpec#isMinRewardNegInf()
     */
    public boolean isMinRewardNegInf() {
        throw new NoSuchMethodError("This version of the task spec does not support: isMinRewardNegInf");
    }

    /**
     * @see rlglue.utilities.TaskSpec#isMaxRewardInf()
     */
    public boolean isMaxRewardInf() {
        throw new NoSuchMethodError("This version of the task spec does not support: isMaxRewardInf");
    }

    /**
     * @see rlglue.utilities.TaskSpec#isMinRewardUnknown()
     */
    public boolean isMinRewardUnknown() {
        throw new NoSuchMethodError("This version of the task spec does not support: isMinRewardUnknown");
    }

    /**
     * @see rlglue.utilities.TaskSpec#isMaxRewardUnknown()
     */
    public boolean isMaxRewardUnknown() {
        throw new NoSuchMethodError("This version of the task spec does not support: isMaxRewardUnknown");
    }

    /**
     * @see rlglue.utilities.TaskSpec#getVersion() 
     */
    public double getVersion() {
        throw new NoSuchMethodError("This version of the task spec does not support: getVersion");
    }

    /**
     * @see rlglue.utilities.TaskSpec#setVersion(int version)
     */
    public void setVersion(int version) {
        throw new NoSuchMethodError("This version of the task spec does not support: setVersion");
    }

    /**
     * @see rlglue.utilities.TaskSpec#getEpisodic()
     */
    public char getEpisodic() {
        throw new NoSuchMethodError("This version of the task spec does not support: getEpisodic");
    }

    /**
     * @see rlglue.utilities.TaskSpec#setEpisodic(char episodic)
     */
    public void setEpisodic(char episodic) {
        throw new NoSuchMethodError("This version of the task spec does not support: setEpisodic");
    }

    /**
     * @see rlglue.utilities.TaskSpec#getObsDim()
     */
    public int getObsDim() {
        throw new NoSuchMethodError("This version of the task spec does not support: getobsDim");
    }

    /**
     * @see rlglue.utilities.TaskSpec#setObsDim(int dim)
     */
    public void setObsDim(int dim) {
        throw new NoSuchMethodError("This version of the task spec does not support: setobsDim");
    }

    /**
     * @see rlglue.utilities.TaskSpec#getNumDiscreteObsDims()
     */
    public int getNumDiscreteObsDims() {
        throw new NoSuchMethodError("This version of the task spec does not support: getNumDiscreteObsDims");
    }

    /**
     * @see rlglue.utilities.TaskSpec#setNumDiscreteObsDims(int numDisc)
     */
    public void setNumDiscreteObsDims(int numDisc) {
        throw new NoSuchMethodError("This version of the task spec does not support: setNumDiscreteObsDims");
    }

    /**
     * @see rlglue.utilities.TaskSpec#getNumContinuousObsDims()
     */
    public int getNumContinuousObsDims() {
        throw new NoSuchMethodError("This version of the task spec does not support: getNumContinuousActionDims");
    }

    /**
     * @see rlglue.utilities.TaskSpec#setNumContinuousObsDims(int numCont)
     */
    public void setNumContinuousObsDims(int numCont) {
        throw new NoSuchMethodError("This version of the task spec does not support: setNumContinuousActionDims");
    }

    /**
     * @see rlglue.utilities.TaskSpec#getObsTypes()
     */
    public char[] getObsTypes() {
        throw new NoSuchMethodError("This version of the task spec does not support: getObsTypes");
    }

    /**
     * @see rlglue.utilities.TaskSpec#setObsTypes(char[] types)
     */
    public void setObsTypes(char[] types) {
        throw new NoSuchMethodError("This version of the task spec does not support: setObsTypes");
    }

    /**
     * @see rlglue.utilities.TaskSpec#getObsMins()
     */
    public double[] getObsMins() {
        throw new NoSuchMethodError("This version of the task spec does not support: getObsMins");
    }

    /**
     * @see rlglue.utilities.TaskSpec#setObsMins(double[] mins)
     */
    public void setObsMins(double[] mins) {
        throw new NoSuchMethodError("This version of the task spec does not support: setObsMins");
    }

    /**
     * @see rlglue.utilities.TaskSpec#getObsMaxs()
     */
    public double[] getObsMaxs() {
        throw new NoSuchMethodError("This version of the task spec does not support: getObsMaxs");
    }

    /**
     * @see rlglue.utilities.TaskSpec#setObsMaxs(double[] maxs)
     */
    public void setObsMaxs(double[] maxs) {
        throw new NoSuchMethodError("This version of the task spec does not support: setObsMaxs");
    }

    /**
     * @see rlglue.utilities.TaskSpec#getActionDim()
     */
    public int getActionDim() {
        throw new NoSuchMethodError("This version of the task spec does not support: getActionDim");
    }

    /**
     * @see rlglue.utilities.TaskSpec#setActionDim(int dim)
     */
    public void setActionDim(int dim) {
        throw new NoSuchMethodError("This version of the task spec does not support: setActionDim");
    }

    /**
     * @see rlglue.utilities.TaskSpec#getNumDiscreteActionDims()
     */
    public int getNumDiscreteActionDims() {
        throw new NoSuchMethodError("This version of the task spec does not support: getNumDiscreteActionDims");
    }

    /**
     * @see rlglue.utilities.TaskSpec#setNumDiscreteActionDims(int numDisc)
     */
    public void setNumDiscreteActionDims(int numDisc) {
        throw new NoSuchMethodError("This version of the task spec does not support: setNumDiscreteActionDims");
    }

    /**
     * @see rlglue.utilities.TaskSpec#getNumContinuousActionDims()
     */
    public int getNumContinuousActionDims() {
        throw new NoSuchMethodError("This version of the task spec does not support: getNumContinuousActionDims");
    }

    /**
     * @see rlglue.utilities.TaskSpec#setNumContinuousActionDims(int numCont)
     */
    public void setNumContinuousActionDims(int numCont) {
        throw new NoSuchMethodError("This version of the task spec does not support: setNumContinuousActionDims");
    }

    /**
     * @see rlglue.utilities.TaskSpec#getActionTypes()
     */
    public char[] getActionTypes() {
        throw new NoSuchMethodError("This version of the task spec does not support: getActionTypes");
    }

    /**
     * @see rlglue.utilities.TaskSpec#setActionTypes(char[] types)
     */
    public void setActionTypes(char[] types) {
        throw new NoSuchMethodError("This version of the task spec does not support: setActionTypes");
    }

    /**
     * @see rlglue.utilities.TaskSpec#getActionMins()
     */
    public double[] getActionMins() {
        throw new NoSuchMethodError("This version of the task spec does not support: getActionMins");
    }

    /**
     * @see rlglue.utilities.TaskSpec#setActionMins(double[] mins)
     */
    public void setActionMins(double[] mins) {
        throw new NoSuchMethodError("This version of the task spec does not support: setActionMins");
    }

    /**
     * @see rlglue.utilities.TaskSpec#getActionMaxs()
     */
    public double[] getActionMaxs() {
        throw new NoSuchMethodError("This version of the task spec does not support: getActionMaxs");
    }

    /**
     * @see rlglue.utilities.TaskSpec#setActionMaxs(double[] maxs)
     */
    public void setActionMaxs(double[] maxs) {
        throw new NoSuchMethodError("This version of the task spec does not support: setActionMaxs");
    }

    /**
     * @see rlglue.utilities.TaskSpec#getRewardMax()
     */
    public double getRewardMax() {
        throw new NoSuchMethodError("This version of the task spec does not support: getRewardMax");
    }

    /**
     * @see rlglue.utilities.TaskSpec#setRewardMax(double max)
     */
    public void setRewardMax(double max) {
        throw new NoSuchMethodError("This version of the task spec does not support: setRewardMax");
    }

    /**
     * @see rlglue.utilities.TaskSpec#getRewardMin()
     */
    public double getRewardMin() {
        throw new NoSuchMethodError("This version of the task spec does not support: getRewardMin");
    }

    /**
     * @see rlglue.utilities.TaskSpec#setRewardMin(double min)
     */
    public void setRewardMin(double min) {
        throw new NoSuchMethodError("This version of the task spec does not support: getRewardMin");
    }

    /**
     * @see rlglue.utilities.TaskSpec#getExtraString()
     */
    public String getExtraString() {
        throw new NoSuchMethodError("This version of the task spec does not support: getExtraString");
    }

    /**
     * @see rlglue.utilities.TaskSpec#setExtraString(String newString)
     */
    public void setExtraString(String newString) {
        throw new NoSuchMethodError("This version of the task spec does not support: setExtraString");
    }

    /**
     * @see rlglue.utilities.TaskSpec#getParserVersion()
     */
    public int getParserVersion() {
        throw new NoSuchMethodError("This version of the task spec does not support: getParserVersion");
    }
}
