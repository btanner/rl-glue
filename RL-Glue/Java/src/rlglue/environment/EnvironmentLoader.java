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
package rlglue.environment;

import rlglue.network.Network;

/**
 * This class can be called from the command line to load an environment and create
 * an executable RL environment program.  
 * 
 * We've recently refactored it to make it useful if anyone ever wants to create
 * local instances of network-bound environment from inside a JVM (like Matlab)
 * @author btanner
 */
public class EnvironmentLoader implements Runnable {

    String host = Network.kDefaultHost;
    int port = Network.kDefaultPort;
    int autoReconnect = 0;
    Environment theEnvironment = null;
    ClientEnvironment theClient = null;

    public EnvironmentLoader(Environment theEnvironment) {
        this.theEnvironment = theEnvironment;
    }

    public EnvironmentLoader(String hostString, String portString, String reconnectString, Environment theEnvironment) {
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

    public void killProcess() {
        theClient.killProcess();
    }

    public void run() {
        System.out.print("Connecting to " + host + " on port " + port + "...");
        theClient = new ClientEnvironment(theEnvironment);
        try {
            do {
                theClient.connect(host, port, Network.kRetryTimeout);
                System.out.println("Connected");
                theClient.runEnvironmentEventLoop();
                theClient.close();
            } while (autoReconnect == 1);
        } catch (Exception e) {
            System.err.println("EnvironmentLoader run(" + theEnvironment.getClass() + ") threw Exception: " + e);
        }
    }

    /**
     * Loads the class envClassName as an rl-glue environment.
     * @param envClassName
     */
    public static EnvironmentLoader loadEnvironment(String envClassName) {
        Environment env = null;

        String hostString = System.getenv("RLGLUE_HOST");
        String portString = System.getenv("RLGLUE_PORT");
        String reconnectString = System.getenv("RLGLUE_AUTORECONNECT");

        try {
            env = (Environment) Class.forName(envClassName).newInstance();
        } catch (Exception e) {
            System.err.println("loadEnvironment(" + envClassName + ") threw Exception: " + e);
        }

        return new EnvironmentLoader(hostString, portString, reconnectString, env);
    }

    public static void main(String[] args) throws Exception {
        String usage = "java EnvironmentLoader <Environment> -classpath <Path To RLGlue>";

        String envVars = "The following environment variables are used by the environment to control its function:\n" +
                "RLGLUE_HOST  : If set the environment will use this ip or hostname to connect to rather than " + Network.kDefaultHost + "\n" +
                "RLGLUE_PORT  : If set the environment will use this port to connect on rather than " + Network.kDefaultPort + "\n" +
                "RLGLUE_AUTORECONNECT  : If set the enviroment will reconnect to the glue after an experiment has finished\n";

        if (args.length < 1) {
            System.out.println(usage);
            System.out.println(envVars);
            System.exit(1);
        }
        EnvironmentLoader theEnvironmentLoader = loadEnvironment(args[0]);
        theEnvironmentLoader.run();
    }
}
