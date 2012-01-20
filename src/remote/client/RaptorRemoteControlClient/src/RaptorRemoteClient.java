import java.io.IOException;
import java.net.InetAddress;


public class RaptorRemoteClient {
	// this is the port the server is listening on
	private static final int SERVER_PORT_NUMER = 7234;
	
	// timeout if we haven't received a response to our ping
	// within this many miliseconds
	private static final int PING_RSP_TIMEOUT_MILLIS = 5000;
	
	
	SimpleSocket socket;
	
	
	public RaptorRemoteClient() {
	}
	
	public void connect(InetAddress hostAddr) throws IOException {
		socket = new SimpleSocket(hostAddr, SERVER_PORT_NUMER);
	}
	
	public boolean ping(InetAddress hostAddr) throws IOException {
		SimpleSocket tempSocket = new SimpleSocket(hostAddr, SERVER_PORT_NUMER);
		
		
		if (tempSocket.isConnected()) {
						
			tempSocket.write(RaptorRemoteProtocol.newPingPacket());
			
			tempSocket.flush();
			
			tempSocket.getUnderlyingSocket().setSoTimeout(PING_RSP_TIMEOUT_MILLIS);
			
			char[] buff = new char[10];
			int bytesRead = tempSocket.read(buff, 0, 2);
			
			if (bytesRead == 2 && buff[0] == RaptorRemoteProtocol.RAPTOR_REMOTE_SESSION_PROTOCOL_VERSION_ID &&
					buff[1] == RaptorRemoteProtocol.PING_RSP_MSG) {
				System.out.println("PING: Response received from host");
			} else {
				System.out.println("PING: No valid response received from host");
			}
						
			tempSocket.close();
			
			return true;
		}
		
		tempSocket.close();
		
		System.out.println("PING: failed to connect");
		
		return false;
	}
	
	public boolean disconnect() {
		return false;
	}
	
	public boolean send(char[] buffer) {
		return false;
		
	}
}
