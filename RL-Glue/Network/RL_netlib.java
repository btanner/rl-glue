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
    protected BufferedReader stdin;

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
}
