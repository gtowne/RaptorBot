package userInterface;

import java.awt.BorderLayout;
import java.awt.Canvas;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Point;
import java.util.concurrent.Semaphore;

import javax.swing.JPanel;

public class DepthMapPanel extends UIPanel {
	private enum DataOrdering {LEFT_TO_RIGHT, RIGHT_TO_LEFT};
	
	private JPanel drawingArea;
	
	private double[] curDepthMapData;
	private double maxDisplayRadius;
	
	private Semaphore updateMutex;
	
	public DepthMapPanel(RaptorRemoteUserInterface parentUI, double maxDisplayRadius) {
		super(parentUI);
		
		updateMutex = new Semaphore(1);
		this.maxDisplayRadius = maxDisplayRadius;
		this.setVisible(true);
		this.repaint();
	}

	public void update(Graphics g) {
		paintComponent(g);
	}

	public void paintComponent(Graphics g) {
		super.paintComponent(g);
		redrawDepth((Graphics2D) g);
	}

	public void redrawDepth(Graphics2D g) {
		try {
			DepthMapPanel.this.updateMutex.acquire();
		} catch (InterruptedException e) {
			e.printStackTrace();
		}

		g.setColor(Color.red);

		Dimension curCanvasSize = this.getSize();

		Point originPoint = new Point(curCanvasSize.width / 2, 0);

		int circleRadius;
		g.drawOval(originPoint.x, originPoint.y, 5, 5);

		DepthMapPanel.this.updateMutex.release();
	}




}
