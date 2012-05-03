package sessionManagement;
import java.io.IOException;
import java.net.InetAddress;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.Hashtable;
import java.util.Map.Entry;

import userInterface.RaptorRemoteUserInterface;


public class RaptorRemoteClient {
	public static final int ONBOARD_LISTENING_PORT = 7234;
	
	public static RaptorRemoteSession initNewSession(RaptorRemoteUserInterface ui, String hostIP) {
		ServerSocket feedbackServerSocket;
		try {
			feedbackServerSocket = new ServerSocket();
			feedbackServerSocket.bind(null);
		} catch (IOException e) {
			return null;
		}
		
		SimpleSocket socket = null;
		try {
			socket = new SimpleSocket(hostIP, 7234);
		} catch (IOException e) {
			return null;
		}
		
		SimpleSocket feedbackSocket = null;
		
		try {
			socket.writeBytes(RaptorRemoteProtocol.newInitSessionPacket(getMyIP(), feedbackServerSocket.getLocalPort()));
			feedbackServerSocket.setSoTimeout(2000);
			feedbackSocket = new SimpleSocket(feedbackServerSocket.accept());
			
			if (feedbackSocket.isConnected()) {
				System.out.println("RaptorRemoteClient::Feedback Socket connected");
			} else {
				System.out.println("RaptorRemoteClient::Error, Feedback Socket not connected");
				throw new IOException();
			}
			
		} catch (IOException e) {
			System.out.println("RaptorRemoteClient::Could not connect to device");
			return null;
		}
		
		RaptorSessionMessage initResponse = RaptorRemoteProtocol.readFromSocket(socket);
		
		RaptorRemoteSession newSession = null;
		
		
		if (initResponse.type == RaptorSessionMessage.MessageType.INIT_RSP) {
			System.out.println("RaptorRemoteClient::Received Init Response Message");
			
			if (initResponse.success) {
				newSession = new RaptorRemoteSession(ui,socket, feedbackSocket);
			} else {
				System.out.println("RaptorRemoteClient::Error, received failed init response message, could not init session");
				return null;
			}
		} else {
			return null;
		}
		
		return newSession;
	}
		
	public static Hashtable<String,String> pollNetworkDevices() {
		Hashtable<String, String> results = new Hashtable<String, String>();
		
		int numThreads = 30;
		int pollsPerThread = 256/20;
		
		String[] resultsArr = new String[256];
		
		NetworkPoller[] threads = new NetworkPoller[numThreads];
		
		for(int i = 0; i < numThreads; i++) {
			threads[i] = new NetworkPoller(7234, i * pollsPerThread, Math.min((i+1) * pollsPerThread,255), resultsArr);
			threads[i].start();
		}
		
		
		for (int i = 0; i < numThreads; i++) {
			try {
				threads[i].join();
			} catch (InterruptedException e) {
				return results;
			}
		}
		
		String myIP = getMyIP();
		int dotIndex = myIP.lastIndexOf('.');
		String lanAddrBase = myIP.substring(0, dotIndex+1);
		
		for (int i = 0; i < 256; i++) {
			if (resultsArr[i] != null) {
				results.put(lanAddrBase + i, resultsArr[i]);
			}
		}
		
		return results;
	}
	
	private static class NetworkPoller extends Thread {
		int port;
		int firstToTry;
		int upTo;
		String[] resultsArr;
		SimpleSocket socket;
		
		public NetworkPoller(int port, int firstToTry, int upTo, String[] resultsArr) {
			this.port = port;
			this.firstToTry = firstToTry;
			this.upTo = upTo;
			this.resultsArr = resultsArr;
			this.socket = null;
		}
		
		public void run() {
			String myIP = getMyIP();
			int dotIndex = myIP.lastIndexOf('.');
			String lanAddrBase = myIP.substring(0, dotIndex+1);
			
			int myExtension = Integer.parseInt(myIP.substring(dotIndex+1, myIP.length()));
			
			for (int i = firstToTry; i < upTo; i++) {
				resultsArr[i] = null;
				//if (i != myExtension) {
										
					try {
						socket = new SimpleSocket();
						socket.setTimeout(300);
						socket.connect(lanAddrBase + i, port, 300);
					} catch (IOException e) {
						try{socket.close();} catch (Exception _e) {continue;}
						continue;
					}
					
					try {
						socket.writeBytes(RaptorRemoteProtocol.newPingPacket());
						socket.flush();
					} catch (IOException e) {
						try{socket.close();} catch (Exception _e) {continue;}
						continue;
					}
					
					RaptorSessionMessage response = RaptorRemoteProtocol.readFromSocket(socket);
					
					if (response.type == RaptorSessionMessage.MessageType.PING_RSP) {
						resultsArr[i] = response.machineName;
					}
						
					try{socket.close();} catch (Exception _e) {continue;}
				//}
			}
		}
	}
	
	public static String getMyIP() {  
	    String addr=null;  
	    try  
	    {  
	        InetAddress ip=InetAddress.getLocalHost();  
	        addr = ip.getHostAddress();
	          
	    }catch(Exception e){e.printStackTrace();}  
	    return addr;  
	}  
}
