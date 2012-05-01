package userInterface;
import javax.swing.JPanel;
import javax.swing.border.EtchedBorder;
import javax.swing.JLabel;
import java.awt.Dimension;


public class UIPanel extends JPanel {

	protected RaptorRemoteUserInterface parentUI;
	/**
	 * Create the panel.
	 */
	public UIPanel(RaptorRemoteUserInterface parentUI) {
		this.parentUI = parentUI;
		setPreferredSize(new Dimension(10000, 10000));
		setBorder(new EtchedBorder(EtchedBorder.LOWERED, null, null));
		
	}

}
