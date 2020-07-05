package rpslabs;

import java.awt.Dimension;
import java.awt.event.ActionEvent;
import java.util.Arrays;
import java.util.stream.Collectors;

import javax.swing.Box;
import javax.swing.BoxLayout;
import javax.swing.ButtonGroup;
import javax.swing.JButton;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JRadioButton;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;
import javax.swing.JTextField;
import javax.swing.border.EmptyBorder;

public class PrimeRandomArrayPanel extends JPanel {

	private static final long serialVersionUID = 1L;
	
	private static final String ARRAY_LENGTH_LABEL = "Длина массива: ";
	
	private static final String PRIME_ARRAY_BUTTON_LABEL = "Выдать список случайных чисел";
	
	private static final String FORMAT_LABEL = "Формат данных: ";
	
	private static final String JSON_FORMAT = "JSON";
	
	private static final String XML_FORMAT = "XML";

	private static final int MAX_ARRAY_LENGTH = 10000000;
	
	private static final String INVALID_ARRAY_LENGTH = String.format(
			"Длина массива должна быть в диапазоне (0; %s]",
			MAX_ARRAY_LENGTH
	);
	
	private JButton primeArrayButton;
	
	private JTextArea primeArrayTextArea;
	
	private JTextField arrayLengthField;
	
	private JLabel arrayLengthLabel;

	private String dumpFormat = JSON_FORMAT;
	
	private PrimeRandom random = new PrimeRandom();
		
	public PrimeRandomArrayPanel() {
		super();
		
		this.setLayout(new BoxLayout(this, BoxLayout.Y_AXIS));
		this.setBorder(new EmptyBorder(10, 10, 10, 10));
		
		primeArrayButton = new JButton(PRIME_ARRAY_BUTTON_LABEL);
		primeArrayButton.addActionListener(this::onPrimeArrayButtonClicked);
		
		primeArrayTextArea = new JTextArea();
		primeArrayTextArea.setEditable(false);
				
		arrayLengthField = new JTextField();
		arrayLengthField.setMaximumSize(new Dimension(200, 30));
		arrayLengthLabel = new JLabel(ARRAY_LENGTH_LABEL);
		
		JPanel lengthPanel = new JPanel();
		lengthPanel.setLayout(new BoxLayout(lengthPanel, BoxLayout.X_AXIS));
		lengthPanel.add(arrayLengthLabel);
		lengthPanel.add(arrayLengthField);
		lengthPanel.add(primeArrayButton);
		
		JRadioButton selectJSON = new JRadioButton(JSON_FORMAT);
		selectJSON.setActionCommand(JSON_FORMAT);
		selectJSON.setSelected(true);
		selectJSON.addActionListener(e -> dumpFormat  = e.getActionCommand());
		
		JRadioButton selectXML = new JRadioButton(XML_FORMAT);
		selectXML.setActionCommand(XML_FORMAT);
		selectXML.addActionListener(e -> dumpFormat = e.getActionCommand());
		
		ButtonGroup formatGroup = new ButtonGroup();
		formatGroup.add(selectJSON);
		formatGroup.add(selectXML);
		
		JPanel formatPanel = new JPanel();
		formatPanel.setLayout(new BoxLayout(formatPanel, BoxLayout.X_AXIS));
		formatPanel.add(new JLabel(FORMAT_LABEL));
		formatPanel.add(selectJSON);
		formatPanel.add(selectXML);
		
		this.add(lengthPanel);
		this.add(Box.createVerticalStrut(10));
		this.add(formatPanel);
		this.add(Box.createVerticalStrut(10));
		this.add(new JScrollPane(primeArrayTextArea));
	}
	
	private String dumpArrayToJSON(int[] array) {
		StringBuilder serialized = new StringBuilder();
		serialized.append("[\n");
		serialized.append(
			Arrays.stream(array)
			  .mapToObj(Integer::toString)
			  .map(v -> "  " + v + "\n")
			  .collect(Collectors.joining(""))
		);
		serialized.append("]");
		
		return serialized.toString();
	}
	
	
	private String dumpArrayToXML(int[] array) {
		StringBuilder serialized = new StringBuilder();
		serialized.append("<?xml version=\"1.0\" ?>\n");
		serialized.append("<array>\n");
		serialized.append(
			Arrays.stream(array)
			  .mapToObj(Integer::toString)
			  .map(v -> "  <item>" + v + "</item>\n")
			  .collect(Collectors.joining(""))
		);
		serialized.append("</array>");
		
		return serialized.toString();
	}
	
	
	private void onPrimeArrayButtonClicked(ActionEvent event) {
		try {
			int length = Integer.parseInt(arrayLengthField.getText());
			
			if (length <= 0 || length > MAX_ARRAY_LENGTH) {
				JOptionPane.showMessageDialog(this, INVALID_ARRAY_LENGTH);
			} else {
				int[] array = random.getRandomArray(length);
				
				if (dumpFormat.equals(JSON_FORMAT)) {
					primeArrayTextArea.setText(dumpArrayToJSON(array));
				} else {
					primeArrayTextArea.setText(dumpArrayToXML(array));
				}
			}
		} catch (NumberFormatException ex) {
			JOptionPane.showMessageDialog(this, INVALID_ARRAY_LENGTH);
		}
	}
	
}
