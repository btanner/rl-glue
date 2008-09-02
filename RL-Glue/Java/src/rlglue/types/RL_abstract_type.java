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

package rlglue.types;

/**
 * Common superclass for all of the Java RL-Glue types.
 * Try to keep handles to the objects and not their arrays, 
 * because there is no guarantee that the arrays will not be 
 * reallocated during certain operations.
 * @author btanner
 */
public class RL_abstract_type{
	public int [] intArray;
	public double [] doubleArray;

	public RL_abstract_type(int numInts, int numDoubles) {
		intArray = new int[numInts];
		doubleArray = new double[numDoubles];
	}
        
        
        public RL_abstract_type(RL_abstract_type src){
            this(src.intArray.length,src.doubleArray.length);
            RLStructCopy(src, this);
        }
	
	public static void RLStructCopy(RL_abstract_type src, RL_abstract_type dest){
            if(src==null||dest==null)throw new IllegalArgumentException("Either src or dest RL_Abstract_Type was null in RLStructCopy");
            if(dest.intArray.length!=src.intArray.length)
                dest.intArray=new int[src.intArray.length];
            if(dest.doubleArray.length!=src.doubleArray.length)
                dest.doubleArray=new double[src.doubleArray.length];
            
            System.arraycopy(src.intArray, 0,dest.intArray, 0, src.intArray.length);
            System.arraycopy(src.doubleArray, 0,dest.doubleArray, 0, src.doubleArray.length);	
	}
}
