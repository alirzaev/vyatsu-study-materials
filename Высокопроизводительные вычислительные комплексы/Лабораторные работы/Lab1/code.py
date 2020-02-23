from math import factorial as fact
from functools import reduce


def c_m_n(m, n):
    return fact(n) / (fact(m) * fact(n - m))


def p_m_n(m, n, p):
    return c_m_n(m, n) * (p ** m) * ((1 - p) ** (n - m))


def r_m_n(m, n, p):
    return 1 - sum(p_m_n(i, n, p) for i in range(0, m))

t1 = reduce(lambda x, y: x * y, [0.98,0.88,0.88,0.955,0.955,0.88,0.88,0.88,0.88,0.955,0.955,0.88,0.88])
print(round(t1, 6))

t2 = r_m_n(1, 2, 0.98) * r_m_n(1, 2, 0.955 ** 4) * r_m_n(1, 2, 0.88 ** 8)
print(round(t2, 7))

t3 = r_m_n(1, 3, 0.98) * r_m_n(1, 3, 0.955 ** 4) * r_m_n(1, 3, 0.88 ** 8)
print(round(t3, 6))

t4 = 0.98 ** 2 * r_m_n(4, 5, 0.955) * r_m_n(8, 9, 0.88)
print(round(t4, 6))

t5 = 0.98 ** 2 * r_m_n(4, 6, 0.955) * r_m_n(8, 10, 0.88)
print(round(t5, 6))

t6 = 0.98 ** 2 * r_m_n(4, 8, 0.955) * r_m_n(8, 16, 0.88)
print(round(t6, 6))