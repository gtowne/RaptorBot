import java.io.IOException;



public class RaptorRemoteProtocol {
	
	/*
	 * Protocol Version
	 */
	public static final char RAPTOR_REMOTE_SESSION_PROTOCOL_VERSION_ID = 0x7d;

	/*
	 * Message Types
	 */
	public static final char PING_MSG = 'p';
	public static final char PING_RSP_MSG = 'P';
	public static final char INIT_MSG = 'i';
	public static final char INIT_RSP_MSG = 'I';
	public static final char QUIT_MSG = 'q';
	public static final char QUIT_RSP_MSG = 'Q';
	public static final char CMD_MSG = 'c';
	
	
	public static final char VID_START = 's';
	public static final char VID_START_RSP = 'S';
	public static final char VID_END = 'e';
	public static final char VID_END_RSP = 'E';
	public static final char VID_DATA = 'd';
	
	private static final int MAX_PACKET_BYTES = 256;
	private static final int INIT_PACKET_BYTES = 2;
	private static final int QUIT_PACKET_BYTES = 2;
	public static final int VID_START_PACKET_BYTES = 2;
	public static final int VID_END_PACKET_BYTES = 2;
	
	public static char[] newPingPacket() {
		char[] packet = {RAPTOR_REMOTE_SESSION_PROTOCOL_VERSION_ID, PING_MSG};
						
		return packet;
	}
	
	public static char[] newInitPacket() {
		char[] packet = new char[INIT_PACKET_BYTES];
		
		packet[0] = RAPTOR_REMOTE_SESSION_PROTOCOL_VERSION_ID;
		packet[1] = INIT_MSG;
						
		return packet;
	}
	
	public static char[] newQuitPacket() {
		char[] packet = new char[QUIT_PACKET_BYTES];
		
		packet[0] = RAPTOR_REMOTE_SESSION_PROTOCOL_VERSION_ID;
		packet[1] = QUIT_MSG;
		
		return packet;
	}
	
	public static char[] newVideoStartPacket() {
		char[] packet = new char[VID_START_PACKET_BYTES];
		
		packet[0] = RAPTOR_REMOTE_SESSION_PROTOCOL_VERSION_ID;
		packet[1] = VID_START;
		
		return packet;
	}
	
	public static char[] newVideoEndPacket() {
		char[] packet = new char[VID_END_PACKET_BYTES];
		
		packet[0] = RAPTOR_REMOTE_SESSION_PROTOCOL_VERSION_ID;
		packet[1] = VID_END;
		
		return packet;
	}
	
	public static RaptorSessionMessage readFromSocket(SimpleSocket socket) {
		RaptorSessionMessage message = new RaptorSessionMessage(MAX_PACKET_BYTES);
				
		int bytesRead;
		try {
			bytesRead = socket.read(message.rawPacket, 0, 1);
		} catch (IOException e1) {
			message.type = RaptorSessionMessage.MessageType.SOCKET_ERROR;
			return message;
		}
		
		if (bytesRead != 1) {
			message.type = RaptorSessionMessage.MessageType.SOCKET_ERROR;
			return message;
		} else if (message.rawPacket[0] != RaptorRemoteProtocol.RAPTOR_REMOTE_SESSION_PROTOCOL_VERSION_ID) {
			message.type = RaptorSessionMessage.MessageType.UNKNOWN_PROTOCOL;
			return message;
		}
		
		try {
			bytesRead = socket.read(message.rawPacket, 1, 1);
		} catch (IOException e) {
			message.type = RaptorSessionMessage.MessageType.SOCKET_ERROR;
			return message;
		}
		
		if (bytesRead != 1) {
			message.type = RaptorSessionMessage.MessageType.SOCKET_ERROR;
			return message;
		} 
		
		char messageType = (char)(message.rawPacket[1]);
		
		switch (messageType) {
		case RaptorRemoteProtocol.PING_RSP_MSG:
			message.type = RaptorSessionMessage.MessageType.PING_RSP;
			
			System.out.println("Ping response message received");
			
			break;
			
		case RaptorRemoteProtocol.INIT_RSP_MSG:
			message.type = RaptorSessionMessage.MessageType.INIT_RSP;
			
			try {
				bytesRead = socket.read(message.rawPacket, 2, 1);
			} catch (IOException e) {
				message.type = RaptorSessionMessage.MessageType.SOCKET_ERROR;
				return message;
			}
			
			if (bytesRead != 1) {
				message.type = RaptorSessionMessage.MessageType.SOCKET_ERROR;
				return message;
			} 
			
			char success = (char)(message.rawPacket[2]);
			
			if (success == 1) {
				message.success = true;
			} else {
				message.success = false;
			}
			
			System.out.println("Init response message received");
			
			break;
			
		case RaptorRemoteProtocol.QUIT_RSP_MSG:
			
			message.type = RaptorSessionMessage.MessageType.QUIT_RSP;
			
			System.out.println("Quit response message received");
			
			break;
			
		case RaptorRemoteProtocol.VID_START_RSP:
			
			message.type = RaptorSessionMessage.MessageType.VID_START;
			
			try {
				bytesRead = socket.read(message.rawPacket, 2, 1);
			} catch (IOException e) {
				message.type = RaptorSessionMessage.MessageType.SOCKET_ERROR;
				return message;
			}
			
			if (bytesRead != 1) {
				message.type = RaptorSessionMessage.MessageType.SOCKET_ERROR;
				return message;
			} 
			
			success = (char) message.rawPacket[2];
			
			if (success == 1) {
				message.success = true;
			} else {
				message.success = false;
			}
			
			break;
			
		case RaptorRemoteProtocol.VID_END_RSP:
			
			message.type = RaptorSessionMessage.MessageType.VID_END;
			
			try {
				bytesRead = socket.read(message.rawPacket, 2, 1);
			} catch (IOException e) {
				message.type = RaptorSessionMessage.MessageType.SOCKET_ERROR;
				return message;
			}
			
			if (bytesRead != 1) {
				message.type = RaptorSessionMessage.MessageType.SOCKET_ERROR;
				return message;
			} 
			
			success = (char) message.rawPacket[2];
			
			if (success == 1) {
				message.success = true;
			} else {
				message.success = false;
			}
			
			break;
		default:
			message.type = RaptorSessionMessage.MessageType.INVALID_MESSAGE_TYPE;
			
			System.out.println("Invalid message type received");
		}
		
		return message;
		
	}
	
	
	
}
