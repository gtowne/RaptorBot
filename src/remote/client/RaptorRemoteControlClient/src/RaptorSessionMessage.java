
public class RaptorSessionMessage {
	public static enum MessageType {PING, PING_RSP, INIT, INIT_RSP, QUIT, 
		QUIT_RSP, CMD, VID, UNKNOWN_PROTOCOL, INVALID_MESSAGE_TYPE, SOCKET_ERROR,
		VID_START, VID_START_RSP, VID_END, VID_END_RSP};
	
	public char[] rawPacket;
	
	public MessageType type;
	public boolean success;
	
	public RaptorSessionMessage(int bufferBytes) {
		rawPacket = new char[bufferBytes];
	}
	

	
}
