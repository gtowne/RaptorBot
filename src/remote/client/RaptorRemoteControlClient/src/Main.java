
import java.io.*;
import java.net.*;
import java.util.*;

public class Main {

	
    public static void main(String args[]) throws Exception {
    	System.out.println("here");
        RaptorRemoteClient client = new RaptorRemoteClient();
		boolean success = client.ping(InetAddress.getLocalHost());
		
		if (success) {
			System.out.println("Successfully contacted host");
		} else {
			System.out.println("Cound't reach host");
		}
		
		success = client.connect(InetAddress.getLocalHost());
		
		if (success) {
			System.out.println("Successfully connected to host");
		} else {
			System.out.println("Failed connecting to host");
		}
		
		
		success = client.initVideoStream();
		
		if (success) {
			System.out.println("Successfully initialized video stream");
		} else {
			System.out.println("Error initializing video stream");
		}
		
		Thread.sleep(3000);
		
		
		success = client.stopVideoStream();
		
		if (success) {
			System.out.println("Successfully ended video stream");
		} else {
			System.out.println("Error stopping video stream");
		}
		
		
		success = client.disconnect();
		
		if (success) {
			System.out.println("Successfully disconnected from host");
		} else {
			System.out.println("Failed disconnecting from host");
		}

    }

}
