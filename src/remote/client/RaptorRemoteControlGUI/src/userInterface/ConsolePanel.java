package userInterface;
import java.awt.Color;
import java.awt.TextArea;

import javax.swing.JScrollPane;


public class ConsolePanel extends UIPanel {
	JScrollPane scrollPane;
	TextArea textArea;
	
	public static final int CONSOLE_CHAR_WIDTH = 100;
	public static final int CONSOLE_CHAR_HEIGHT = 40;
	
	public ConsolePanel() {
		super();
		
		textArea = new TextArea();
		textArea.setBackground(Color.black);
		textArea.setForeground(Color.white);
		scrollPane = new JScrollPane(textArea);
		scrollPane.setPreferredSize(this.getSize());
		this.add(scrollPane);
		
		textArea.append("Hello, world.");
		
	}
}
