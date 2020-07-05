from typing import List

_LAST_PRIME_NUMBER = 999999937


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
        
    e = lambda b, v: int(floor(log(b, v)))
        
    aa = lambda a, v: int(pow(a, pow(v, e(b, v), number), number))
        
    red = lambda a: a if gcd(a, number) == 1 else ((a ** 2) % number + 3) % number

    test = lambda a: len([gcd(aa(a, v), number) for v in q if aa(a, v) != 0 and 1 < gcd(aa(a, v), number) < number]) > 0

    gen = lambda a, n: [red(a)] + gen(red(a), n - 1) if n > 0 else []

    return len([i for i in gen(a, 10) if test(i)]) == 0


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
    lrec = lambda n: -1 if n == 10 ** 9 else n if is_prime(n) else lrec(n + 1)
            
    return lrec(number + 1)


def get_random_number() -> int:
    """
    Random prime number generator

    :return: Random prime number between 2 and 1 000 000 000
    """
    from random import randint

    n = randint(2, _LAST_PRIME_NUMBER)
    
    return n if is_prime(n) else get_next(n)


def get_random_array(length: int) -> List[int]:
    """
    Generate sorted array of random prime numbers

    :param length: The length of the array
    :return: Array of random prime numbers
    """
    return sorted(get_random_number() for _ in range(length))
