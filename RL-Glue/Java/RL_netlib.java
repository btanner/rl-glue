import java.io.*;
import java.nio.*;
import java.nio.channels.*;
import java.net.*;

public class RL_netlib
{
    protected Socket socket;
    protected SocketChannel socketChannel;
    protected ByteBuffer byteBuffer;
    protected DataInputStream inputStream;
    protected DataOutputStream outputStream;

    public static final int kExperimentConnection = 1;
    public static final int kAgentConnection = 2;
    public static final int kEnvironmentConnection = 3;

    public static final int kAgentInit = 4;
    public static final int kAgentStart = 5;
    public static final int kAgentStep = 6;
    public static final int kAgentEnd = 7;
    public static final int kAgentCleanup = 8;
    public static final int kAgentFreeze = 30;
    public static final int kAgentMessage = 31;

    public static final int kEnvInit = 9;
    public static final int kEnvStart = 10;
    public static final int kEnvStep = 11;
    public static final int kEnvCleanup = 12;
    public static final int kEnvSetState = 13;
    public static final int kEnvSetRandomSeed = 14;
    public static final int kEnvGetState = 15;
    public static final int kEnvGetRandomSeed = 16;
    public static final int kEnvMessage = 32;

    public static final int kRLInit = 17;
    public static final int kRLStart = 18;
    public static final int kRLStep = 19;
 

    public static final int kRLCleanup = 20;
    public static final int kRLReturn = 21;
    public static final int kRLNumSteps = 22;
    public static final int kRLNumEpisodes = 23;
    public static final int kRLEpisode = 24;
    public static final int kRLSetState = 25;
    public static final int kRLSetRandomSeed = 26;
    public static final int kRLGetState = 27;
    public static final int kRLGetRandomSeed = 28;
    public static final int kRLFreeze = 29;
    public static final int kRLAgentMessage = 33;
    public static final int kRLEnvMessage = 34;

    public static final String kLocalHost = "127.0.0.1";
    public static final int kDefaultPort = 4096;
    public static final int kRetryTimeout = 10;


    public RL_netlib(String host, int port) throws Exception
    {
	/* Setup SocketChannel for communication */
	InetSocketAddress address = new InetSocketAddress(host, port);
	socketChannel = SocketChannel.open();
	socketChannel.configureBlocking(true);
	socketChannel.connect(address);
	socket = socketChannel.socket();
	socket.setTcpNoDelay(true);
	inputStream = new DataInputStream(socket.getInputStream());
	outputStream = new DataOutputStream(socket.getOutputStream());
    }

    public void close() throws Exception
    {
	inputStream.close();
	outputStream.close();
	socket.close();
    }

    public DataInputStream getInputStream() 
    {
	return inputStream;
    }

    public DataOutputStream getOutputStream() 
    {
	return outputStream;
    }

    public byte[] read(int numBytes) throws Exception
    {
	int bytesRecv = 0;
	int msgError = 0;

	byte[] buffer = new byte[numBytes];

	while (bytesRecv < numBytes) {
	    msgError = inputStream.read(buffer, bytesRecv, numBytes-bytesRecv);
	    if (msgError <= 0) { throw new IOException(); }
	    else { bytesRecv += msgError; }
	}
	return buffer;
    }

    public void writeObservation(Observation theObservation) throws IOException {
	outputStream.writeInt(theObservation.intArray.length);
	outputStream.writeInt(theObservation.doubleArray.length);

	for (int i = 0; i < theObservation.intArray.length; ++i) {
	    outputStream.writeInt(theObservation.intArray[i]);
	}

	for (int i = 0; i < theObservation.doubleArray.length; ++i) {
	    outputStream.writeDouble(theObservation.doubleArray[i]);
	}
    }

    public void writeAction(Action theAction) throws IOException {
	outputStream.writeInt(theAction.intArray.length);
	outputStream.writeInt(theAction.doubleArray.length);

	for (int i = 0; i < theAction.intArray.length; ++i) {
	    outputStream.writeInt(theAction.intArray[i]);
	}

	for (int i = 0; i < theAction.doubleArray.length; ++i) {
	    outputStream.writeDouble(theAction.doubleArray[i]);
	}
    }

    public void writeStateKey(StateKey theStateKey) throws IOException {
	outputStream.writeInt(theStateKey.intArray.length);
	outputStream.writeInt(theStateKey.doubleArray.length);

	for (int i = 0; i < theStateKey.intArray.length; ++i) {
	    outputStream.writeInt(theStateKey.intArray[i]);
	}

	for (int i = 0; i < theStateKey.doubleArray.length; ++i) {
	    outputStream.writeDouble(theStateKey.doubleArray[i]);
	}
    }

    public void writeRandomSeedKey(RandomSeedKey theRandomSeedKey) throws IOException {
	outputStream.writeInt(theRandomSeedKey.intArray.length);
	outputStream.writeInt(theRandomSeedKey.doubleArray.length);

	for (int i = 0; i < theRandomSeedKey.intArray.length; ++i) {
	    outputStream.writeInt(theRandomSeedKey.intArray[i]);
	}

	for (int i = 0; i < theRandomSeedKey.doubleArray.length; ++i) {
	    outputStream.writeDouble(theRandomSeedKey.doubleArray[i]);
	}
    }

    public Observation readObservation() throws IOException {
	int numInts = inputStream.readInt();
	int numDoubles = inputStream.readInt();

	Observation theObservation = new Observation(numInts, numDoubles);
	
	for (int i = 0; i < numInts; ++i) {
	    theObservation.intArray[i] = inputStream.readInt();
	}

	for (int i = 0; i < numDoubles; ++i) {
	    theObservation.doubleArray[i] = inputStream.readDouble();
	}

	return theObservation;
    }

    public Action readAction() throws IOException {
	int numInts = inputStream.readInt();
	int numDoubles = inputStream.readInt();

	Action theAction = new Action(numInts, numDoubles);
	
	for (int i = 0; i < numInts; ++i) {
	    theAction.intArray[i] = inputStream.readInt();
	}

	for (int i = 0; i < numDoubles; ++i) {
	    theAction.doubleArray[i] = inputStream.readDouble();
	}

	return theAction;
    }

    public StateKey readStateKey() throws IOException {
	int numInts = inputStream.readInt();
	int numDoubles = inputStream.readInt();

	StateKey theStateKey = new StateKey(numInts, numDoubles);
	
	for (int i = 0; i < numInts; ++i) {
	    theStateKey.intArray[i] = inputStream.readInt();
	}

	for (int i = 0; i < numDoubles; ++i) {
	    theStateKey.doubleArray[i] = inputStream.readDouble();
	}

	return theStateKey;
    }

    public RandomSeedKey readRandomSeedKey() throws IOException {
	int numInts = inputStream.readInt();
	int numDoubles = inputStream.readInt();

	RandomSeedKey theRandomSeedKey = new RandomSeedKey(numInts, numDoubles);
	
	for (int i = 0; i < numInts; ++i) {
	    theRandomSeedKey.intArray[i] = inputStream.readInt();
	}

	for (int i = 0; i < numDoubles; ++i) {
	    theRandomSeedKey.doubleArray[i] = inputStream.readDouble();
	}

	return theRandomSeedKey;
    }
}

