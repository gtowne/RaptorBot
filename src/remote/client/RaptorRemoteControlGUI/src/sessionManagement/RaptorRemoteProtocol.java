package sessionManagement;
import java.io.ByteArrayOutputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.util.Arrays;

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
	public static final int VID_START = 0x07;
	public static final int VID_START_RSP = 0x08;
	
	private static final int MAX_PACKET_BYTES = 256;
	
	public static byte[] newPingPacket() {
		ByteArrayOutputStream _out = new ByteArrayOutputStream();
		DataOutputStream out = new DataOutputStream(_out);
		 
		try {
			out.writeInt(PING_MSG);
			
			
		} catch (IOException e) {
			e.printStackTrace();
		}
		
		return _out.toByteArray();
	}
	
	public static byte[] newInitSessionPacket() {
		ByteArrayOutputStream _out = new ByteArrayOutputStream();
		DataOutputStream out = new DataOutputStream(_out);
		 
		try {
			out.writeInt(INIT_SESSION_MSG);
			
			
		} catch (IOException e) {
			e.printStackTrace();
		}
		
		return _out.toByteArray();
	}
	
	public static byte[] newVideoInitPacket(String thisIP, int thisPort, int camNum, int targetFPS) {
		ByteArrayOutputStream _out = new ByteArrayOutputStream();
		DataOutputStream out = new DataOutputStream(_out);
		
		try {
			out.writeInt(VID_START);
			out.writeInt(thisPort);
			out.writeInt(camNum);
			out.writeInt(targetFPS);
			out.writeInt(thisIP.length());
			out.writeBytes(thisIP);
			
			for (int i = 0; i < 128 - thisIP.length(); i++) {
				out.writeByte(' ');
			}
			
		} catch (IOException e) {
			e.printStackTrace();
		}
		
		
		return _out.toByteArray();
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
		
		int success = 0;
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
			try {
				socket.readBytes(machineNameBuff, 0, machineNameLen);
			} catch (IOException e) {
				message.type = RaptorSessionMessage.MessageType.SOCKET_ERROR;
				return message;
			}
			
			String machineName =  new String(machineNameBuff);
			
			message.type = RaptorSessionMessage.MessageType.PING_RSP;
			message.machineName = machineName;
						
			break;
			
		case INIT_SESSION_RSP_MSG:
			
			try {
				success = socket.readInt();
			} catch (IOException e) {
				message.type = RaptorSessionMessage.MessageType.SOCKET_ERROR;
				return message;
			}
			
			message.type = RaptorSessionMessage.MessageType.INIT_RSP;
			if (success == 0) {
				message.success = false;
			} else if (success == 1) {
				message.success = true;
			} else {
				message.type = RaptorSessionMessage.MessageType.MALFORMED_MESSAGE;
			}
			
			break;
			
			
		case VID_START_RSP:

			try {
				success = socket.readInt();
			} catch (IOException e) {
				message.type = RaptorSessionMessage.MessageType.SOCKET_ERROR;
				return message;
			}
			
			message.type = RaptorSessionMessage.MessageType.VID_START_RSP;
			if (success == 0) {
				message.success = false;
			} else if (success == 1) {
				message.success = true;
			} else {
				message.type = RaptorSessionMessage.MessageType.MALFORMED_MESSAGE;
			}
			
			break;
			
		default:
			System.out.println("RaptorRemoteProtocol:: Unsupported message type received");
			
		}
		
		return message;
		
	}
	
	
	
}
