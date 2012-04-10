package videoStreaming;
import java.math.BigInteger;


public class MyUtils {
	public static int byteArrayToInt(byte[] in) {
		return (new BigInteger(in)).intValue();
	}
	
	public static void memcpyRange(byte[] dest, int destOffset, byte[] source, int sourceOffset, int length) {
		for (int i = 0; i < length; i++) {
			dest[destOffset + i] = source[sourceOffset + i];
		}
	}
}
