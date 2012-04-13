import java.io.ByteArrayOutputStream;
import java.io.DataOutputStream;
import java.io.IOException;

public class RaptorRemoteProtocol {
	
	/*
	 * Protocol Version
	 */
	public static final char RAPTOR_REMOTE_SESSION_PROTOCOL_VERSION_ID = 0x7d;

	/*
	 * Message Types
	 */
	public static final int PING_MSG = 0x01;
	public static final int PING_RSP_MSG = 0x02;
	public static final int INIT_SESSION_MSG = 0x03;
	public static final int INIT_SESSION_RSP_MSG = 0X04;
	public static final int QUIT_MSG = 0x05;
	public static final int QUIT_RSP_MSG = 0x06;
	
	private static final int MAX_PACKET_BYTES = 256;
	
	public static byte[] newPingPacket() {
		ByteArrayOutputStream out = new ByteArrayOutputStream();
		 
		out.write(PING_MSG);
		
		return out.toByteArray();
	}
	
	public static RaptorSessionMessage readFromSocket(SimpleSocket socket) {
		RaptorSessionMessage message = new RaptorSessionMessage();
			
		int messageType = -1;
		try {
			messageType = socket.readInt();
		} catch (IOException e) {
			message.type = RaptorSessionMessage.MessageType.SOCKET_ERROR;
			return message;
		}
		
		switch (messageType) {
			
		case PING_RSP_MSG:
			int machineNameLen = 0;
			try {
				machineNameLen = socket.readInt();
			} catch (IOException e) {
				message.type = RaptorSessionMessage.MessageType.SOCKET_ERROR;
				return message;
			}
			
			byte[] machineNameBuff = new byte[machineNameLen];
			socket.readBytes(machineNameBuff, 0, machineNameLen);
			String machineName = String.valueOf(machineNameBuff);
			
			break;
		default:
			System.out.println("RaptorRemoteProtocol:: Unsupported message type received");
			
		}
		
		return message;
		
	}
	
	
	
}
