
import java.io.*;
import java.net.*;
import java.util.*;

public class Main {

	
    public static void main(String args[]) throws Exception {
    	System.out.println("here");
        RaptorRemoteClient client = new RaptorRemoteClient();
		client.ping(InetAddress.getLocalHost());
    }

}
