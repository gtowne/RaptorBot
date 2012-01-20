
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
	public static final char VID_MSG = 'v';
	
	public static char[] newPingPacket() {
		char[] packet = {RAPTOR_REMOTE_SESSION_PROTOCOL_VERSION_ID, PING_MSG};
						
		return packet;
	}
	
	
}
