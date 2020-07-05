from unittest import TestCase
from primerandom import *


class TestPrimeRandom(TestCase):
    def test_powmod(self):
        self.assertTrue(is_prime(2))
        self.assertTrue(is_prime(97))
        self.assertTrue(is_prime(999999937))

    def test_get_next(self):
        self.assertEqual(get_next(2687), 2689)
        self.assertEqual(get_next(3529), 3533)

    def test_get_random_array(self):
        self.assertTrue(all(is_prime(v) for v in get_random_array(10)))
