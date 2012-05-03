package sessionManagement;

import java.io.IOException;
import java.net.ServerSocket;
import java.util.concurrent.Semaphore;

import behaviorControl.BehaviorParam;
import behaviorControl.BehaviorParam.ArcDirection;
import behaviorControl.BehaviorParam.PivotDirection;

import userInterface.RaptorRemoteUserInterface;
import videoStreaming.VideoStreamer;


public class RaptorRemoteSession {
	protected SimpleSocket controlSocket;
	protected SimpleSocket feedbackSocket;
	
	protected Semaphore controlSocketMutex;
	
	protected FeedbackListingThread feedbackListeningThread;
		
	protected boolean sessionIsActive;
	protected RaptorRemoteUserInterface thisUI;
	
	public RaptorRemoteSession(RaptorRemoteUserInterface thisUI, SimpleSocket controlSocket, SimpleSocket feedbackSocket) {
		System.out.println("RaptorRemoteSession:: Initializing new session");
		
		this.controlSocket = controlSocket;		
		
		controlSocketMutex = new Semaphore(1);
		
		this.thisUI = thisUI;
		sessionIsActive = true;
		feedbackListeningThread = new FeedbackListingThread(feedbackSocket);
		feedbackListeningThread.start();
		
	}
	
	public void sendMoveCommand(BehaviorParam.MoveDirection direction, double speed, double distance, boolean setNext) {
		try {
			controlSocketMutex.acquire();
		} catch (InterruptedException e) {
			System.out.println("RaptorRemoteSession:: Error, could not acquire contorl socket mutex");
			return;
		}
		
		int maneuver = RaptorRemoteProtocol.MAN_MOVE;
		int _direction = 0;
		
		if (direction == BehaviorParam.MoveDirection.FORWARD) {
			_direction = RaptorRemoteProtocol.DIR_FORWARD;
		} else if (direction == BehaviorParam.MoveDirection.BACKWARD) {
			_direction = RaptorRemoteProtocol.DIR_BACKWARD;
		}
		
		int updateMethod = RaptorRemoteProtocol.MAN_QUEUE_ENQUEUE;
		if (setNext) {
			updateMethod = RaptorRemoteProtocol.MAN_QUEUE_SET_NEXT;
		}
		
		try {
			controlSocket.writeBytes(RaptorRemoteProtocol.newManeuverPacket(maneuver, _direction, 0, speed, distance, 0, updateMethod));
		} catch (IOException e) {
			System.out.println("RaptorRemoteSession:: Error, could not send maneuver packet");
			return;
		}
		
		
		controlSocketMutex.release();
	}
	
	public void sendArcCommand(BehaviorParam.ArcDirection direction, double speed, double degrees, double radius, boolean setNext) {
		try {
			controlSocketMutex.acquire();
		} catch (InterruptedException e) {
			System.out.println("RaptorRemoteSession:: Error, could not acquire contorl socket mutex");
			return;
		}
		
		int maneuver = RaptorRemoteProtocol.MAN_ARC;
		int _direction = 0;
		
		switch (direction) {
		case FORWARD_LEFT:
			_direction = RaptorRemoteProtocol.DIR_FORWARD_LEFT;
			break;
			
		case FORWARD_RIGHT:
			_direction = RaptorRemoteProtocol.DIR_FORWARD_RIGHT;
			break;
			
		case BACKWARD_LEFT:
			_direction = RaptorRemoteProtocol.DIR_BACKWARD_LEFT;
			break;
			
		case BACKWARD_RIGHT:
			_direction = RaptorRemoteProtocol.DIR_BACKWARD_LEFT;
			break;
		}
		
		
		int updateMethod = RaptorRemoteProtocol.MAN_QUEUE_ENQUEUE;
		if (setNext) {
			updateMethod = RaptorRemoteProtocol.MAN_QUEUE_SET_NEXT;
		}
		
		try {
			controlSocket.writeBytes(RaptorRemoteProtocol.newManeuverPacket(maneuver, _direction, degrees, speed, 0, radius, updateMethod));
		} catch (IOException e) {
			System.out.println("RaptorRemoteSession:: Error, could not send maneuver packet");
			return;
		}
		
		
		controlSocketMutex.release();
	}
	
	public void sendPivotCommand(BehaviorParam.PivotDirection pivotDir, double degrees, boolean setNext) {
		try {
			controlSocketMutex.acquire();
		} catch (InterruptedException e) {
			System.out.println("RaptorRemoteSession:: Error, could not acquire control socket mutex");
			return;
		}
		
		int maneuver = RaptorRemoteProtocol.MAN_PIVOT;
		int _direction = 0;
		
		switch (pivotDir) {
		case LEFT:
			_direction = RaptorRemoteProtocol.DIR_LEFT;
			break;
			
		case RIGHT:
			_direction = RaptorRemoteProtocol.DIR_RIGHT;
			break;
		}
		
		int updateMethod = RaptorRemoteProtocol.MAN_QUEUE_ENQUEUE;
		if (setNext) {
			updateMethod = RaptorRemoteProtocol.MAN_QUEUE_SET_NEXT;
		}
		
		
		try {
			controlSocket.writeBytes(RaptorRemoteProtocol.newManeuverPacket(maneuver, _direction, degrees, 0, 0, 0, updateMethod));
		} catch (IOException e) {
			System.out.println("RaptorRemoteSession:: Error, could not send maneuver packet");
			return;
		}
		
		
		controlSocketMutex.release();
	}
	
	
	private class FeedbackListingThread extends Thread {
		private SimpleSocket socket;
		
		public FeedbackListingThread(SimpleSocket socket) {
			this.socket = socket;
		}
		
		public void run() {
			System.out.println("RaptorRemoteSession:: Feedback Listening Thread for session started");
			
			while (RaptorRemoteSession.this.sessionIsActive) {
				if (socket != null && socket.isConnected()) {
					RaptorSessionMessage newMessage = RaptorRemoteProtocol.readFromSocket(socket);
				
					if (newMessage.type == RaptorSessionMessage.MessageType.FEEDBACK_MSG) {
						RaptorRemoteSession.this.thisUI.behaviorControlPanel.updateTimeToCompletion(newMessage.msToNext);
												
						String curStr = maneuverParamsToString(newMessage.currentManeuver, newMessage.currentDirection, newMessage.currentDegrees, newMessage.currentSpeed, newMessage.currentRadius, newMessage.currentDistance);
						String nextStr = maneuverParamsToString(newMessage.nextManeuver, newMessage.nextDirection, newMessage.nextDegrees, newMessage.nextSpeed, newMessage.nextRadius, newMessage.nextDistance);
						
						RaptorRemoteSession.this.thisUI.behaviorControlPanel.updateCurrentManeuverString(curStr);
						RaptorRemoteSession.this.thisUI.behaviorControlPanel.updateNextManeuverString(nextStr);
					}
				}
			}
			
			System.out.println("RaptorRemoteSession:: Feedback Listening Thread for session exiting");
			
			try {
				feedbackSocket.flush();
				feedbackSocket.close();
			} catch (Exception e) {
			}
		}
	}
	
	public boolean isActive() {
		return controlSocket.isConnected() && sessionIsActive;
	}
	
	public boolean close() {
		boolean sendSuccess = this.sendQuitMessage();
				
		sessionIsActive = false;
		
		try {
			controlSocket.flush();
			controlSocket.close();
		} catch (IOException e) {
			e.printStackTrace();
		}
		
		return true;
	}
	
	public boolean initVideoStream(int camNum, int targetFPS) {		
		sendVideoInitMessage(RaptorRemoteClient.getMyIP(), VideoStreamer.LOCAL_PORT_NO, camNum, targetFPS);
		
		RaptorSessionMessage reply = RaptorRemoteProtocol.readFromSocket(controlSocket);
		
		if (reply == null) {
			return false;
		}
		
		if (reply.type != RaptorSessionMessage.MessageType.VID_START_RSP) {
			return false;
		}
		
		return reply.success;
	}
	
	public boolean stopVideoStream() {
		return sendVideoStopMessage();
	}
	
	private boolean sendVideoInitMessage(String thisIP, int thisPort, int camNum, int targetFPS) {
		
		try {
			controlSocket.writeBytes(RaptorRemoteProtocol.newVideoInitPacket(thisIP, thisPort, camNum, targetFPS));
		} catch (IOException e) {
			return false;
		}
		
		return true;
		
	}
	
	private boolean sendVideoStopMessage() {
		try {
			controlSocket.writeBytes(RaptorRemoteProtocol.newVideoStopPacket());
		} catch (IOException e) {
			return false;
		}
		
		return true;
	}
	
	private boolean sendQuitMessage() {
		try {
			controlSocket.writeBytes(RaptorRemoteProtocol.newQuitPacket());
		} catch (IOException e) {
			return false;
		}
		
		return true;
	}
	
	public int sendScript(String scriptString) {
		try {
			controlSocket.writeBytes(RaptorRemoteProtocol.newScriptPacket(scriptString));
			
			RaptorSessionMessage reply = RaptorRemoteProtocol.readFromSocket(controlSocket);
			
			if (reply.type == RaptorSessionMessage.MessageType.SCRIPT_RSP_MSG) {
				return reply.errorLine;
			}
		} catch (IOException e) {
			return 1;
		}
		
		return 1;
	}

	private String maneuverParamsToString(int maneuverType, int direction, double degrees, double speed, double radius, double distance) {
		String manStr = "";
		
		switch(maneuverType) {
		case RaptorRemoteProtocol.MAN_NONE:
			manStr += "None ";
			break;
		case RaptorRemoteProtocol.MAN_MOVE:
			manStr += "Move ";
			
			if (direction == RaptorRemoteProtocol.DIR_FORWARD) {
				manStr += "forward ";
			} else if (direction == RaptorRemoteProtocol.DIR_BACKWARD) {
				manStr += "backward ";
			}
			
			if (speed > 0) {
				manStr += "at " + speed + " meters/s ";
			}
			
			if (distance > 0) {
				manStr += " for " + distance + " meters";
			}
			
			break;
		case RaptorRemoteProtocol.MAN_ARC:
			manStr += "Arc turn ";
			
			switch(direction) {
			case RaptorRemoteProtocol.DIR_FORWARD_LEFT:
				manStr += "forward-left ";
				break;
			case RaptorRemoteProtocol.DIR_FORWARD_RIGHT:
				manStr += "forward-right ";
				break;
			case RaptorRemoteProtocol.DIR_BACKWARD_LEFT:
				manStr += "backward-left ";
				break;
			case RaptorRemoteProtocol.DIR_BACKWARD_RIGHT:
				manStr += "backward-right ";
				break;
			}
			
			if (degrees > 0) {
				manStr += degrees + " degrees ";
			}
			
			if (radius > 0) {
				manStr += "with radius " + radius + " meters";
			}
			
			break;
		case RaptorRemoteProtocol.MAN_PIVOT:
			manStr += "Pivot turn ";
			
			if (direction == RaptorRemoteProtocol.DIR_LEFT) {
				manStr += "left ";
			} else if (direction == RaptorRemoteProtocol.DIR_RIGHT) {
				manStr += "right ";
			}
			
			if (degrees > 0) {
				manStr += degrees + " degrees ";
			}
			
			break;
		}
		
		return manStr;
	}
	
}
