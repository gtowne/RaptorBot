package videoStreaming;
import java.awt.image.BufferedImage;

public class Frame {
	public int seqNo;
	public BufferedImage frame;
	
	public Frame(int _seqNo, BufferedImage _frame) {
		frame = _frame;
		seqNo = _seqNo;
	}
}
