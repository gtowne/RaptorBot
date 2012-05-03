package sessionManagement;

public class RaptorSessionMessage {
	public static enum MessageType {PING, PING_RSP, INIT, INIT_RSP, QUIT, 
		QUIT_RSP, CMD, VID, UNKNOWN_PROTOCOL, MALFORMED_MESSAGE, SOCKET_ERROR,
		VID_START, VID_START_RSP, VID_END, VID_END_RSP, FEEDBACK_MSG, SCRIPT_RSP_MSG};
		
	public MessageType type;
	public boolean success;
	public String machineName;
	
	public int errorLine;
	
	public int currentManeuver;
	public int currentDirection;
	public double currentDegrees;
	public double currentSpeed;
	public double currentRadius;
	public double currentDistance;
	public int msToNext;
	
	public int nextManeuver;
	public int nextDirection;
	public double nextDegrees;
	public double nextSpeed;
	public double nextRadius;
	public double nextDistance;
	
	public RaptorSessionMessage() {
	}
	

	
}
