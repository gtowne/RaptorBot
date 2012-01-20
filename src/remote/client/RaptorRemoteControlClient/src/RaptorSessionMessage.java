
public class RaptorSessionMessage {
	public static enum MessageType {PING, PING_RSP, INIT, INIT_RSP, QUIT, QUIT_RSP, CMD, VID, UNKNOWN_PROTOCOL, INVALID_MESSAGE_TYPE};
	
	private char[] rawPacket;
	
	public MessageType type;
	
	public static RaptorSessionMessage parsePacket(char[] data) {
		RaptorSessionMessage message = new RaptorSessionMessage();
		
		char protocolID = (char) (data[0] >> 8);
		
		if (protocolID != RaptorRemoteProtocol.RAPTOR_REMOTE_SESSION_PROTOCOL_VERSION_ID) {
			message.type = MessageType.UNKNOWN_PROTOCOL;
			return message;
		}
		
		char messageType = (char)(data[0] & (char)0xFF);
		
		switch (messageType) {
		case RaptorRemoteProtocol.PING_RSP_MSG:
			message.type = MessageType.PING_RSP;
			
			System.out.println("Ping response message received");
			
			break;
		default:
			message.type = MessageType.INVALID_MESSAGE_TYPE;
			
			System.out.println("Invalid message type received");
		}
		
		return message;
	}
	
	public RaptorSessionMessage() {
		
	}
	
	public char[] getPayload() {
		return null;
	}
}
