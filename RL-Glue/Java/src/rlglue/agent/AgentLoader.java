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

import rlglue.network.Network;

/**
 * This class can be called from the command line to load an agent and create
 * an executable RL agent program.  
 * 
 * We've recently refactored it to make it useful if anyone ever wants to create
 * local instances of network-bound agents from inside a JVM (like Matlab)
 * @author btanner
 */
public class AgentLoader implements Runnable {

    String host = Network.kDefaultHost;
    int port = Network.kDefaultPort;
    int autoReconnect = 0;
    Agent theAgent = null;
    ClientAgent theClient = null;

    public AgentLoader(Agent theAgent) {
        this.theAgent = theAgent;
    }

    public AgentLoader(String hostString, String portString, String reconnectString, Agent theAgent) {
        this.theAgent = theAgent;

        if (hostString != null) {
            host = hostString;
        }

        try {
            port = Integer.parseInt(portString);
        } catch (Exception e) {
            port = Network.kDefaultPort;
        }

        try {
            autoReconnect = Integer.parseInt(reconnectString);
        } catch (Exception e) {
            autoReconnect = 0;
        }



    }

    /**
     * Loads the class agentClassName as an rl-glue agent.
     * @param envClassName
     */
    public static AgentLoader loadAgent(String agentClassName) {
        Agent agent = null;


        String hostString = System.getenv("RLGLUE_HOST");
        String portString = System.getenv("RLGLUE_PORT");
        String reconnectString = System.getenv("RLGLUE_AUTORECONNECT");

        try {
            agent = (Agent) Class.forName(agentClassName).newInstance();
        } catch (Exception ex) {
            System.err.println("loadAgent(" + agentClassName + ") threw Exception: " + ex);
        }
        AgentLoader theLoader = new AgentLoader(hostString, portString, reconnectString, agent);
        return theLoader;
    }

    public void killProcess() {
        theClient.killProcess();
    }

    public void run() {
        System.out.print("Connecting to " + host + " on port " + port + "...");
        ClientAgent theClient = new ClientAgent(theAgent);

        try {
            do {
                theClient.connect(host, port, Network.kRetryTimeout);
                System.out.println("Connected");
                theClient.runAgentEventLoop();
                theClient.close();
            } while (autoReconnect == 1);
        } catch (Exception e) {
            System.err.println("AgentLoader run(" + theAgent.getClass() + ") threw Exception: " + e);
        }
    }

    public static void main(String[] args) throws Exception {
        String usage = "java AgentLoader <Agent> -classpath <Path To RLGlue>";

        String envVars = "The following environment variables are used by the agent to control its function:\n" +
                "RLGLUE_HOST  : If set the agent will use this ip or hostname to connect to rather than " + Network.kDefaultHost + "\n" +
                "RLGLUE_PORT  : If set the agent will use this port to connect on rather than " + Network.kDefaultPort + "\n" +
                "RLGLUE_AUTORECONNECT  : If set the agent will reconnect to the glue after an experiment has finished\n";

        if (args.length < 1) {
            System.out.println(usage);
            System.out.println(envVars);
            System.exit(1);
        }

        AgentLoader theLoader = loadAgent(args[0]);
        theLoader.run();
    }
}
