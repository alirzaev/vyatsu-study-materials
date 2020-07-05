#!/usr/bin/python3
from pprint import pprint


def gauss(a, b):
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


def compute(cs, xs):
    return sum(c * x for (c, x) in zip(cs, xs))


FUNC_A = [1, 9, 5, 3, 4, 14]
FUNC_B = 0

LIMITS_A = [
    [1, 0, 0, 1, 0, 0],
    [0, 1, 0, 0, 1, 0],
    [0, 0, 1, 0, 0, 1],
    [0, 0, 0, 1, 1, 1]
]
LIMITS_B = [20, 50, 30, 60]

CANONICAL_A = [
    [1, 1, 1, 0, 0, 0],
    [1, 0, 0, 1, 0, 0],
    [-1, 0, -1, 0, 1, 0],
    [0, 0, 1, 0, 0, 1]
]
CANONICAL_B = [40, 20, 10, 30]

CANONICAL_FUNC_A = [7, 0, 14, 0, 0, 0]
CANONICAL_FUNC_B = 880

table = [[CANONICAL_FUNC_B] + CANONICAL_FUNC_A] + [[b] + a for (a, b) in zip(CANONICAL_A, CANONICAL_B)]
base = [i for i in range(1, len(table[0])) if table[0][i] == 0]

while not all(v <= 0 for v in table[0][1:]):
    column = table[0][1:].index(max(table[0][1:])) + 1
    _, row = min(
        (table[i][0] / table[i][column], i) for i in range(1, len(table)) if table[i][column] > 0
    )

    val = table[row][column]
    for i in range(0, len(table[row])):
        table[row][i] /= val

    for i in range(0, len(table)):
        if i == row:
            continue
        c = table[i][column]
        for j in range(0, len(table[i])):
            table[i][j] -= c * table[row][j]

    base[row - 1] = column

pprint(base)
pprint(table)
xs = [0] * len(FUNC_A)
for i, v in enumerate(base):
    xs[v - 1] = table[i + 1][0]
pprint(xs)
pprint(compute(FUNC_A, xs))

