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
	public static final int VID_START = 0x07;
	public static final int VID_START_RSP = 0x08;
	public static final int VID_STOP = 0x09;
	public static final int FEEDBACK_MSG = 0x0A;
	public static final int MANEUVER_MSG = 0x0B;
	
	/*
	 * Maneuver Types
	 */
	public static final int MAN_NONE = 0x01;
	public static final int MAN_MOVE = 0x02;
	public static final int MAN_PIVOT = 0x03;
	public static final int MAN_ARC = 0x04;
	
	/*
	 * Maneuver Parameters
	 */
	/*
	 *  --- Direction
	 */
	public static final int DIR_FORWARD = 0x01;
	public static final int DIR_BACKWARD = 0x02;
	public static final int DIR_LEFT = 0x03;
	public static final int DIR_RIGHT = 0x04;
	public static final int DIR_FORWARD_LEFT = 0x05;
	public static final int DIR_FORWARD_RIGHT = 0x06;
	public static final int DIR_BACKWARD_LEFT = 0x07;
	public static final int DIR_BACKWARD_RIGHT = 0X08;
	
	private static final int MAX_PACKET_BYTES = 256;
	
	public static final double FLOAT_SCALE_FACOR = 10000.0;
	
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
	
	
	
	public static byte[] newInitSessionPacket(String myIP, int feedbackListeningPort) {
		ByteArrayOutputStream _out = new ByteArrayOutputStream();
		DataOutputStream out = new DataOutputStream(_out);
		 
		try {
			out.writeInt(INIT_SESSION_MSG);
			out.writeInt(feedbackListeningPort);
			out.writeInt(myIP.length());
			out.writeBytes(myIP);
			
			for (int i = 0; i < 128 - myIP.length(); i++) {
				out.writeByte(' ');
			}
			
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
		
		byte[] returnBuff = _out.toByteArray();
				
		return _out.toByteArray();
	}
	
	public static byte[] newVideoStopPacket() {
		ByteArrayOutputStream _out = new ByteArrayOutputStream();
		DataOutputStream out = new DataOutputStream(_out);
		 
		try {
			out.writeInt(VID_STOP);
			
			
		} catch (IOException e) {
			e.printStackTrace();
		}
		
		return _out.toByteArray();
	}
	
	public static byte[] newQuitPacket() {
		ByteArrayOutputStream _out = new ByteArrayOutputStream();
		DataOutputStream out = new DataOutputStream(_out);
		 
		try {
			out.writeInt(QUIT_MSG);
			
			
		} catch (IOException e) {
			e.printStackTrace();
		}
		
		return _out.toByteArray();
	}
	
	public static byte[] newManeuverPacket(int maneuver, int direction, double degrees, double speed, double distance, double radius) {
		ByteArrayOutputStream _out = new ByteArrayOutputStream();
		DataOutputStream out = new DataOutputStream(_out);
		 
		try {
			out.writeInt(MANEUVER_MSG);
			out.writeInt(maneuver);
			out.writeInt(direction);
			out.writeInt((int)(degrees * FLOAT_SCALE_FACOR));
			out.writeInt((int)(speed * FLOAT_SCALE_FACOR));
			out.writeInt((int)(distance * FLOAT_SCALE_FACOR));
			out.writeInt((int)(radius * FLOAT_SCALE_FACOR));
			
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
			
		case FEEDBACK_MSG:
			
			System.out.println("RaptorRemoteProtocol:: Received new Fedback message");
			
			message.type = RaptorSessionMessage.MessageType.FEEDBACK_MSG;
			
			try {
				message.currentManeuver = socket.readInt();
				message.currentDirection = socket.readInt();
				message.currentDegrees = socket.readInt() / FLOAT_SCALE_FACOR;
				message.currentSpeed = socket.readInt() / FLOAT_SCALE_FACOR;
				message.currentDistance = socket.readInt() / FLOAT_SCALE_FACOR;
				message.currentRadius = socket.readInt() / FLOAT_SCALE_FACOR;
				message.msToNext = socket.readInt();
				message.nextManeuver = socket.readInt();
				message.nextDirection = socket.readInt();
				message.nextDegrees = socket.readInt() / FLOAT_SCALE_FACOR;
				message.nextSpeed = socket.readInt() / FLOAT_SCALE_FACOR;
				message.nextDistance = socket.readInt() / FLOAT_SCALE_FACOR;
				message.nextRadius = socket.readInt() / FLOAT_SCALE_FACOR;
			} catch (IOException e) {
				message.type = RaptorSessionMessage.MessageType.MALFORMED_MESSAGE;
			}
						
			break;
			
		default:
			System.out.println("RaptorRemoteProtocol:: Unsupported message type received");
			
		}
		
		return message;
		
	}
	
	
	
}
