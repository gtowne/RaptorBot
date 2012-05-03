package userInterface;
import java.awt.BorderLayout;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.DefaultComboBoxModel;
import javax.swing.JButton;
import javax.swing.JComboBox;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JSlider;
import javax.swing.SwingConstants;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;

import behaviorControl.BehaviorParam;

public class BehaviorControlPanel extends UIPanel {

	protected JPanel infoPanel;
	protected JPanel editPanel;
	
	protected JLabel speedLabel;
	protected JSlider speedSlider;

	protected JLabel currentManeuverLabel;
	protected JLabel currentManeuverValueLabel;
	
	protected JLabel nextManeuverLabel;
	protected JLabel nextManeuverValueLabel;

	protected JLabel timeToCompletionLabel;
	protected JLabel timeToCompletionValueLabel;

	protected JLabel newManeuverLabel;
	protected JComboBox newManeuverComboBox;

	protected JButton updateManeuverButton;
	protected JButton enqueueManeuverButton;

	protected JLabel maneuverParametersLabel;

	protected JLabel directionLabel;
	protected JComboBox directionComboBox;
	protected DefaultComboBoxModel moveDirectionComboBoxModel;
	protected DefaultComboBoxModel pivotDirectionComboBoxModel;
	protected DefaultComboBoxModel arcDirectionComboBoxModel;

	private JLabel distanceLabel;
	private JSlider distanceSlider;
	
	private JLabel degreesLabel;
	private JSlider degreesSlider;
	
	private JLabel radiusLabel;
	private JSlider radiusSlider;


	public BehaviorControlPanel(RaptorRemoteUserInterface parentUI) {
		super(parentUI);		
		this.setLayout(new BorderLayout());
		
		infoPanel = new JPanel(new GridBagLayout());
		editPanel = new JPanel(new GridBagLayout());
		
		this.add(infoPanel, BorderLayout.PAGE_START);
		this.add(editPanel, BorderLayout.CENTER);

		GridBagConstraints labelConstraints = new GridBagConstraints();
		labelConstraints.weightx = .1;
		labelConstraints.anchor = GridBagConstraints.EAST;
		labelConstraints.gridx = 0;
		labelConstraints.gridy = 0;
		labelConstraints.ipadx = 5;
		labelConstraints.ipady = 5;

		GridBagConstraints centerControlConstraints = new GridBagConstraints();
		centerControlConstraints.weightx = .8;
		centerControlConstraints.anchor = GridBagConstraints.WEST;
		centerControlConstraints.gridx = 1;
		centerControlConstraints.gridy = 0;
		centerControlConstraints.ipadx = 5;
		centerControlConstraints.ipady = 5;

		currentManeuverLabel = new JLabel("Current Maneuver:");
		labelConstraints.gridy++;
		infoPanel.add(currentManeuverLabel, labelConstraints);

		currentManeuverValueLabel = new JLabel("None");
		centerControlConstraints.gridy++;
		infoPanel.add(currentManeuverValueLabel, centerControlConstraints);

		timeToCompletionLabel = new JLabel("Time to Completion:");
		labelConstraints.gridy++;
		infoPanel.add(timeToCompletionLabel, labelConstraints);

		timeToCompletionValueLabel = new JLabel(getTimeString(0,0));
		centerControlConstraints.gridy++;
		infoPanel.add(timeToCompletionValueLabel, centerControlConstraints);
		
		nextManeuverLabel = new JLabel("Next Queued Maneuver:");
		labelConstraints.gridy++;
		infoPanel.add(nextManeuverLabel, labelConstraints);
		
		nextManeuverValueLabel = new JLabel("None");
		centerControlConstraints.gridy++;
		infoPanel.add(nextManeuverValueLabel, centerControlConstraints);
		
		labelConstraints = new GridBagConstraints();
		labelConstraints.weightx = .1;
		labelConstraints.anchor = GridBagConstraints.EAST;
		labelConstraints.gridx = 0;
		labelConstraints.gridy = 0;
		labelConstraints.ipadx = 5;
		labelConstraints.ipady = 5;

		centerControlConstraints = new GridBagConstraints();
		centerControlConstraints.weightx = .8;
		centerControlConstraints.anchor = GridBagConstraints.WEST;
		centerControlConstraints.gridx = 1;
		centerControlConstraints.gridy = 0;
		centerControlConstraints.ipadx = 5;
		centerControlConstraints.ipady = 5;

		GridBagConstraints rightControlConstraints = new GridBagConstraints();
		rightControlConstraints.weightx = .8;
		rightControlConstraints.anchor = GridBagConstraints.WEST;
		rightControlConstraints.gridx = 2;
		rightControlConstraints.gridy = 0;
		rightControlConstraints.ipadx = 5;
		rightControlConstraints.ipady = 5;
		
		speedLabel = new JLabel("Speed (m/s):");
		labelConstraints.gridy++;
		editPanel.add(speedLabel, labelConstraints);

		speedSlider = new JSlider(JSlider.HORIZONTAL, BehaviorParam.MIN_SPEED, BehaviorParam.MAX_SPEED, 0);
		speedSlider.setPaintTicks(true);
		speedSlider.setMajorTickSpacing((BehaviorParam.MAX_SPEED - BehaviorParam.MIN_SPEED) / 5);
		speedSlider.setMinorTickSpacing(1);
		speedSlider.setPaintLabels(true);
		centerControlConstraints.gridy++;
		editPanel.add(speedSlider, centerControlConstraints);

		newManeuverLabel = new JLabel("Edit Maneuver:");
		labelConstraints.gridy++;
		editPanel.add(newManeuverLabel, labelConstraints);

		newManeuverComboBox = new JComboBox(BehaviorParam.BEHAVIOR_NAMES);
		centerControlConstraints.gridy++;
		editPanel.add(newManeuverComboBox, centerControlConstraints);
		newManeuverComboBox.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				BehaviorControlPanel.this.update();
			}
		});

		

		maneuverParametersLabel = new JLabel("Maneuver Parameters:");
		centerControlConstraints.gridy++;
		editPanel.add(maneuverParametersLabel, centerControlConstraints);
		
		int maneuverParametersLabelY = centerControlConstraints.gridy;

		directionLabel = new JLabel("Direction:");
		centerControlConstraints.anchor = GridBagConstraints.EAST;
		centerControlConstraints.gridy = maneuverParametersLabelY + 1;
		editPanel.add(directionLabel, centerControlConstraints);

		moveDirectionComboBoxModel = new DefaultComboBoxModel(BehaviorParam.MOVE_DIRECTION_NAMES);
		pivotDirectionComboBoxModel = new DefaultComboBoxModel(BehaviorParam.PIVOT_DIRECTION_NAMES);
		arcDirectionComboBoxModel = new DefaultComboBoxModel(BehaviorParam.ARC_DIRECTION_NAMES);

		directionComboBox = new JComboBox();
		rightControlConstraints.gridy = maneuverParametersLabelY + 1;
		directionComboBox.setVerifyInputWhenFocusTarget(false);
		editPanel.add(directionComboBox, rightControlConstraints);
		directionComboBox.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				BehaviorControlPanel.this.update();
			}
		});
		
		distanceLabel = new JLabel("Distance (m)");
		centerControlConstraints.gridy++;
		editPanel.add(distanceLabel, centerControlConstraints);
		
		distanceSlider = new JSlider(JSlider.HORIZONTAL, 0, 100, 0);
		distanceSlider.setMajorTickSpacing(10);
		distanceSlider.setMinorTickSpacing(2);
		distanceSlider.setPaintTicks(true);
		distanceSlider.setPaintLabels(true);
		rightControlConstraints.gridy++;
		editPanel.add(distanceSlider, rightControlConstraints);
		distanceSlider.addChangeListener(new ChangeListener() {
			public void stateChanged(ChangeEvent e) {
				BehaviorControlPanel.this.update();
			}
		});

		degreesLabel = new JLabel("Degrees");
		centerControlConstraints.gridy++;
		editPanel.add(degreesLabel, centerControlConstraints);

		degreesSlider = new JSlider(JSlider.HORIZONTAL, 0, 90, 0);
		degreesSlider.setMajorTickSpacing(10);
		degreesSlider.setMinorTickSpacing(5);
		degreesSlider.setPaintTicks(true);
		degreesSlider.setPaintLabels(true);
		rightControlConstraints.gridy++;
		editPanel.add(degreesSlider, rightControlConstraints);
		degreesSlider.addChangeListener(new ChangeListener() {
			public void stateChanged(ChangeEvent e) {
				BehaviorControlPanel.this.update();
			}
		});
		
		radiusLabel = new JLabel("Radius (m)");
		centerControlConstraints.gridy++;
		editPanel.add(radiusLabel, centerControlConstraints);
		
		radiusSlider = new JSlider(JSlider.HORIZONTAL, 0, 20, 0);
		radiusSlider.setMajorTickSpacing(5);
		radiusSlider.setMinorTickSpacing(1);
		radiusSlider.setPaintTicks(true);
		radiusSlider.setPaintLabels(true);
		rightControlConstraints.gridy++;
		editPanel.add(radiusSlider, rightControlConstraints);
		
		updateManeuverButton = new JButton("Update Next");
		centerControlConstraints.gridy = Math.max(rightControlConstraints.gridy, centerControlConstraints.gridy) + 1;
		centerControlConstraints.anchor = GridBagConstraints.CENTER;
		editPanel.add(updateManeuverButton, centerControlConstraints);
		updateManeuverButton.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				BehaviorControlPanel.this.handleSendManeuver(true);
			}
		});
		
		enqueueManeuverButton = new JButton("Enqueue");
		rightControlConstraints.gridy = centerControlConstraints.gridy;
		rightControlConstraints.anchor = GridBagConstraints.CENTER;
		editPanel.add(enqueueManeuverButton, rightControlConstraints);
		enqueueManeuverButton.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				BehaviorControlPanel.this.handleSendManeuver(false);
			}
		});

		update();

	}

	// if parameter setNext is true, then this maneuver will replace the next
	// maneuver currently at the head of the queue, else it will be considered
	// and enqueue message and will go to the back of the queue
	protected void handleSendManeuver(boolean setNext) {
		if (this.parentUI.remoteSession == null || !this.parentUI.remoteSession.isActive()) {
			JOptionPane.showMessageDialog(null, "No session is currently active to update maneuver.", "Error", JOptionPane.WARNING_MESSAGE);
			return;
		}
		
		BehaviorParam.Maneuver selectedManeuver =  getManeuverByIndex(this.newManeuverComboBox.getSelectedIndex());

		int speedSetting = this.speedSlider.getValue();
		int degreesSetting = this.degreesSlider.getValue();
		int radiusSetting = this.radiusSlider.getValue();
		int distanceSetting = this.distanceSlider.getValue();

		switch (selectedManeuver) {
		case MOVE:

			BehaviorParam.MoveDirection moveDir = getMoveDirectionByIndex(this.directionComboBox.getSelectedIndex());

			this.parentUI.remoteSession.sendMoveCommand(moveDir, speedSetting, distanceSetting, setNext);

			break;

		case ARC_TURN:

			BehaviorParam.ArcDirection arcDir = getArcDirectionByIndex(this.directionComboBox.getSelectedIndex());

			this.parentUI.remoteSession.sendArcCommand(arcDir, speedSetting, degreesSetting, radiusSetting, setNext);
			break;

		case PIVOT_TURN:

			BehaviorParam.PivotDirection pivotDir = getPivotDirectionByIndex(this.directionComboBox.getSelectedIndex());

			this.parentUI.remoteSession.sendPivotCommand(pivotDir, degreesSetting, setNext);
			break;
		}

		this.update();
	}
	
	private void setViewForSelectedManeuver(BehaviorParam.Maneuver maneuver) {
		directionLabel.setEnabled(false);
		directionComboBox.setEnabled(false);
		degreesLabel.setEnabled(false);
		degreesSlider.setEnabled(false);
		radiusLabel.setEnabled(false);
		radiusSlider.setEnabled(false);
		distanceLabel.setEnabled(false);
		distanceSlider.setEnabled(false);

		switch (maneuver) {
		case MOVE:
			directionComboBox.setModel(moveDirectionComboBoxModel);
			directionLabel.setEnabled(true);
			directionComboBox.setEnabled(true);
			distanceLabel.setEnabled(true);
			distanceSlider.setEnabled(true);
			break;
			
		case ARC_TURN:
			directionComboBox.setModel(arcDirectionComboBoxModel);
			directionLabel.setEnabled(true);
			directionComboBox.setEnabled(true);

			degreesLabel.setEnabled(true);
			degreesSlider.setEnabled(true);
			
			radiusLabel.setEnabled(true);
			radiusSlider.setEnabled(true);
			break;

		case PIVOT_TURN:
			directionComboBox.setModel(pivotDirectionComboBoxModel);
			directionLabel.setEnabled(true);
			directionComboBox.setEnabled(true);

			degreesLabel.setEnabled(true);
			degreesSlider.setEnabled(true);
			break;
		}
	}
	
	public void updateCurrentManeuverString(String currentManeuver) {
		if (currentManeuver != null) {
			this.currentManeuverValueLabel.setText(currentManeuver);
		}
	}
	
	public void updateNextManeuverString(String nextManeuver) {
		if (nextManeuver != null) {
			this.nextManeuverValueLabel.setText(nextManeuver);
		}
	}
	
	public void updateTimeToCompletion(int _ms) {
		int secs = _ms / 1000;
		int ms = _ms - (secs * 1000);
		int mins = secs / 60;
		secs = secs - (mins * 60);
		
		String timeStr = "";
		
		if (mins <= 0) {
			timeStr += "00 : ";
		} else if (mins < 10) {
			timeStr += "0" + mins + " : ";
		} else if (mins >= 10) {
			timeStr += mins + " : ";
		}
		
		if (secs <= 0) {
			timeStr += "00 . ";
		} else if (secs < 10) {
			timeStr += "0" + secs + " . ";
		} else if (secs >= 10) {
			timeStr += secs + " . ";
		}
		
		timeStr += ms;
		
		this.timeToCompletionValueLabel.setText(timeStr);
		
	}

	private void update() {
		BehaviorParam.Maneuver selectedManeuver = getManeuverByIndex(newManeuverComboBox.getSelectedIndex());	
		setViewForSelectedManeuver(selectedManeuver);
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

	protected static BehaviorParam.Maneuver getManeuverByIndex(int index) {
		return BehaviorParam.Maneuver.values()[index];
	}

	protected static BehaviorParam.MoveDirection getMoveDirectionByIndex(int index) {
		return BehaviorParam.MoveDirection.values()[index];
	}

	protected static BehaviorParam.PivotDirection getPivotDirectionByIndex(int index) {
		return BehaviorParam.PivotDirection.values()[index];
	}

	protected static BehaviorParam.ArcDirection getArcDirectionByIndex(int index) {
		return BehaviorParam.ArcDirection.values()[index];
	}


}
