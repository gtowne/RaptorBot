package sessionManagement;

import java.io.IOException;

import userInterface.RaptorRemoteUserInterface;
import videoStreaming.VideoStreamer;


public class RaptorRemoteSession {
	private SimpleSocket controlSocket;
	
	private RaptorRemoteUserInterface thisUI;
	
	public RaptorRemoteSession(RaptorRemoteUserInterface thisUI, SimpleSocket controlSocket) {
		this.controlSocket = controlSocket;
		this.thisUI = thisUI;
	}
	
	public boolean isActive() {
		return controlSocket.isConnected();
	}
	
	public boolean initVideoStream(int camNum, int targetFPS) {		
		return sendVideoInitMessage(RaptorRemoteClient.getMyIP(), VideoStreamer.LOCAL_PORT_NO, camNum, targetFPS);
	}
	
	private boolean sendVideoInitMessage(String thisIP, int thisPort, int camNum, int targetFPS) {
		
		try {
			controlSocket.writeBytes(RaptorRemoteProtocol.newVideoInitPacket(thisIP, thisPort, camNum, targetFPS));
		} catch (IOException e) {
			return false;
		}
		
		RaptorSessionMessage reply = RaptorRemoteProtocol.readFromSocket(controlSocket);
		
		if (reply == null) {
			return false;
		}
		
		if (reply.type != RaptorSessionMessage.MessageType.VID_START_RSP) {
			return false;
		}
		
		return reply.success;
		
	}
	
}
