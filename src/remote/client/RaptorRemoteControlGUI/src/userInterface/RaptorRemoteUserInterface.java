package userInterface;
import java.awt.EventQueue;
import java.awt.Toolkit;

import javax.swing.JFrame;
import java.awt.Dimension;
import javax.swing.JToolBar;
import java.awt.BorderLayout;
import java.awt.event.ActionEvent;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.util.Hashtable;
import java.util.Map.Entry;

import javax.swing.AbstractAction;
import javax.swing.JButton;
import javax.swing.JFileChooser;
import javax.swing.JMenu;
import javax.swing.JMenuItem;
import javax.swing.JOptionPane;
import javax.swing.SpringLayout;
import javax.swing.JPanel;
import javax.swing.JMenuBar;
import javax.swing.JSplitPane;
import javax.swing.JTextPane;

import org.jdesktop.swingx.MultiSplitLayout;
import org.jdesktop.swingx.MultiSplitPane;

import videoStreaming.*;
import sessionManagement.*;


public class RaptorRemoteUserInterface {

	private JFrame frame;
	
	
	/*
	 * Menu bar members
	 */
	private JMenuBar menuBar;
		private JMenu connectionSettingsMenu;
			private JMenuItem openConnection;
			private JMenuItem closeConnection;
		
		private JMenu videoSettingsMenu;
			private JMenuItem stopVideoFeed;
			private JMenuItem initVideoFeed;
			
		private JMenu behaviorSettingsMenu;
			private JMenuItem loadScriptFile;
			
	public JFileChooser scriptFileChooser;
	
	public VideoPanel videoPanel;
	public ConsolePanel consolePanel;
	public BehaviorControlPanel behaviorControlPanel;
	public DepthMapPanel depthMapPanel;
	
	public RaptorRemoteSession remoteSession;
	public VideoStreamer videoStreamer;
	public StreamingVideoFrameLoader frameLoader;

	/**
	 * Launch the application.
	 */
	public static void main(String[] args) {		
		EventQueue.invokeLater(new Runnable() {
			public void run() {
				RaptorRemoteUserInterface window = null;
				try {
					window = new RaptorRemoteUserInterface();
					window.frame.setVisible(true);
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
		});
		
		
	}

	/**
	 * Create the application.
	 */
	public RaptorRemoteUserInterface() {
		initialize();
	}

	/**
	 * Initialize the contents of the frame.
	 */
	private void initialize() {
		Toolkit.getDefaultToolkit().setDynamicLayout(true);
		frame = new JFrame();
		frame.setLayout(new BorderLayout());
		frame.setMinimumSize(new Dimension(800, 400));
		frame.setBounds(100, 100, 1200, 600);
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		
		scriptFileChooser = new JFileChooser();
		scriptFileChooser.setFileSelectionMode(JFileChooser.FILES_ONLY);
		
		MultiSplitPane multiSplitPane = new MultiSplitPane();
		
		//String layoutDef =
		//    "(COLUMN (ROW weight=1.0 left (COLUMN middle.top middle middle.bottom) right) bottom)";
		
		String layoutDef = "(ROW (LEAF name=topleft weight=0.5) (LEAF name=bottomright weight=0.5))";
			//"(COLUMN (ROW weight=0.5 (LEAF name=topleft weight=0.5) (LEAF name=topright weight=0.5)) (ROW weight=0.5 (LEAF name=bottomleft weight=0.5) (LEAF name=bottomright weight=0.5)) )";
		
		MultiSplitLayout.Node modelRoot = MultiSplitLayout.parseModel(layoutDef);
		
		multiSplitPane.getMultiSplitLayout().setModel(modelRoot);
		
		videoPanel = new VideoPanel(this);
		multiSplitPane.add(videoPanel, "topleft");
		
		videoStreamer = new VideoStreamer();
		frameLoader = new StreamingVideoFrameLoader(videoPanel, videoStreamer, 30);
		
		//multiSplitPane.add(new UIPanel(this), "topright");
		//multiSplitPane.add(new UIPanel(this), "bottomleft");
		
		behaviorControlPanel = new BehaviorControlPanel(this);
		multiSplitPane.add(behaviorControlPanel, "bottomright");
		
		//consolePanel = new ConsolePanel();
		//multiSplitPane.add(consolePanel, "consolearea");
		
		//depthMapPanel = new DepthMapPanel(this, 10);
		//multiSplitPane.add(depthMapPanel, "topright");

		frame.getContentPane().add(multiSplitPane, BorderLayout.CENTER);
		
		
		// Set up Menu Bar
		// Set up Connection Settings
		menuBar = new JMenuBar();
		frame.getContentPane().add(menuBar, BorderLayout.NORTH);
		connectionSettingsMenu = new JMenu("Connection");
		menuBar.add(connectionSettingsMenu);
		openConnection = new JMenuItem(new OpenConnectionAction());
		openConnection.setText("Open Device Connection");
		closeConnection = new JMenuItem(new CloseConnectionAction());
		closeConnection.setText("Close Device Connection");
		connectionSettingsMenu.add(openConnection);
		connectionSettingsMenu.add(closeConnection);

		//Set up Video Settings
		videoSettingsMenu = new JMenu("Video");
		menuBar.add(videoSettingsMenu);
		initVideoFeed = new JMenuItem(new StartVideoAction());
		initVideoFeed.setText("Initialize Video Feed");
		stopVideoFeed = new JMenuItem(new StopVideoAction());
		stopVideoFeed.setText("Stop Video Feed");
		videoSettingsMenu.add(initVideoFeed);
		videoSettingsMenu.add(stopVideoFeed);
		
		//Set up Behavior Settings
		behaviorSettingsMenu = new JMenu("Behaviors");
		menuBar.add(behaviorSettingsMenu);
		loadScriptFile = new JMenuItem(new LoadScriptFileAction());
		loadScriptFile.setText("Upload Script File");
		behaviorSettingsMenu.add(loadScriptFile);
		
	}
	
	public void startVideo(VideoStreamer _videoStreamer) {
		if (this.videoStreamer != null) {
			return;
		}
		
		this.videoStreamer = videoStreamer;
		frameLoader = new StreamingVideoFrameLoader(this.videoPanel, this.videoStreamer, 30);
	}
	
	private class OpenConnectionAction extends AbstractAction {
		public void actionPerformed(ActionEvent e) {
			if (remoteSession != null && remoteSession.isActive()) {
				JOptionPane.showMessageDialog(null, "Session is already active", "Error", JOptionPane.WARNING_MESSAGE);
				return;
			}
			
			Hashtable<String, String> foundDevices = RaptorRemoteClient.pollNetworkDevices();
			
			if (foundDevices.size() == 0) {
				JOptionPane.showMessageDialog(null, "No devicves found on network", "Error", JOptionPane.WARNING_MESSAGE);
				return;
			}
			
			String [] deviceNames = new String[foundDevices.size()];
						
			int i = 0;
			for (Entry<String, String> curFoundDevice : foundDevices.entrySet()) {
				deviceNames[i++] = curFoundDevice.getValue() + " : " + curFoundDevice.getKey();
			}
			
			String selection = (String)JOptionPane.showInputDialog(null, "Select from available devices", "Choose Device", JOptionPane.PLAIN_MESSAGE, null, deviceNames, deviceNames[0]); 
			
			remoteSession = RaptorRemoteClient.initNewSession(RaptorRemoteUserInterface.this, selection.substring(selection.indexOf(':') + 2, selection.length()));
			
			if (remoteSession == null) {
				JOptionPane.showMessageDialog(null, "Could not open connection, device responded with an error", "Error", JOptionPane.WARNING_MESSAGE);
				return;
			}
			
			return;
		}
	}
	
	private class CloseConnectionAction extends AbstractAction {
		public void actionPerformed(ActionEvent e) {
			if (remoteSession == null || !remoteSession.isActive()) {
				JOptionPane.showMessageDialog(null, "No open device conneciton to close", "Error", JOptionPane.WARNING_MESSAGE);
				return;
			}
			
			remoteSession.close();
		}
	}
	
	private class StartVideoAction extends AbstractAction {
		public void actionPerformed(ActionEvent e) {
			if (remoteSession == null || !remoteSession.isActive()) {
				JOptionPane.showMessageDialog(null, "No open connection to start video feed", "Error", JOptionPane.WARNING_MESSAGE);
				return;
			}
			
			if (!frameLoader.isActive()) {
				frameLoader.start();
			}
			
			if (!videoStreamer.isAlive()) {
				videoStreamer.start();
			}
			
			boolean success = remoteSession.initVideoStream(0, 20);
			
			if (!success) {
				JOptionPane.showMessageDialog(null, "Device replied with an error when trying to open video stream", "Error", JOptionPane.WARNING_MESSAGE);
				return;
			}
			
			return;
		}
	}
	
	private class StopVideoAction extends AbstractAction {
		public void actionPerformed(ActionEvent e) {
			if (remoteSession == null || !remoteSession.isActive()) {
				JOptionPane.showMessageDialog(null, "No open connection to stop video feed", "Error", JOptionPane.WARNING_MESSAGE);
				return;
			}
			
			remoteSession.stopVideoStream();

		}
	}
	
	private class LoadScriptFileAction extends AbstractAction {
		public void actionPerformed(ActionEvent e) {
			if (remoteSession == null || !remoteSession.isActive()) {
				JOptionPane.showMessageDialog(null, "No open connection to upload script file", "Error", JOptionPane.WARNING_MESSAGE);
				return;
			}
			
			if (RaptorRemoteUserInterface.this.scriptFileChooser.showOpenDialog(null) == JFileChooser.APPROVE_OPTION) {
				File file = RaptorRemoteUserInterface.this.scriptFileChooser.getSelectedFile();
				BufferedReader fileReader = null;
				try {
					fileReader = new BufferedReader(new FileReader(file));
				} catch (FileNotFoundException e1) {
					JOptionPane.showMessageDialog(null, "Error could not open selected file " + file.getName(), "Error", JOptionPane.WARNING_MESSAGE);
					return;
				}
				
				String scriptString = "";
				try {
					if (fileReader != null) {
						int readChar = (char)fileReader.read();
						while (readChar >= 0) {
							scriptString += (char) readChar;
							System.out.println(scriptString);
							readChar = fileReader.read();
						}
					}
				} catch (IOException e2) {
					JOptionPane.showMessageDialog(null, "Error reading file " + file.getName(), "Error", JOptionPane.WARNING_MESSAGE);
					return;
				}
				
				int selectedOption = JOptionPane.showConfirmDialog(null, "Confirm upload script to device for immediate execution.", "Confirm Upload", JOptionPane.YES_NO_OPTION);
				
				if (selectedOption == JOptionPane.YES_OPTION) {
					remoteSession.sendScript(scriptString);
				} else if (selectedOption == JOptionPane.NO_OPTION) {
					return;
				}
			}

		}
	}
}
