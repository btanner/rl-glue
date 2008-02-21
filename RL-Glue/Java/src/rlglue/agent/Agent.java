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

package rlglue.agent;

import rlglue.types.Action;
import rlglue.types.Observation;

public interface Agent
{
    public void agent_init(final String taskSpecification);
    public Action agent_start(Observation observation);
    public Action agent_step(double reward, Observation observation);
    public void agent_end(double reward);
    public void agent_cleanup();
    public void agent_freeze();
    public String agent_message(final String message);
}
