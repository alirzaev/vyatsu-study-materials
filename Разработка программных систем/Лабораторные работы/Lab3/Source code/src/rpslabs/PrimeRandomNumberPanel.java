package rpslabs;

import java.awt.Dimension;
import java.awt.event.ActionEvent;

import javax.swing.Box;
import javax.swing.BoxLayout;
import javax.swing.JButton;
import javax.swing.JPanel;
import javax.swing.JTextField;
import javax.swing.border.EmptyBorder;

public class PrimeRandomNumberPanel extends JPanel {

	private static final long serialVersionUID = 1L;
	
	private static final String PRIME_NUMBER_BUTTON_LABEL = "Выдать простое случайное число";
	
	private PrimeRandom random = new PrimeRandom();
	
	private JTextField primeNumberField;
	
	private JButton primeNumberButton;

	public PrimeRandomNumberPanel() {
		super();
		
		this.setLayout(new BoxLayout(this, BoxLayout.X_AXIS));
		this.setBorder(new EmptyBorder(10, 10, 10, 10));
		
		primeNumberField = new JTextField();
		primeNumberField.setMaximumSize(new Dimension(200, 30));
		primeNumberField.setEditable(false);
		
		primeNumberButton = new JButton(PRIME_NUMBER_BUTTON_LABEL);
		primeNumberButton.addActionListener(this::onPrimeNumberButtonClicked);
		
		this.add(primeNumberButton);
		this.add(Box.createHorizontalStrut(10));
		this.add(primeNumberField);
	}
	
	private void onPrimeNumberButtonClicked(ActionEvent event) {
		primeNumberField.setText(Integer.toString(random.getRandomNumber()));
	}
	
}
