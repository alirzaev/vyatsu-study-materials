from math import *


def simple_iterations(a, b, eps):
    x0 = b[:]
    x1 = b[:]
    n = len(b)

    while True:
        for i in range(n):
            x1[i] = b[i]
            for j in range(n):
                x1[i] += x0[j] * a[i][j]

        if all((abs(v2 - v1) <= eps for v2, v1 in zip(x1, x0))):
            break
        else:
            x0 = x1[:]

    check = [0] * n

    for i in range(n):
        check[i] = b[i]
        for j in range(n):
            if i == j:
                check[i] += x0[j] * (a[i][j] - 1)
            else:
                check[i] += x0[j] * a[i][j]

    return [round(x, int(-log10(eps))) for x in x0], all((abs(c) <= eps for c in check))


def reverse_matrix(a, b, eps):
    def det(m):
        return (m[0][0] * (m[1][1] * m[2][2] - m[2][1] * m[1][2]) -
                m[0][1] * (m[1][0] * m[2][2] - m[2][0] * m[1][2]) +
                m[0][2] * (m[1][0] * m[2][1] - m[2][0] * m[1][1]))

    n = len(b)
    d0 = det(a)
    d = [0] * n

    for i in range(n):
        for j in range(n):
            a[j][i], b[j] = b[j], a[j][i]
        d[i] = det(a)
        for j in range(n):
            a[j][i], b[j] = b[j], a[j][i]

    return ([round(di / d0, int(-log10(eps))) for di in d], True) if abs(d0) > eps else ([-1] * n, False)


def gauss(a, b, eps):
    n = len(b)
    for i in range(n):
        z = i
        while z < n and a[z][i] == 0:
            z += 1
        a[i], a[z] = a[z], a[i]

        val = a[i][i]
        for j in range(n):
            a[i][j] /= val
        b[i] /= val

        for j in range(i + 1, n):
            val = a[j][i]
            for k in range(i, n):
                a[j][k] -= a[i][k] * val
            b[j] -= b[i] * val

    x = [0] * n
    for i in range(n - 1, -1, -1):
        x[i] = b[i]
        for j in range(i + 1, n):
            x[i] -= a[i][j] * x[j]

    return [round(i, int(-log10(eps))) for i in x], True


def newton(funcs, eps, s):
    f1, f2, f1x, f1y, f2x, f2y = funcs
    x0, y0 = s

    while True:
        a = [
            [f1x(x0, y0), f1y(x0, y0)],
            [f2x(x0, y0), f2y(x0, y0)]
        ]

        b = [-f1(x0, y0), -f2(x0, y0)]

        d = (a[0][0] * a[1][1]) - (a[0][1] * a[1][0])

        dx = (b[0] * a[1][1] - b[1] * a[0][1]) / d
        dy = (a[0][0] * b[1] - b[0] * a[1][0]) / d

        if abs(dx) <= eps and abs(dy) <= eps:
            break
        else:
            x0 += dx
            y0 += dy

    return [round(i, int(-log10(eps))) for i in [x0, y0]], True


A = [
    [0.14,  0.23, 0.18,  0.17],
    [0.12, -0.14, 0.08,  0.09],
    [0.16,  0.24, 0.00, -0.35],
    [0.23, -0.08, 0.55,  0.25]
]

B = [-1.42, -0.83, 1.21, 0.05]

print('Решение второй системы (метод простых итераций)')
print(simple_iterations(A, B, 0.0001)[0])

A = [
    [2, 2, 3],
    [1, 1, 3],
    [2, 1, 2]
]

B = [1, 2, -1]

print('Решение третьей системы (метод обратных матриц)')
print(reverse_matrix(A, B, 0.001)[0])

A = [
    [1.00,  0.55, -0.13,  0.34],
    [0.13, -0.17,  0.33,  0.17],
    [0.11,  0.18, -0.22, -0.11],
    [0.13, -0.12,  0.21,  0.22]
]

B = [0.13, 0.11, 1.00, 0.18]

print('Решение первой системы (метод Гаусса)')
print(gauss(A, B, 0.001)[0])

functions = [
    lambda x, y: x - exp(y - 0.4) - 3.5,  # f1
    lambda x, y: y - sin(x),  # f2
    lambda x, y: 1,  # f1x
    lambda x, y: -exp(y - 0.4),  # f1y
    lambda x, y: -cos(x),  # f2x
    lambda x, y: 1  # f2y
]

s = [3.5, -0.4]

print('Решение четвертой системы (метод Ньютона для нелинейных систем)')
print(newton(functions, 0.0001, s)[0])
input()
