package rlglue.network;

import java.io.IOException;
import java.net.InetSocketAddress;
import java.nio.ByteBuffer;
import java.nio.channels.SocketChannel;

public class Network
{
    protected SocketChannel socketChannel;
    protected ByteBuffer byteBuffer;

    public static final int kExperimentConnection  = 1;
    public static final int kAgentConnection       = 2;
    public static final int kEnvironmentConnection = 3;

    public static final int kAgentInit    = 4;
    public static final int kAgentStart   = 5;
    public static final int kAgentStep    = 6;
    public static final int kAgentEnd     = 7;
    public static final int kAgentCleanup = 8;
    public static final int kAgentFreeze  = 9;
    public static final int kAgentMessage = 10;

    public static final int kEnvInit          = 11;
    public static final int kEnvStart         = 12;
    public static final int kEnvStep          = 13;
    public static final int kEnvCleanup       = 14;
    public static final int kEnvSetState      = 15;
    public static final int kEnvSetRandomSeed = 16;
    public static final int kEnvGetState      = 17;
    public static final int kEnvGetRandomSeed = 18;
    public static final int kEnvMessage       = 19;

    public static final int kRLInit          = 20;
    public static final int kRLStart         = 21;
    public static final int kRLStep          = 22;
    public static final int kRLCleanup       = 23;
    public static final int kRLReturn        = 24;
    public static final int kRLNumSteps      = 25;
    public static final int kRLNumEpisodes   = 26;
    public static final int kRLEpisode       = 27;
    public static final int kRLSetState      = 28;
    public static final int kRLSetRandomSeed = 29;
    public static final int kRLGetState      = 30;
    public static final int kRLGetRandomSeed = 31;
    public static final int kRLFreeze        = 32;
    public static final int kRLAgentMessage  = 33;
    public static final int kRLEnvMessage    = 34;

    public static final String kDefaultHost = "127.0.0.1";
    public static final int kDefaultPort = 4096;
    public static final int kRetryTimeout = 10;

    public Network()
    {
	byteBuffer = ByteBuffer.allocateDirect(65536);
    }

    public void connect(String host, int port, int retryTimeout) throws IOException
    {
	InetSocketAddress address = new InetSocketAddress(host, port);
	socketChannel = SocketChannel.open();
	socketChannel.configureBlocking(true);
	socketChannel.connect(address);
    }

    public void close() throws IOException
    {
	socketChannel.close();
    }

    public SocketChannel getChannel()
    {
	return socketChannel;
    }

    public ByteBuffer getBuffer()
    {
	return byteBuffer;
    }

    public void sendBuffer()
    {
	socketChannel.write(byteBuffer);
    }

    public void recvBuffer()
    {
	socketChannel.read(byteBuffer);
    }
}

