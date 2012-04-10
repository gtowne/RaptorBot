import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.ServerSocket;
import java.net.InetSocketAddress;
import java.net.Socket;
import java.net.SocketException;
import java.util.concurrent.Semaphore;


public class RaptorVideoStreamClient extends Thread {
	
	private ServerSocket serverSocket;
	private SimpleSocket controlSocket;
	private DatagramSocket dataSocket;
	
	private InetAddress remoteInetAddr;
	private VideoPlayer clientPlayer;
	private int refreshMS;
		
	public RaptorVideoStreamClient(VideoPlayer _clientPlayer){
		clientPlayer = _clientPlayer;
	}
	
	public boolean init(SimpleSocket hostSocket) {
		try {
			serverSocket = new ServerSocket();
			serverSocket.bind(null); // let OS choose a port
		} catch (IOException e3) {
			System.out.println("Error opening server socket for video stream");
		}
		
		int myServerPort = serverSocket.getLocalPort();
		
		try {
			dataSocket = new DatagramSocket();
		} catch (SocketException e2) {
			System.out.println("Error opening data socket for video stream");
			return false;
		}
		
		int myDataPort = dataSocket.getLocalPort();
		
		System.out.printf("Data port %d, Server port %d\n", myDataPort, myServerPort);
		
		try {
			hostSocket.writeBytes(RaptorRemoteProtocol.newVideoStartPacket(myServerPort, myDataPort));
			hostSocket.flush();
		} catch (IOException e1) {
			System.out.println("Error sending video start request");
			return false;
		}
		
		System.out.println("Sent video start request message");
		
		try {
			serverSocket.setSoTimeout(10000);
		} catch (SocketException e1) {
			System.out.println("Error setting server socket timeout");
			return false;
		}
		
		try {
			Socket temp = serverSocket.accept();
			controlSocket = new SimpleSocket(temp);
		} catch (IOException e) {
			System.out.println("Error accepting conneciton on video stream server socket");
			return false;
		}
		
		remoteInetAddr = controlSocket.getUnderlyingSocket().getInetAddress();
		
		try {
			dataSocket.setSoTimeout(10000);
		} catch (SocketException e) {
			System.out.println("Error setting timeout for data socket");
		}
		
		DatagramPacket initRspPacket = new DatagramPacket(new byte[256], 256);
		
		try {
			dataSocket.receive(initRspPacket);
		} catch (IOException e) {
			System.out.println("Error receiving init response on data socket in video stream");
			return false;
		}
		
		System.out.println("Video client connected");
		
		return true;
	}
	
	
	public void run() {
		
		
		
	}
 	
}
