package rpslabs;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;

/**
 * The Lab1 program implements application 
 * that generates random prime numbers using PrimeRandom class
 * 
 * @author Ali Rzaev (alirzaev1996@gmail.com)
 * @version 0.0.1
 * @since 2017-09-18
 */
public class Lab1 {

	/**
	 * This is the main method which makes use of PrimeRandom methods
	 * 
	 * @param args Unused
	 * @throws IOException On input error
	 * @see IOException
	 */
	public static void main(String[] args) throws IOException {
		PrimeRandom rnd = new PrimeRandom();
		BufferedReader reader = new BufferedReader(new InputStreamReader(System.in));
		
		while (true) {
			System.out.print(
					"Выберите действие:\n" +
					"1. Выдать случайное простое число\n" +
					"2. Выдать массив случаных чисел\n" +
					"3. Выйти\n");
			String line = reader.readLine();
			int c;
			try {
				c = Integer.parseInt(line);
			} catch (NumberFormatException ignore) {
				System.out.println("Неизвестная команда");
				continue;
			}
			
			if (c == 1) {
				System.out.println(rnd.getRandomNumber());
			} else if (c == 2) {
				System.out.println("Введите длину массива:");
				int length;
				while (true) {
					line = reader.readLine();
					try {
						length = Integer.parseInt(line);
					} catch (NumberFormatException ignore) {
						System.out.println("Длина массива должна быть целым положительным числом");
						continue;
					}
					
					if (length <= 0) {
						System.out.println("Длина массива должна быть целым положительным числом");
						continue;
					} else {
						break;
					}
				}
				
				int[] array = rnd.getRandomArray(length);
				for (int v : array) {
					System.out.println(v);
				}
			} else if (c == 3) {
				break;
			} else {
				System.out.println("Неизвестная команда");
			}
		}
	}

}
