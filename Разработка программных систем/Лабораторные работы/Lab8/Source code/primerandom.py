from typing import List

_LAST_PRIME_NUMBER = 999999937


def _powmod(a: int, n: int, m: int) -> int:
    """
    This method computes n-th power of number by modulo

    :param a: Base
    :param n: Exponent
    :param m: Modulo
    :return: a ^ n mod m
    """
    res = 1
    while n > 0:
        if n % 2 != 0:
            res = (res * a) % m
            n -= 1
        else:
            a = (a * a) % m
            n >>= 1

    return res


def _pollard_p1_test(number: int) -> bool:
    """
    This method determines whether given number is is a probable prime
    using Pollard's p − 1 algorithm

    :param number: Testing number
    :return: True if number is probable prime or False otherwise
    """
    from math import gcd, log, floor

    b = 13
    q = (2, 3, 5, 7, 11, 13)

    a = 5 % number
    for _ in range(10):
        while gcd(a, number) != 1:
            a = (a * a) % number
            a += 3
            a %= number

        for v in q:
            e = int(floor(log(b, v)))
            aa = int(pow(a, pow(v, e, number), number))

            if aa == 0:
                continue

            g = gcd(aa - 1, number)
            if 1 < g < number:
                return g == 1

    return True


def is_prime(number: int) -> bool:
    """
    This method determines whether number is prime or not

    :param number: Testing number
    :return: True if number is prime or False otherwise
    """
    return number == 2 or _pollard_p1_test(number)


def get_next(number: int) -> int:
    """
    This method finds prime number following given argument

    :param number: Current number
    :return: Prime number or -1 if there is no one
    """
    for i in range(number + 1, 10 ** 9):
        if is_prime(i):
            return i
    else:
        return -1


def get_random_number() -> int:
    """
    Random prime number generator

    :return: Random prime number between 2 and 1 000 000 000
    """
    from random import randint

    n = randint(2, _LAST_PRIME_NUMBER)
    if is_prime(n):
        return n
    else:
        return get_next(n)


def get_random_array(length: int) -> List[int]:
    """
    Generate sorted array of random prime numbers

    :param length: The length of the array
    :return: Array of random prime numbers
    """
    return sorted(get_random_number() for _ in range(length))
