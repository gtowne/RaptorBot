import java.awt.Color;
import java.awt.Point;
import java.awt.image.BufferedImage;
import java.awt.image.DataBufferByte;
import java.awt.image.Raster;
import java.io.ByteArrayInputStream;
import java.io.File;
import java.io.IOException;
import java.net.*;
import java.util.*;

import javax.imageio.*;

import videoStreaming.Frame;
import videoStreaming.VideoStreamer;


public class Main {

	public static final int RECEIVE_BUFFER_SIZE = 1024 * 768 * 4;

	public static void main(String[] args) {
		VideoStreamer streamer = new VideoStreamer();
		streamer.start();
		
		while(true) {
			Frame newFrame = streamer.getFrame();
			try {
				Thread.sleep(30);
			} catch (InterruptedException e) {

				e.printStackTrace();
			}
			
			if (newFrame != null) {
				try {
					ImageIO.write(newFrame.frame, "jpg", new File("images/image-"+newFrame.seqNo));
				} catch (IOException e) {
					e.printStackTrace();
				}
			}
		}
	}
	
	
}
