from math import *
from functools import reduce


def newton_interpolation(h: float, func: list, eps: float, points: list):
    xs, ys = func
    n = len(ys)
    diffs = [0] * n

    diffs[0] = ys[:]

    for i in range(1, n):
        diffs[i] = [d2 - d1 for d1, d2 in zip(diffs[i - 1][:-1], diffs[i - 1][1:])]

    res = [0] * len(points)

    factorials = [factorial(n) for n in range(n)]
    for i in range(len(res)):
        q = (points[i] - xs[0]) / h
        #  1, q, q - 1, ... , q - n + 1
        qs = [reduce(lambda a, b: a * b, (q - i for i in range(j)), 1) for j in range(n)]

        res[i] = sum(qs[i] * diffs[i][0] / factorials[i] for i in range(n))

    return [round(y, int(-log10(eps))) for y in res]


func_newton = [
    (0.101, 0.106, 0.111, 0.116, 0.121, 0.126, 0.131, 0.136, 0.141, 0.146, 0.151),
    (1.26153, 1.27644, 1.29122, 1.30617, 1.32130, 1.33660, 1.35207, 1.36773, 1.38357, 1.39959, 1.41579)
]

h_newton = 0.005

x_newton = [0.1074, 0.1485, 0.1006, 0.1560]

eps_newton = 0.000001

y_newton = newton_interpolation(h_newton, func_newton, eps_newton, x_newton)

print('x\ty')
print('\n'.join('{}\t{}'.format(x, y) for x, y in zip(x_newton, y_newton)))


def lagrange_interpolation(x: float, func: list, eps: float):
    xs, ys = func
    n = len(xs)

    ls = [reduce(lambda a, b: a * b, ((x - xs[j]) / (xs[i] - xs[j]) for j in range(n) if i != j)) for i in range(n)]

    res = sum(y * l for y, l in zip(ys, ls))

    return round(res, int(-log10(eps)))


func_lagrange = [
    (0.02, 0.08, 0.12, 0.17, 0.23, 0.3),
    (1.02316, 1.09590, 1.14725, 2.21483, 2.30120, 2.40976)
]

x_lagrange = 0.125

eps_lagrange = 0.000001

print('y = {} при x = {}'.format(lagrange_interpolation(x_lagrange, func_lagrange, eps_lagrange), x_lagrange))


def ols_interpolation(func: list):
    sum_x = sum(func[0])
    sum_x2 = sum(x ** 2 for x in func[0])
    sum_y = sum(func[1])
    sum_xy = sum(x * y for x, y in zip(*func))
    n = len(func[0])

    d = sum_x2 * n - sum_x ** 2
    d_a = sum_xy * n - sum_y * sum_x
    d_b = sum_x2 * sum_y - sum_x ** 2

    a = d_a / d
    b = d_b / d

    return 'y = {} * x + {}'.format(round(a, 4), round(b, 4))

func_ols = [
    (4.0, 4.1, 4.2, 4.3, 4.4, 4.5),
    (1.23, 1.28, 1.33, 1.38, 1.44, 1.49)
]

print('Приближение функции выглядит следующим образом:\n{}'.format(ols_interpolation(func_ols)))
