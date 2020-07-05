package rpslabs;

import java.awt.Dimension;

import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.JTabbedPane;

public class PrimeNumberGenerator implements Runnable {
	
	private static final String MAIN_FRAME_TITLE = "Генератор простых случайных чисел";
	
	private static final String NUMBER_PANEL_TITLE = "Случайное число";
	
	private static final String ARRAY_PANEL_TITLE = "Список случайных чисел";
	
	private JFrame mainFrame;
	
	@Override
	public void run() {
		mainFrame = new JFrame(MAIN_FRAME_TITLE);
		mainFrame.setPreferredSize(new Dimension(640, 480));
		
		JPanel primeRandomNumberPanel = new PrimeRandomNumberPanel();
		JPanel primeRandomArrayPanel = new PrimeRandomArrayPanel();
		
		JTabbedPane mainPane = new JTabbedPane();
		mainPane.addTab(NUMBER_PANEL_TITLE, primeRandomNumberPanel);
		mainPane.addTab(ARRAY_PANEL_TITLE, primeRandomArrayPanel);
		
		mainFrame.getContentPane().add(mainPane);
		
		mainFrame.pack();
		mainFrame.setVisible(true);
	}

}
