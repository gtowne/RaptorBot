package videoStreaming;
import java.awt.Image;
import java.awt.image.BufferedImage;
import java.awt.image.RenderedImage;
import java.io.ByteArrayInputStream;
import java.io.File;
import java.io.IOException;
import java.math.BigInteger;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.SocketException;
import java.util.Arrays;
import java.util.Hashtable;
import java.util.concurrent.Semaphore;
import javax.media.jai.PlanarImage;
import com.sun.media.jai.codec.ByteArraySeekableStream;
import com.sun.media.jai.codec.ImageCodec;
import com.sun.media.jai.codec.ImageDecoder;
import com.sun.media.jai.codec.SeekableStream;


import javax.imageio.ImageIO;


public class VideoStreamer extends Thread {
	
	public static final int LOCAL_PORT_NO = 7777;
	public static final int RECEIVE_BUFFER_SIZE = 1024 * 768 * 4;
	public static final int TARGET_NUM_BUFFERED_FRAMES = 3;
	
	private DatagramSocket socket;
	private DatagramPacket receivePacket;
	private Hashtable<Integer, PartialPacket> partialPackets;
	
	private int oldestPartialPacketSeqNo; 
	private int latestCompletePacket;
	
	private Hashtable<Integer, Frame> bufferedFrames;
	private int oldestBufferedFrame;
	
	private Semaphore bufferMutex;
	
	private boolean keepStreaming;
	
	public VideoStreamer() {		
		try {
			socket = new DatagramSocket(LOCAL_PORT_NO);
		} catch (SocketException e) {
			// System.out.println("Error, could not bind socket to local port");
			e.printStackTrace();
		}
		
		partialPackets = new Hashtable<Integer, PartialPacket>();
		byte[] receiveBuff = new byte[RECEIVE_BUFFER_SIZE];
		receivePacket = new DatagramPacket(receiveBuff, RECEIVE_BUFFER_SIZE);
		latestCompletePacket = -1;
		oldestPartialPacketSeqNo = -1;
		
		bufferMutex = new Semaphore(1);
		bufferedFrames = new Hashtable<Integer, Frame>();
		
		System.out.println("VideoStreamer initialized");
		
	}
	
	public void run() {
		System.out.println("VideoStreamer:: Starting to listen for incoming frames in background");
		
		keepStreaming = true;
		while (keepStreaming) {
			Frame newFrame = _getFrame();
			
			try {
				bufferMutex.acquire();
			} catch (InterruptedException e) {
				// System.out.println("Failed to acquire buffer mutex");
				e.printStackTrace();
			}
			
				bufferedFrames.put(newFrame.seqNo, newFrame);
				
				// if the oldest buffered frame has been played out, update
				// the oldest buffered frame
				if (bufferedFrames.size() == 1) {
					oldestBufferedFrame = newFrame.seqNo;
				}
			
			bufferMutex.release();
		}
	}
	
	public Frame getFrame() {
		try {
			bufferMutex.acquire();
		} catch (InterruptedException e) {
			// System.out.println("Failed to acquire buffer mutex");
			e.printStackTrace();
		}
			
			boolean doCleanUp = false;
			Frame returnFrame = null;
			
			if (bufferedFrames.isEmpty()) {
				// System.out.println("Trying to play out frame but none buffered");
				
			} else if (bufferedFrames.containsKey(oldestBufferedFrame)) {
				
				//System.out.printf("VideoStreamer:: Playing out frame %d, \n", oldestBufferedFrame);
				
				returnFrame = bufferedFrames.get(oldestBufferedFrame);
				
				// if this is the last frame in the buffer, save it and we'll return
				// it on the next call if we don't receive another packet before then
				if (bufferedFrames.size() == 1) {
					//System.out.println("    VideoStreamer:: No more frames to play out;");
					
				// else, do some buffer management
				} else {
					doCleanUp = true;
				}
				
			} else { // oldestBufferedFrame is not in buffer, something is wrong 
				 //System.out.println("VideoStreamer:: Error, oldestBufferedFrame is not in Buffer");
				
				doCleanUp = true;
			}
			
			if (doCleanUp && !bufferedFrames.isEmpty()) {
				Object[] keys = bufferedFrames.keySet().toArray();
				
				Arrays.sort(keys);
				
				// if the buffer is starting to fill up, drop some old packets,
				// and "skip ahead" in the buffer so we don't fall too far behind
				// real time
				if (keys.length > TARGET_NUM_BUFFERED_FRAMES) {
					oldestBufferedFrame = (Integer)keys[keys.length - TARGET_NUM_BUFFERED_FRAMES];
				}
				// else if there's only a few buffered packets, just update our oldest buffered
				// frame accordingly
				else {
					oldestBufferedFrame = (Integer)keys[1];
				}
				
				// dump any frames from the buffer older than the one's we've already played out
				for (Object _curKey : keys) {
					int curKey = (Integer) _curKey;
					
					if (curKey < oldestBufferedFrame) {
						bufferedFrames.remove(curKey);
					}
				}
				
				 //System.out.printf("   VideoStreamer:: Next frame to play out is %d\n", oldestBufferedFrame);
				 //System.out.printf("   VideoStreamer:: Total in queue %d\n", bufferedFrames.size());
			}
			
		bufferMutex.release();
		
		return returnFrame;
				
	}

	private Frame _getFrame() {
				
		byte[] frameData;
		int dataLength;
		int imWidth;
		int imHeight;
		byte[] newData;
		
		newData = this.receive();
		
		int messageType = MyUtils.byteArrayToInt(Arrays.copyOfRange(newData, 0, 4));
		dataLength = MyUtils.byteArrayToInt(Arrays.copyOfRange(newData, 4, 8));
		int frameNum = MyUtils.byteArrayToInt(Arrays.copyOfRange(newData, 8, 12));
		imWidth = MyUtils.byteArrayToInt(Arrays.copyOfRange(newData, 12, 16));
		imHeight = MyUtils.byteArrayToInt(Arrays.copyOfRange(newData, 16, 20));
		
		// System.out.printf("Received frame %d of size %d and dims %d x %d\n", frameNum, dataLength, imWidth, imHeight);
	
		ByteArrayInputStream inputStream = new ByteArrayInputStream(newData, 20, dataLength);
		
		BufferedImage frameImage = null;
		try {
			frameImage = ImageIO.read(inputStream);
		} catch (IOException e1) {
			// System.out.println("Error converting bye array to image");
			e1.printStackTrace();
		}
		
		return new Frame(frameNum, frameImage);
	}
	
	private byte[] receive() {
		byte[] returnBuff = null;
		
		int numBytesReceived = 0;
		
		int lastFragNo = -1;
		
		while (true) {

			try {
				socket.receive(receivePacket);
			} catch (IOException e) {
				// System.out.println("Error calling receive on socket");
				e.printStackTrace();
			}
			
			byte[] newData = receivePacket.getData();
			
			int headerLength = 4 * 7; 
			int messageType = MyUtils.byteArrayToInt(Arrays.copyOfRange(newData, 0, 4));
			int thisSeqNo = MyUtils.byteArrayToInt(Arrays.copyOfRange(newData, 4, 8));
			int numBytesExpected = MyUtils.byteArrayToInt(Arrays.copyOfRange(newData, 8, 12));
			int numFrags = MyUtils.byteArrayToInt(Arrays.copyOfRange(newData, 12, 16));
			int offset = MyUtils.byteArrayToInt(Arrays.copyOfRange(newData, 16, 20));
			int fragBytes = MyUtils.byteArrayToInt(Arrays.copyOfRange(newData, 20, 24));
			int thisFragNo = MyUtils.byteArrayToInt(Arrays.copyOfRange(newData, 24, 28));

			
			// If this fragment is for an older packet ignore it
			
			if (thisSeqNo <= latestCompletePacket) {
				// System.out.println("STREAMER::Throwing away a fragment from an old packet");
			}
			
			// If this is newer than the last packet returned, add its data to buffer
			else if (thisSeqNo > latestCompletePacket) {
				
				// if this is the first data for this packet, 
				if (!partialPackets.containsKey(thisSeqNo)) {
					PartialPacket newPacket = new PartialPacket(thisSeqNo, numBytesExpected);
					partialPackets.put(thisSeqNo, newPacket);					
				}
				
				// add new data to this partial packet
				PartialPacket thisPartialPacket = partialPackets.get(thisSeqNo);
				thisPartialPacket.addDataAtOffset(newData, headerLength, fragBytes, offset);
				
				// if this new data completes the packet, return it
				if (thisPartialPacket.isComplete()) {
					// System.out.printf("STREAMER::Returning complete packet number %d\n", thisSeqNo);
					latestCompletePacket = thisSeqNo;
					partialPackets.remove(thisSeqNo);
					
					
					// clean up fragments of all older packets, they will never be returned
					for (int i = oldestPartialPacketSeqNo; i <= thisPartialPacket.getSeqNo(); i++) {
						partialPackets.remove(i);
					}
					
					oldestPartialPacketSeqNo = thisPartialPacket.getSeqNo();
					return thisPartialPacket.getData();
				}
				
			}
			
		}
		
		//return returnBuff;
	}
	
	public static int byteToUnsignedIntValue(byte signedVal) {
		int result = (int) signedVal;
		if (result < 0) result += 256;
		return result;
	}
	
	public static int rgbToIntARGB(int r, int g, int b) {
		int result = 0;
		//result = result | 0xFF000000;
		result = result | (r << 16);
		result = result | (g << 8);
		result = result | b;
		
		return result;
	}
	
	static BufferedImage loadImage(byte[] data, int offset, int len) throws Exception{
	    BufferedImage image = null;
	    SeekableStream stream = new ByteArraySeekableStream(data, offset, len);
	    String[] names = ImageCodec.getDecoderNames(stream);
	    ImageDecoder dec = 
	      ImageCodec.createImageDecoder(names[0], stream, null);
	    RenderedImage im = dec.decodeAsRenderedImage();
	    image = PlanarImage.wrapRenderedImage(im).getAsBufferedImage();
	    return image;
	  }
	
	

}
