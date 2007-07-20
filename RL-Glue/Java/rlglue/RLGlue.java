package rlglue;

import java.nio.ByteBuffer;
import java.io.*;

public class RLGlue
{
    protected static Network network = null;
    protected static ByteBuffer headerBuffer;
    protected static ByteBuffer byteBuffer;
	
    private static void forceConnection()
    {
	if (network == null)
	{   
	    network = new Network();
	    headerBuffer = ByteBuffer.allocateDirect(8);
	    byteBuffer = ByteBuffer.allocateDirect(65536);
	    
	    // Connect
	    network.connect(Network.kDefaultHost, 
			    Network.kDefaultPort, 
			    Network.kRetryTimeout);
      	    
    	    byteBuffer.clear();
	    byteBuffer.putInt(Network.kExperimentConnection);
	    byteBuffer.putInt(0);
	    byteBuffer.flip();

	    try 
	    {
		network.send(byteBuffer);
	    }
	    catch(IOException ioException)
	    {
		ioException.printStackTrace();
		System.exit(1);
	    }
	}
    }
	
    public static void RL_init()
    {
	forceConnection();

	try
	{
	    // Call RL_init
	    byteBuffer.clear();
	    byteBuffer.putInt(Network.kRLInit);
	    byteBuffer.putInt(0);
	    byteBuffer.flip();
	    network.send(byteBuffer);
	    
	    // Recv reply from RL_init
	    headerBuffer.clear();
	    network.recv(headerBuffer, 8);
	    headerBuffer.flip();
	    
	    int glueState = headerBuffer.getInt();
	    int dataSize = headerBuffer.getInt();
	    
	    if (glueState != Network.kRLInit)
	    {
		System.err.println("RL_init not synched with server\n");
		System.exit(1);
	    }
	}
	catch(IOException ioException)
	{
	    ioException.printStackTrace();
	    System.exit(1);
	}
    }
	
    public static Observation_action RL_start()
    {
	if (network == null) {
	    System.out.println("You must call RL_init before calling RL_start");
	    System.exit(1);
	}

	try
	{
	    Observation_action obsact = new Observation_action();
	    byteBuffer.clear();
	    byteBuffer.putInt(Network.kRLStart);
	    byteBuffer.putInt(0);
	    byteBuffer.flip();
	    network.send(byteBuffer);
	    
	    headerBuffer.clear();
	    network.recv(headerBuffer, 8);
	    headerBuffer.flip();
	    
	    int glueState = headerBuffer.getInt();
	    int dataSize = headerBuffer.getInt();
	    
	    if (glueState != Network.kRLStart)
	    {
		System.err.println("RL_start not synched with server\n");
		System.exit(1);
	    }
		
	    byteBuffer.clear();
	    network.recv(byteBuffer, dataSize);
	    byteBuffer.flip();
	    
	    obsact.o = Network.getObservation(byteBuffer);
	    obsact.a = Network.getAction(byteBuffer);
	    return obsact;
	}
	catch (IOException ioException)
	{
	    ioException.printStackTrace();
	    System.exit(1);
	    return null; // We will never get here.
	}
    }
	
    public static Reward_observation_action_terminal RL_step()
    {
	if (network == null) {
	    System.out.println("You must call RL_init before calling RL_step");
	    System.exit(1);
	}

	try 
	{
	    Reward_observation_action_terminal roat = new Reward_observation_action_terminal();
	    
	    byteBuffer.clear();
	    byteBuffer.putInt(Network.kRLStep);
	    byteBuffer.putInt(0);
	    byteBuffer.flip();
	    network.send(byteBuffer);
	    
	    headerBuffer.clear();
	    network.recv(headerBuffer, 8);
	    headerBuffer.flip();
	    
	    int glueState = headerBuffer.getInt();
	    int dataSize = headerBuffer.getInt();
	    
	    if (glueState != Network.kRLStep)
	    {
		System.err.println("RL_step not synched with server\n");
		System.exit(1);
	    }
	    
	    byteBuffer.clear();
	    network.recv(byteBuffer, dataSize);
	    byteBuffer.flip();
	    
	    roat.terminal = byteBuffer.getInt();
	    roat.r = byteBuffer.getDouble();
	    roat.o = Network.getObservation(byteBuffer);
	    roat.a = Network.getAction(byteBuffer);
	    
	    return roat;
	}
	catch (IOException ioException)
	{
	    ioException.printStackTrace();
	    System.exit(1);
	    return null; // We will never get here.
	}
    }
	
    public static void RL_cleanup()
    {
	if (network == null) {
	    System.out.println("You must call RL_init before calling RL_cleanup");
	    System.exit(1);
	}

	try
	{
	    byteBuffer.clear();
	    byteBuffer.putInt(Network.kRLCleanup);
	    byteBuffer.putInt(0);
	    byteBuffer.flip();
	    network.send(byteBuffer);
	    
	    headerBuffer.clear();
	    network.recv(headerBuffer, 8);
	    headerBuffer.flip();
	    
	    int glueState = headerBuffer.getInt();
	    int dataSize = headerBuffer.getInt();
	    
	    if (glueState != Network.kRLCleanup)
	    {
		System.err.println("RL_cleanup not synched with server\n");
		System.exit(1);
	    }
	    
	    byteBuffer.clear();
	    network.close();
	}
	catch (IOException ioException)
	{
	    ioException.printStackTrace();
	    System.exit(1);
	}
    }
	
    public static String RL_agent_message(String message)
    {
	forceConnection();

	try
	{
	    byteBuffer.clear();
	    byteBuffer.putInt(Network.kRLAgentMessage);
	    byteBuffer.putInt(message.length()+4);
	    if (message.length() > 0) {
		Network.putString(byteBuffer, message);
	    }
	    byteBuffer.flip();
	    network.send(byteBuffer);
	    
	    headerBuffer.clear();
	    network.recv(headerBuffer, 8);
	    headerBuffer.flip();
	    
	    int glueState = headerBuffer.getInt();
	    int dataSize = headerBuffer.getInt();
	    
	    if (glueState != Network.kRLAgentMessage)
	    {
		System.err.println("RL_agent_message not synched with server\n");
		System.exit(1);
	    }
	    
	    byteBuffer.clear();
	    network.recv(byteBuffer, dataSize);
	    byteBuffer.flip();
	    
	    String response = network.getString(byteBuffer);
	    return response;
	}
	catch (IOException ioException)
	{
	    ioException.printStackTrace();
	    System.exit(1);
	    return null; // We will never get here.
	}
    }
	
    public static String RL_env_message(String message)
    {
	forceConnection();

	try
	{
	    byteBuffer.clear();
	    byteBuffer.putInt(Network.kRLEnvMessage);
	    byteBuffer.putInt(message.length()+4);
	    if (message.length() > 0) {
		Network.putString(byteBuffer, message);
	    }
	    byteBuffer.flip();
	    network.send(byteBuffer);
	    
	    headerBuffer.clear();
	    network.recv(headerBuffer, 8);
	    headerBuffer.flip();
	    
	    int glueState = headerBuffer.getInt();
	    int dataSize = headerBuffer.getInt();
	    
	    if (glueState != Network.kRLEnvMessage)
	    {
		System.err.println("RL_env_message not synched with server\n");
		System.exit(1);
	    }
	    
	    byteBuffer.clear();
	    network.recv(byteBuffer, dataSize);
	    byteBuffer.flip();
	    
	    String response = network.getString(byteBuffer);
	    return response;
	}
	catch (IOException ioException)
	{
	    ioException.printStackTrace();
	    System.exit(1);
	    return null;
	}
    }
	
    public static double RL_return()
    {
	if (network == null) {
	    System.out.println("You must call RL_init before calling RL_return");
	    System.exit(1);
	}

	try
	{
	    byteBuffer.clear();
	    byteBuffer.putInt(Network.kRLReturn);
	    byteBuffer.putInt(0);
	    byteBuffer.flip();
	    network.send(byteBuffer);
	    
	    headerBuffer.clear();
	    network.recv(headerBuffer, 8);
	    headerBuffer.flip();
	    
	    int glueState = headerBuffer.getInt();
	    int dataSize = headerBuffer.getInt();
	    
	    if (glueState != Network.kRLReturn)
	    {
		System.err.println("RL_return not synched with server\n");
		System.exit(1);
	    }
	    
	    byteBuffer.clear();
	    network.recv(byteBuffer, dataSize);
	    byteBuffer.flip();
	    
	    double reward = byteBuffer.getDouble();
	    return reward;
	}
	catch (IOException ioException)
	{
	    ioException.printStackTrace();
	    System.exit(1);
	    return 0;
	}
    }
	
    public static int RL_num_steps()
    {
	if (network == null) {
	    System.out.println("You must call RL_init before calling RL_num_steps");
	    System.exit(1);
	}

	try
	{
	    byteBuffer.clear();
	    byteBuffer.putInt(Network.kRLNumSteps);
	    byteBuffer.putInt(0);
	    byteBuffer.flip();
	    network.send(byteBuffer);
	    
	    headerBuffer.clear();
	    network.recv(headerBuffer, 8);
	    headerBuffer.flip();
	    
	    int glueState = headerBuffer.getInt();
	    int dataSize = headerBuffer.getInt();
	    
	    if (glueState != Network.kRLNumSteps)
	    {
		System.err.println("RL_num_steps not synched with server\n");
		System.exit(1);
	    }
	    
	    byteBuffer.clear();
	    network.recv(byteBuffer, dataSize);
	    byteBuffer.flip();
	    
	    int numSteps = byteBuffer.getInt();
	    return numSteps;
	}
	catch (IOException ioException)
	{
	    ioException.printStackTrace();
	    System.exit(1);
	    return 0;
	}
    }
	
    public static int RL_num_episodes()
    {
	if (network == null) {
	    System.out.println("You must call RL_init before calling RL_num_episodes");
	    System.exit(1);
	}

	try
	{
	    byteBuffer.clear();
	    byteBuffer.putInt(Network.kRLNumEpisodes);
	    byteBuffer.putInt(0);
	    byteBuffer.flip();
	    network.send(byteBuffer);
	    
	    headerBuffer.clear();
	    network.recv(headerBuffer, 8);
	    headerBuffer.flip();
	    
	    int glueState = headerBuffer.getInt();
	    int dataSize = headerBuffer.getInt();
	    
	    if (glueState != Network.kRLNumEpisodes)
	    {
		System.err.println("RL_num_episodes not synched with server\n");
		System.exit(1);
	    }
	    
	    byteBuffer.clear();
	    network.recv(byteBuffer, dataSize);
	    byteBuffer.flip();
	    
	    int numEpisodes = byteBuffer.getInt();
	    return numEpisodes;	
	}
	catch (IOException ioException)
	{
	    ioException.printStackTrace();
	    System.exit(1);
	    return 0;
	}
    }
	
    public static void RL_episode(int numSteps)
    {
	if (network == null) {
	    System.out.println("You must call RL_init before calling RL_episode");
	    System.exit(1);
	}

	try
	{
	    byteBuffer.clear();
	    byteBuffer.putInt(Network.kRLEpisode);
	    byteBuffer.putInt(4);
	    byteBuffer.putInt(numSteps);
	    byteBuffer.flip();
	    network.send(byteBuffer);
	    
	    headerBuffer.clear();
	    network.recv(headerBuffer, 8);
	    headerBuffer.flip();
	    
	    int glueState = headerBuffer.getInt();
	    int dataSize = headerBuffer.getInt();
	    
	    if (glueState != Network.kRLEpisode)
	    {
		System.err.println("RL_episode not synched with server\n");
		System.exit(1);
	    }
	}
	catch (IOException ioException)
	{
	    ioException.printStackTrace();
	    System.exit(1);
	}
    }
	
    public static void RL_freeze()
    {
	if (network == null) {
	    System.out.println("You must call RL_init before calling RL_freeze");
	    System.exit(1);
	}

	try
	{
	    byteBuffer.clear();
	    byteBuffer.putInt(Network.kRLFreeze);
	    byteBuffer.putInt(0);
	    byteBuffer.flip();
	    network.send(byteBuffer);
	    
	    headerBuffer.clear();
	    network.recv(headerBuffer, 8);
	    headerBuffer.flip();
	    
	    int glueState = headerBuffer.getInt();
	    int dataSize = headerBuffer.getInt();
	    
	    if (glueState != Network.kRLFreeze)
	    {
		System.err.println("RL_freeze not synched with server\n");
		System.exit(1);
	    }
	}
	catch (IOException ioException)
	{
	    ioException.printStackTrace();
	    System.exit(1);
	}
    }
	
    public static void RL_set_state(State_key sk)
    {
	if (network == null) {
	    System.out.println("You must call RL_init before calling RL_set_state");
	    System.exit(1);
	}

	try
	{
	    int size = (4 + sk.intArray.length * 4) + (4 + sk.doubleArray.length * 8);
	    
	    byteBuffer.clear();
	    byteBuffer.putInt(Network.kRLSetState);
	    byteBuffer.putInt(size);
	    network.putStateKey(byteBuffer, sk);
	    byteBuffer.flip();
	    network.send(byteBuffer);
	    
	    headerBuffer.clear();
	    network.recv(headerBuffer, 8);
	    headerBuffer.flip();
	    
	    int glueState = headerBuffer.getInt();
	    int dataSize = headerBuffer.getInt();
	    
	    if (glueState != Network.kRLSetState)
	    {
		System.err.println("RL_set_state not synched with server\n");
		System.exit(1);
	    }
	}
	catch (IOException ioException)
	{
	    ioException.printStackTrace();
	    System.exit(1);
	}
    }
	
    public static void RL_set_random_seed(Random_seed_key rsk)
    {
	if (network == null) {
	    System.out.println("You must call RL_init before calling RL_set_random_seed");
	    System.exit(1);
	}

	try
	{
	    int size = (4 + rsk.intArray.length * 4) + (4 + rsk.doubleArray.length * 8);
	    
	    byteBuffer.clear();
	    byteBuffer.putInt(Network.kRLSetRandomSeed);
	    byteBuffer.putInt(size);
	    network.putRandomSeedKey(byteBuffer, rsk);
	    byteBuffer.flip();
	    network.send(byteBuffer);
	    
	    headerBuffer.clear();
	    network.recv(headerBuffer, 8);
	    headerBuffer.flip();
		
	    int glueState = headerBuffer.getInt();
	    int dataSize = headerBuffer.getInt();
	    
	    if (glueState != Network.kRLSetRandomSeed)
	    {
		System.err.println("RL_set_random_seed not synched with server\n");
		System.exit(1);
	    }
	}
	catch (IOException ioException)
	{
	    ioException.printStackTrace();
	    System.exit(1);
	}
    }
	
    public static State_key RL_get_state()
    {
	if (network == null) {
	    System.out.println("You must call RL_init before calling RL_get_state");
	    System.exit(1);
	}

	try
	{
	    byteBuffer.clear();
	    byteBuffer.putInt(Network.kRLGetState);
	    byteBuffer.putInt(0);
	    byteBuffer.flip();
	    network.send(byteBuffer);
	    
	    headerBuffer.clear();
	    network.recv(headerBuffer, 8);
	    headerBuffer.flip();
	    
	    int glueState = headerBuffer.getInt();
	    int dataSize = headerBuffer.getInt();
	    
	    if (glueState != Network.kRLGetState)
	    {
		System.err.println("RL_get_state not synched with server\n");
		System.exit(1);
	    }
	    
	    byteBuffer.clear();
	    network.recv(byteBuffer, dataSize);
	    byteBuffer.flip();
	    
	    State_key key = network.getStateKey(byteBuffer);
	    return key;
	}
	catch (IOException ioException)
	{
	    ioException.printStackTrace();
	    System.exit(1);
	    return null; // We will never get here.
	}
    }
	
    public static Random_seed_key RL_get_random_seed()
    {
	if (network == null) {
	    System.out.println("You must call RL_init before calling RL_get_random_seed");
	    System.exit(1);
	}

	try
	{
	    byteBuffer.clear();
	    byteBuffer.putInt(Network.kRLGetRandomSeed);
	    byteBuffer.putInt(0);
	    byteBuffer.flip();
	    network.send(byteBuffer);
	    
	    headerBuffer.clear();
	    network.recv(headerBuffer, 8);
	    headerBuffer.flip();
	    
	    int glueState = headerBuffer.getInt();
	    int dataSize = headerBuffer.getInt();
	    
	    if (glueState != Network.kRLGetRandomSeed)
	    {
		System.err.println("RL_get_random_seed not synched with server\n");
		System.exit(1);
	    }
	    
	    byteBuffer.clear();
	    network.recv(byteBuffer, dataSize);
	    byteBuffer.flip();
	    
	    Random_seed_key key = network.getRandomSeedKey(byteBuffer);
	    return key;
	}
	catch (IOException ioException)
	{
	    ioException.printStackTrace();
	    System.exit(1);
	    return null; // We will never get here.
	}
    }
}
