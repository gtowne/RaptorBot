import java.awt.image.BufferedImage;


public class DummyVideoPlayer implements VideoPlayer{

	@Override
	public void updateFrame(BufferedImage frame) {
		System.out.println("*** FRAME UPDATE RECEIVED ****");
		
	}

}
