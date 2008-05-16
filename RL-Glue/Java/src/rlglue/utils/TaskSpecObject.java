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

public class TaskSpecObject {

    TaskSpecDelegate theTSO = null;
    int TSVersion = 0;

    
    public int getVersion(){
        return TSVersion;
    }
    public TaskSpecObject(String taskSpec) {
        String errorAccumulator="Task Spec Parse Results:";
        try {
            theTSO = new TaskSpecV3(taskSpec);
            TSVersion = 3;
        } catch (Exception e) {
            errorAccumulator+="\nParsing as V3: "+e.toString();
        }

        if (theTSO == null) {
            try {
                theTSO = new TaskSpecV2(taskSpec);
                TSVersion = 2;
            } catch (Exception e) {
            errorAccumulator+="\nParsing as V2: "+e.toString();
            }
        }
        
        if (theTSO == null) {
            System.err.println("Task spec couldn't be parsed");
            throw new IllegalArgumentException(errorAccumulator);
        }

    }


    public String toString() {
        return theTSO.getStringRepresentation();
    }

    public String dump() {
        return theTSO.dump();
    }

    //check if obs_min[index] is negative infinity
    public boolean isObsMinNegInfinity(int index) {
        return theTSO.isObsMinNegInfinity(index);
    }
    //check if action_min[index] is negative infinity
    public boolean isActionMinNegInfinity(int index) {
        return theTSO.isActionMinNegInfinity(index);
    }
    //check if obs_max[index] is positive infinity
    public boolean isObsMaxPosInfinity(int index) {
        return theTSO.isObsMaxPosInfinity(index);
    }
    //check if action_max[index] is positive infinity
    public boolean isActionMaxPosInfinity(int index) {
        return theTSO.isActionMaxPosInfinity(index);
    }
    //check if the value range for observation[index] is known
    public boolean isObsMinUnknown(int index) {
        return theTSO.isObsMinUnknown(index);
    }

    public boolean isObsMaxUnknown(int index) {
        return theTSO.isObsMaxUnknown(index);
    }
    //check if the value range for action[index] is known
    public boolean isActionMinUnknown(int index) {
        return theTSO.isActionMinUnknown(index);
    }

    public boolean isActionMaxUnknown(int index) {
        return theTSO.isActionMaxUnknown(index);
    }

    public boolean isMinRewardNegInf() {
        return theTSO.isMinRewardNegInf();
    }

    public boolean isMaxRewardInf() {
        return theTSO.isMaxRewardInf();
    }

    public boolean isMinRewardUnknown() {
        return theTSO.isMinRewardUnknown();
    }

    public boolean isMaxRewardUnknown() {
        return theTSO.isMaxRewardUnknown();
    }
    
    public static void main(String[] args){
        String sampleTS="2:e:2_[f,f]_[-1.2,0.6]_[-0.07,0.07]:1_[i]_[0,2]";
        TaskSpecObject theTSO=new TaskSpecObject(sampleTS);
//        System.out.println(sampleTS+" is version: "+theTSO.getVersion());
//
//    
//        sampleTS="2:e:2_[f,f]_[-1.2,0.6]_[-0.07,0.07]:1_[i]_[0,2]:[]";
//        theTSO=new TaskSpecObject(sampleTS);
//        System.out.println(sampleTS+" is version: "+theTSO.getVersion());
//
//        sampleTS="2:e:2_[f,f]_[-1.2,0.6]_[-0.07,0.07]:1_[i]_[0,2]:[0,3]";
//        theTSO=new TaskSpecObject(sampleTS);
//        System.out.println(sampleTS+" is version: "+theTSO.getVersion());

        sampleTS="2:e:2_[f,f]_[-1.2,0.6]_[-0.07,0.07]:1_[i]_[0,2]:[0,3]:Extra strings and stuff here";
        theTSO=new TaskSpecObject(sampleTS);
        System.out.println(sampleTS+" is version: "+theTSO.getVersion() +"\n" +theTSO.toString());
        System.out.println(theTSO.dump());

//        sampleTS="2:e:2_[f,f]_[-1.2,0.6]_[-0.07,0.07]:1_[i]_[0,2]:[0,3]:";
//        theTSO=new TaskSpecObject(sampleTS);
//        System.out.println(sampleTS+" is version: "+theTSO.getVersion());
//
//        sampleTS="2:e:[0,3]";
//        theTSO=new TaskSpecObject(sampleTS);
//        System.out.println(sampleTS+" is version: "+theTSO.getVersion());
}
}
