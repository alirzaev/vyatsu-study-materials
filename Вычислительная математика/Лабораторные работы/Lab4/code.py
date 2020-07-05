from math import *
from functools import reduce


def trapezoid(func, derivation, rng, eps):
    a, b = rng
    n = abs(round((b - a) ** 3 * derivation(a) / (12 * eps)))
    h = (b - a) / n

    integral = sum(h * (func(a + h * i) + func(a + h * (i + 1))) / 2 for i in range(n))

    return round(integral, int(-log10(eps)))


func_trapezoid = lambda x: (0.5 * x ** 2 + 1.5) ** -0.5

deriv_trapezoid = lambda x: -0.5 * x * (0.5 * x ** 2 + 1.5) ** -1.5

rng_trapezoid = (1.2, 2.0)

eps_trapezoid = 0.0001

print('''Определённый интеграл от функции: 1/sqrt(0,5*x^2+1,5)
Пределы интегрирования: [1,2;2,0]''')
print('I = {}'.format(trapezoid(func_trapezoid, deriv_trapezoid, rng_trapezoid, eps_trapezoid)))


def simpson(func, rng, eps):
    n = 8
    a, b = rng

    while True:
        h1 = (b - a) / n
        h2 = (b - a) / (2 * n)

        i1 = h1 / 3 * (func(a) + func(b)
                       + 4 * sum(func(a + i * h1) for i in range(1, n - 1, 2))
                       + 2 * sum(func(a + i * h1) for i in range(2, n - 1, 2)))

        i2 = h2 / 3 * (func(a) + func(b)
                       + 4 * sum(func(a + i * h2) for i in range(1, 2 * n - 1, 2))
                       + 2 * sum(func(a + i * h2) for i in range(2, 2 * n - 1, 2)))

        if abs(i2 - i1) > eps:
            n += 1
        else:
            return round(i1, int(-log10(eps))), n


func_simpson = lambda x: (x + 1) * cos(x ** 2)

rng_simpson = (0.2, 1.0)

eps_simpson = 0.0001

print('''Определённый интеграл от функции: (x+1)*cos(x^2)
Пределы интегрирования: [0,2;1,0]''')
print('I = {}'.format(simpson(func_simpson, rng_simpson, eps_simpson)[0]))


def gauss(table1, table2, func, rng):
    a, b = rng
    res = [0.5 * (b - a) * sum(a * func((b - a) * x / 2 + (a + b) / 2) for a, x in zip(*table)) for table in (table1, table2)]
    return [round(v, 4) for v in (*res, abs(res[0] - res[1]))]

table1_gauss = [
    (0.129484966, 0.279705391, 0.381830051, 0.417959184, 0.381830051, 0.279705391, 0.129484966),
    (-0.949107912, -0.741531186, -0.405845151, 0, 0.405845151, 0.741531186, 0.949107912)
]

table2_gauss = [
    (0.34785, 0.65215, 0.65215, 0.34785),
    (-0.86114, -0.33998, 0.33998, 0.86114)
]

func_gauss = lambda x: (x ** 2 + 2) / (x ** 2 + 1) ** 0.5

rng_gauss = (-0.4, 1.8)

print('''Определённый интеграл от функции: (x^2+2)/sqrt(x^2+1)
Пределы интегрирования: [-0,4;1,8]''')
print('I(7) = {}, I(4) = {}, погрешность = {}'.format(*gauss(table1_gauss, table2_gauss, func_gauss, rng_gauss)))


def adams(func, begs, h, rng):
    a, b = rng
    res = []
    for d in (h, h / 2):
        ys = list(begs)
        xs = []
        fs = []
        x = a
        i = 0
        while i < len(ys):
            fs.append(func(x, ys[i]))
            xs.append(x)
            x += d
            i += 1

        while x <= b:
            d_f = fs[-1] - fs[-2]
            d2_f = fs[-1] - 2 * fs[-2] + fs[-3]
            d3_f = fs[-1] - 3 * fs[-2] + 3 * fs[-3] - fs[-4]

            y = ys[-1] + h * fs[-1] + \
                0.5 * (h ** 2) * d_f + \
                5 / 12 * (h ** 3) * d2_f + \
                3 / 8 * (h ** 4) * d3_f

            ys.append(y)
            fs.append(func(x, y))
            xs.append(x)

            x += d

        res.append((xs, ys))

    return res


func_adams = lambda x, y: 2 * x + y

h_adams = 0.1

rng_adams = (0, 1)

begs_adams = (1, 0.9145, 0.8562, 0.8225)

res = adams(func_adams, begs_adams, h_adams, rng_adams)

print('''Решить дифференциальное уравнение методом Адамса:
y'=2*x+y
y(0)=1; h=0,1; 0<=x<=1; нач. отр. [1; 0,9145; 0,8562; 0,8225]''')
for r in res:
    for x, y in zip(*res[0]):
        print('x = {}\ty = {}'.format(round(x, 6), round(y, 6)))
    print()
input()
