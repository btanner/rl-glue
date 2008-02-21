/*
Copyright 2007 Brian Tanner
http://rl-glue.googlecode.com/
brian@tannerpages.com
http://brian.tannerpages.com
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
package rlglue;

import rlglue.agent.AgentLoader;
import rlglue.environment.EnvironmentLoader;

/**
 *
 * @author Brian Tanner
 */
public class RLGlueCore {

    private static void printHelp() {
        System.out.println("--------------------------------");
        System.out.println("RL-Glue.jar main diagnostic program");
        System.out.println("--------------------------------");
        System.out.println("-v or --version will print out the interface version");
        System.out.println("-e ENVIRONMENTCLASSNAME or --environment ENVIRONMENTCLASSNAME will load class ENVIRONMENTCLASSNAME (make sure it's in the class path)");
    }

    private static void printVersion() {
        System.out.println(getSpecVersion());
    }
    /*
     * Print out the current interface version of RLGlue.  We'll make it do more interesting things with commandline parameters later
     */

    public static void main(String[] args) {
        if (args.length == 0) {
            printHelp();
            return;
        }
        if (args[0].equalsIgnoreCase("-v") || args[0].equalsIgnoreCase("--version")) {
            printVersion();
            return;
        }
        if (args[0].equalsIgnoreCase("-e") || args[0].equalsIgnoreCase("--environment")) {
            EnvironmentLoader.loadEnvironment(args[1]);
        }
        if (args[0].equalsIgnoreCase("-a") || args[0].equalsIgnoreCase("--agent")) {
            AgentLoader.loadAgent(args[1]);
        }

    }

    /**
     * Get the Specification (Interface) version of RLGlue as set in the Manifest file.  
     * @return String representation of current RLGlue Interface version.
     * @since 2.1
     * 
     */
    public static String getSpecVersion() {
        String specAsString = RLGlueCore.class.getPackage().getSpecificationVersion();
        return specAsString;
    }

    /**
     * Get the Implementation (Build) version of RLGlue as set in the Manifest file.  
     * @return String representation of current RLGlue Build version.
     * @since 2.1
     */
    public static String getImplementationVersion() {
        return RLGlueCore.class.getPackage().getImplementationVersion();
    }
}
