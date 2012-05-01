import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.net.InetAddress;
import java.net.Socket;
import java.net.SocketException;
import java.util.Arrays;


public class SimpleSocket {
	private Socket socket;
	private DataInputStream in;
	private DataOutputStream out;
	
	public SimpleSocket(InetAddress addr, int port) throws IOException {
		socket = new Socket(addr, port);
		in = new DataInputStream(socket.getInputStream());
		
		out = new DataOutputStream(socket.getOutputStream());
		
	}
	
	public SimpleSocket(String hostname, int port) throws IOException {
		socket = new Socket(hostname, port);
		in = new DataInputStream(socket.getInputStream());
		out = new DataOutputStream(socket.getOutputStream());
	}
	
	public SimpleSocket(Socket openSocket) throws IOException {
		socket = openSocket;
		in = new DataInputStream(socket.getInputStream());
		out = new DataOutputStream(socket.getOutputStream());
	}
	
	public byte readByte() throws IOException {
		return in.readByte();
	}
	
	public int readInt() throws IOException {
		return in.readInt();
	}
	
	public void writeBytes(byte[] bytes) throws IOException {
		out.write(bytes);
	}
	
	public void flush() throws IOException {
		out.flush();
	}
	
	public boolean isConnected() {
		return socket.isConnected();
	}
	
	public void close() throws IOException {
		in.close();
		out.close();
		socket.close();
	}
	
	public Socket getUnderlyingSocket() {
		return socket;
	}
	
	public void setTimeout(int millis) throws SocketException {
		socket.setSoTimeout(millis);
	}

}
