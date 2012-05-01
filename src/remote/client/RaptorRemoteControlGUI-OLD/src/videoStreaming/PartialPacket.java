package videoStreaming;

public class PartialPacket {
	private int seqNo;
	private int expectedBytes;
	private int bytesReceived;
	private byte[] buffer;
	
	public PartialPacket(int thisSeqNo, int numBytesExpected) {
		expectedBytes = numBytesExpected;
		buffer = new byte[expectedBytes];
		bytesReceived = 0;
		seqNo = thisSeqNo;
		
		//System.out.printf("PARTIAL::Init new packet seqNo=%d numByted=%d\n", seqNo, numBytesExpected);
	}
	
	public void addDataAtOffset(byte[] inBuff, int inBuffOffset, int inLen, int packetBuffOffset) {
		MyUtils.memcpyRange(buffer, packetBuffOffset, inBuff, inBuffOffset, inLen);
		bytesReceived += inLen;
		
		//System.out.printf("PARTIAL::Adding data to seqNo=%d at=%d of=%d newTotal=%d\n", seqNo, packetBuffOffset, inLen, bytesReceived);
	}
	
	public boolean isComplete() {
		return bytesReceived == expectedBytes;
	}
	
	public byte[] getData() {
		return buffer;
	}
	
	public int getSeqNo() {
		return this.seqNo;
	}
	
}
