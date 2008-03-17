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

public class Observation_action {

    public Observation o;
    public Action a;

    public Observation_action() {
    }

    public Observation_action(Observation theObservation, Action theAction) {
        this.o = theObservation;
        this.a = theAction;
    }

    /**
     * This is a deep copy constructor
     * @param src
     */
    public Observation_action(Observation_action src) {
        this.o = src.o.duplicate();
        this.a = src.a.duplicate();
    }

    public Observation_action duplicate() {
        return new Observation_action(this);
    }
}
