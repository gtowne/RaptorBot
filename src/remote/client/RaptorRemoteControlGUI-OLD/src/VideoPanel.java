import java.awt.BorderLayout;
import java.awt.Button;
import java.awt.Image;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;

import javax.imageio.ImageIO;
import javax.media.*;
import javax.swing.ImageIcon;
import javax.swing.JLabel;

public class VideoPanel extends UIPanel implements VideoPlayer {
	
	JLabel imageLabel;
	BufferedImage curImage;
	
	public VideoPanel() {
		super();
		
		this.setLayout(new BorderLayout());
				
		imageLabel = new JLabel();
		this.add(imageLabel);
		
		System.out.println("VideoPanel initialized");
	}

	public void updateFrame(BufferedImage newFrame) {
		setCurFrame(newFrame);
	}
	
	private void setCurFrame(BufferedImage newFrame) {
		curImage = newFrame;
		ImageIcon newIcon = new ImageIcon(curImage);
		
		int curPanelWidth = this.getWidth();
		int curPanelHeight = this.getHeight();
		
		imageLabel.setIcon(new ImageIcon(curImage));
	}
}
