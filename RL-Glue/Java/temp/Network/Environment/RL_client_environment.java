import RL_netlib.*;

public class RL_client_environment {
	
	RL_netlib netlib;
	Environment environment;
	
	public RL_client_environment(RL_netlib netlib) {
		this.netlib = netlib;
	}
	
	protected void onEnvInit() {
	  String theTaskSpec = null;
	  int theTaskSpecLength = 0;
	  int offset = 0;

	  theTaskSpec = environment.env_init();
	  if (theTaskSpec != null) {
	    theTaskSpecLength = theTaskSpec.length() + 1;
	  }

		netlib.getOutputStream().writeInt(theTaskSpecLength);
		if (theTaskSpecLength > 0) {
			netlib.getOutputStream().writeChars(theTaskSpec);
		}
	}

	protected void onEnvStart() {
	  RLAbstractType theObservation = environment.env_start();
		netlib.writeAbstractType(theObservation);
	}

	protected void onEnvStep() {
	  RewardObservationTerm ro;
		RLAbstractType theAction = netlib.readAbstractType();

	  ro = environment.env_step(theAction);
		DataOutputStream o = netlib.getOutpuStream();
		o.writeInt(ro.isTerminal());
		o.writeDouble(ro.getReward());
		netlib.writeAbstractType(ro.getObservation());
	}

	protected void onEnvCleanup() {
	  environment.env_cleanup();
	}

	protected void onEnvSetState() {
		RLAbstractType theStateKey = netlib.readAbstractType();
	  environment.env_set_state(theStateKey);
	}

	protected void onEnvSetRandomSeed() {
		RLAbstractType theRandomSeedKey = netlib.readAbstractType();
	  environment.env_set_random_seed(theRandomSeedKey);
	}

	protected void onEnvGetState() {
	  RLAbstractType theStateKey = environment.env_get_state();
		netlib.sendAbstractType(theStateKey);
	}

	protected void onEnvGetRandomSeed() {
	  RLAbstractType theStateKey = environment.env_get_random_seed();
		netlib.sendAbstractType(theStateKey);
	}

 	protected void onEnvMessage() {
	  int inMessageLength = 0;
	  int outMessageLength = 0;
	  String inMessage = null;
	  String outMessage = null;
	
		inMessageLength = netlib.getInputStream().readInt();

	  if (inMessageLength > 0) {
			inMessage = netlib.getInputStream().readString();
	  }

	  outMessage = environment.env_message(inMessage);

	  if (outMessage != null) {
	   outMessageLength = outMessage.length() + 1;
	  }

		netlib.getOutputStream.writeInt(outMessageLength);

	  if (outMessageLength > 0) {
			netlib.getOutputStream.writeString(outMessage);
	  }
	}

	public void runEnvironmentEventLoop() {
	  int envState = 0;

	  do { 
			envState = netlib.getInputStream().readInt();

	    switch(envState) {
	    case RL_netlib.kEnvInit:
	      this.onEnvInit();
	      break;

	    case RL_netlib.kEnvStart:
	      this.onEnvStart();
	      break;

	    case RL_netlib.kEnvStep:
	      this.onEnvStep();
	      break;

	    case RL_netlib.kEnvCleanup:
	      this.onEnvCleanup();
	      break;

	    case RL_netlib.kEnvSetState:
	      this.onEnvSetState();
	      break;

	    case RL_netlib.kEnvSetRandomSeed:
	      this.onEnvSetRandomSeed();
	      break;

	    case RL_netlib.kEnvGetState:
	      this.onEnvGetState();
	      break;

	    case RL_netlib.kEnvGetRandomSeed:
	      this.onEnvGetRandomSeed();
	      break;

	    case RL_netlib.kEnvMessage:
	      this.onEnvMessage();
	      break;

	    default:
				System.err.print("Unknown message: ");
				System.err.println(envState);
	      System.exit(0);
	      break;
	    };
	  } while (envState != RL_netlib.kEnvCleanup);
	}

	public static void main(String[] args) {
	  String host = RL_netlib.kLocalHost;
	  String strPort = null;
		String strAutoReconnect = null;
	  int port = RL_netlib.kDefaultPort;
	  int autoReconnect = 0;

	  host = System.getProperty("RLGLUE_HOST");
	  if (host == null) {
	    host = RL_netlib.kLocalHost;
	  }

	  strPort = System.getProperty("RLGLUE_PORT");  
	  if (strPort != null) {
		  try {
	    	port = Integer.parseInt(strPort);
	    } catch(NumberFormatException e) {
	      port = RL_netlib.kDefaultPort;
	    }
			if (port < 0) {
				port = RL_netlib.kDefaultPort;
			}
	  }

	  strAutoReconnect = System.getProperty("RLGLUE_AUTORECONNECT");
	  if (strAutoReconnect != null) {
			try {
	    	autoReconnect = Integer.parseInt(strAutoReconnect);
	  	} catch(NumberFormatException e) {
				autoReconnect = 0;
			}
			if (autoReconnect < 0) {
				autoReconnect = 0;
			}
		}

		System.err.print("Connecting to host=");
		System.err.print(host);
		System.err.print(" on port=");
		System.err.print(port);
		System.err.print(" with autoreconnect=");
		System.err.println(autoReconnect);

	  do {
			RL_netlib netlib = new RL_netlib(host,port);
			netlib.getOutputStream().sendInt(RL_netlib.kEnvironmentConnection);
			RL_client_environment rce = new RL_client_environment(netlib);
	    rce.runEnvironmentEventLoop();
	  } while (autoReconnect > 0);

	}
}