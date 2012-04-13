package userInterface;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;

import javax.swing.JComboBox;
import javax.swing.JLabel;
import javax.swing.JSlider;
import javax.swing.SwingConstants;

public class BehaviorControlPanel extends UIPanel {
	
	private JLabel speedLabel;
	private JSlider speedSlider;
	
	private JLabel currentManeuverLabel;
	private JLabel currentManeuverValueLabel;
	
	private JLabel timeToCompletionLabel;
	private JLabel timeToCompletionValueLabel;
	
	private JLabel nextManeuverLabel;
	private JComboBox nextManeuverComboBox;
	
	public BehaviorControlPanel(BehaviorControlParam param) {
		super();
		this.setLayout(new GridBagLayout());
		
		GridBagConstraints labelConstraints = new GridBagConstraints();
		labelConstraints.anchor = GridBagConstraints.EAST;
		labelConstraints.gridx = 0;
		labelConstraints.gridy = 0;
		labelConstraints.ipadx = 5;
		labelConstraints.ipady = 5;
		
		GridBagConstraints centerControlConstraints = new GridBagConstraints();
		centerControlConstraints.anchor = GridBagConstraints.WEST;
		centerControlConstraints.gridx = 1;
		centerControlConstraints.gridy = 0;
		centerControlConstraints.ipadx = 5;
		centerControlConstraints.ipady = 5;
				
		speedLabel = new JLabel("Speed:");
		this.add(speedLabel, labelConstraints);
		
		speedSlider = new JSlider(JSlider.HORIZONTAL, param.minSpeed, param.maxSpeed, 0);
		this.add(speedSlider, centerControlConstraints);
		
		currentManeuverLabel = new JLabel("Current Maneuver:");
		labelConstraints.gridy = 1;
		this.add(currentManeuverLabel, labelConstraints);
		
		currentManeuverValueLabel = new JLabel("None");
		centerControlConstraints.gridy = 1;
		this.add(currentManeuverValueLabel, centerControlConstraints);
		
		timeToCompletionLabel = new JLabel("Time to Completion:");
		labelConstraints.gridy = 2;
		this.add(timeToCompletionLabel, labelConstraints);
		
		timeToCompletionValueLabel = new JLabel(getTimeString(0,0));
		centerControlConstraints.gridy = 2;
		this.add(timeToCompletionValueLabel, centerControlConstraints);
		
		nextManeuverLabel = new JLabel("Next Maneuver");
		labelConstraints.gridy = 3;
		this.add(nextManeuverLabel, labelConstraints);
		
		nextManeuverComboBox = new JComboBox();
		centerControlConstraints.gridy = 3;
		this.add(nextManeuverComboBox, centerControlConstraints);
		
	}
	
	private static String getTimeString (int minutes, int seconds) {
		String string = "";
		if (minutes < 10) {
			string += '0';
		}
		
		string += minutes + " : ";
		
		if (seconds < 10) {
			string += '0';
		}
		
		string += seconds;
		
		return string;
	}
}
