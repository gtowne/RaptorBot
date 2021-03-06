import java.io.IOException;
import java.net.InetAddress;


public class RaptorRemoteClient {
	// this is the port the server is listening on
	private static final int SERVER_PORT_NUMER = 7234;
	
	// timeout if we haven't received a response to our ping
	// within this many milliseconds
	private static final int RSP_TIMEOUT_MILLIS = 5000;
	
	private SimpleSocket socket;
	private RaptorVideoStreamClient videoStreamClient;
	
	private boolean connected;
	
	
	public RaptorRemoteClient() {
		connected = false;
	}
	
	public boolean connect(InetAddress hostAddr) throws IOException {
		socket = new SimpleSocket(hostAddr, SERVER_PORT_NUMER);
		
		if (socket.isConnected()) {
			
			socket.writeBytes(RaptorRemoteProtocol.newInitPacket());
			
			socket.flush();
			
			socket.setTimeout(RSP_TIMEOUT_MILLIS);
			
			RaptorSessionMessage response = RaptorRemoteProtocol.readFromSocket(socket);
			
			if (response != null && response.type == RaptorSessionMessage.MessageType.INIT_RSP && response.success) {
				connected = true;
				return true;
			}
			
		}	
		
		socket.close();
				
		return false;
	}
	
	public boolean disconnect() throws IOException {
		if (this.isConnected()) {
			socket.writeBytes(RaptorRemoteProtocol.newQuitPacket());
			
			socket.flush();
			
			RaptorSessionMessage response = RaptorRemoteProtocol.readFromSocket(socket);
			
			if (response != null && response.type == RaptorSessionMessage.MessageType.QUIT_RSP) {
				connected = false;
				return true;
			}
		}
		
		return false;
	}
	
	public boolean ping(InetAddress hostAddr) throws IOException {
		SimpleSocket tempSocket = new SimpleSocket(hostAddr, SERVER_PORT_NUMER);
		
		
		if (tempSocket.isConnected()) {
						
			tempSocket.writeBytes(RaptorRemoteProtocol.newPingPacket());
			
			tempSocket.flush();
			
			tempSocket.setTimeout(RSP_TIMEOUT_MILLIS);
			
			RaptorSessionMessage response = RaptorRemoteProtocol.readFromSocket(tempSocket);
			
			if (response.type == RaptorSessionMessage.MessageType.PING_RSP) {
				tempSocket.close();
				return true;
			}
		}
		
		tempSocket.close();
				
		return false;
	}
	
	public boolean isConnected() {
		return connected && socket.isConnected();
	}

	public boolean initVideoStream(VideoPlayer clientPlayer) {
		if (this.isConnected()) {
			videoStreamClient = new RaptorVideoStreamClient(clientPlayer);
			
			if (videoStreamClient.init(socket)) {
				videoStreamClient.start();
				return true;
			}
		}
		
		return false;
		
	}

	public boolean stopVideoStream() {
		if (this.isConnected()) {
			try {
				socket.writeBytes(RaptorRemoteProtocol.newVideoEndPacket());
				socket.flush();
			} catch (IOException e) {
				return false;
			}
			
			RaptorSessionMessage response = RaptorRemoteProtocol.readFromSocket(socket);
			
			if (response.type == RaptorSessionMessage.MessageType.VID_END_RSP &&
					response.success) {
				return true;
			}
		}
		
		return false;
		
	}
	

}
