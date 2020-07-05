package rpslabs;

import java.util.Random;
import java.util.Arrays;

/**
 * PrimeRandom class implements generator for random prime numbers
 * 
 * @author Ali Rzaev (alirzaev1996@gmail.com)
 * @version 0.0.1
 * @since 2017-09-18
 */
public class PrimeRandom {
	
	/**
	 * Standard pseudo random number generator
	 */
	private Random _random;
	
	/**
	 * Biggest prime number not greater than 1 000 000 000
	 */
	private static final int LAST_PRIME_NUMBER = 999999937;

	/**
	 * Constructs new random prime number generator
	 */
	public PrimeRandom() {
		_random = new Random();
	}
	
	/**
	 * Random prime number generator
	 * 
	 * @return Random prime number between 2 and 1 000 000 000
	 */
	public int getRandomNumber() {
		int n = 2 + _random.nextInt(LAST_PRIME_NUMBER - 1);
		return isPrime(n) ? n : getNext(n);
	}

	/**
	 * Generate sorted array of random prime numbers
	 * 
	 * @param length The length of the array
	 * @return Array of random prime numbers
	 */
	public int[] getRandomArray(int length) {
		int[] array = new int[length];
		
		for (int i = 0; i < length; ++i) {
			array[i] = getRandomNumber();
		}
		Arrays.sort(array);
		
		return array;
	}
	
	/**
	 * This method finds prime number following given argument
	 * 
	 * @param number Current number
	 * @return Prime number or -1 if there is no one
	 */
	private int getNext(int number) {
		for (int i = number + 1; i < 1000000000; ++i) {
			if (isPrime(i)) {
				return i;
			}
		}
		return -1;
	}

	
	/**
	 * This method determines whether number is prime or not
	 * 
	 * @param number Testing number
	 * @return true if number is prime or false otherwise
	 */
	private boolean isPrime(int number) {
		if (number == 2) {
			return true;
		} else {
			return pollardP1Test(number) == 1 && fermatTest(number) == 1;
		}
	}

	/**
	 * This method determines whether given number is is a probable prime
	 * using Fermat primality test
	 * 
	 * @param number Testing number
	 * @return 1 if number is probable prime or other number otherwise
	 */
	private int fermatTest(int number) {
		int x = (int) Math.sqrt(number), y = 0;
		int r = x * x - y * y - number;
		for (;;) {
			if (r == 0) {
				return x != y ? x - y : x + y;
			} else {
				if (r > 0) {
					r -= y + y + 1;
					++y;
				} else {
					r += x + x + 1;
					++x;
				}
			}
		}
	}

	/**
	 * Recursive implementation of Euclidean algorithm
	 * 
	 * @param a First number
	 * @param b Second number
	 * @return gcd(a, b)
	 */
	private int gcd(int a, int b) {
		if (a == 0) {
			return b;
		} else {
			return gcd(b % a, a);
		}
	}

	/**
	 * This method implements product of two numbers by modulo
	 * 
	 * @param a First multiplier
	 * @param b Second multiplier
	 * @param m Modulo
	 * @return a * b mod m
	 */
	private int mulmod(long a, long b, long m) {
		return (int) ((a * b) % m);
	}

	/**
	 * This method computes n-th power of number by modulo
	 * 
	 * @param a Base
	 * @param n Exponent
	 * @param m Modulo
	 * @return a ^ n mod m
	 */
	private long powmod(long a, long n, long m) {
		long res = 1;
		for (int i = 0; i < n; ++i) {
			res = (res * a) % m;
		}
		return res;
	}

	/**
	 * This method determines whether given number is is a probable prime
	 * using Pollard's p − 1 algorithm
	 * 
	 * @param number Testing number
	 * @return 1 if number is probable prime or other number otherwise
	 */
	private int pollardP1Test(int number) {
		final int b = 13;
		final int[] q = { 2, 3, 5, 7, 11, 13 };

		int a = 5 % number;
		for (int j = 0; j < 10; ++j) {
			while (gcd(a, number) != 1) {
				a = mulmod(a, a, number);
				a += 3;
				a %= number;
			}

			for (int i = 0; i < q.length; ++i) {
				int qq = q[i];
				int e = (int) Math.floor(Math.log((double) b) / Math.log((double) qq));
				int aa = (int) powmod(a, powmod(qq, e, number), number);
				if (aa == 0) {
					continue;
				}

				int g = gcd(aa - 1, number);
				if (1 < g && g < number) {
					return g;
				}
			}
		}

		return 1;
	}
}
