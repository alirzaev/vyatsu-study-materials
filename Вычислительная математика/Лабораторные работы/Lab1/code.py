def func(x):
    from math import sin
    return sin(x / 2) + 1 - x ** 2


def derivation1(x):
    from math import cos
    return 0.5 * cos(x / 2) - 2 * x


def derivation2(x):
    from math import sin
    return 0.25 * sin(x / 2) - 2


def applicator(left, right, eps, functor, applicable):
    res = abs(functor(left))
    while left < right:
        left += eps / 2
        res = applicable(res, abs(functor(left)))

    return res


def iterative_method(left, right, eps, functor, deriv1):
    sign = lambda x: 1 if x >= 0 else -1
    ceil = lambda x: x if int(x) == x else int(x) + 1
    N = max(deriv1(left), deriv1(right))
    k = sign(deriv1(left)) * (ceil(N / 2) + 1)

    xn = left
    n = 0

    while True:
        n += 1

        xn1 = xn - functor(xn) / k
        y = functor(xn)
        delta = xn - xn1
        xn = xn1

        yield n, xn, xn1, y, delta

        if abs(delta) < eps:
            break


def combined_method(left, right, eps, functor, deriv1, deriv2):
    def hord1(xn, xi):
        return xi - functor(xi) * (xi - xn) / (functor(xi) - functor(xn))

    fixed = functor(left) * deriv2(left) > 0
    if fixed:
        xn, xi = left, right
    else:
        xn, xi = right, left

    n = 0

    while True:
        n += 1

        delta = xi - xn
        if fixed:
            xi = hord1(xn, xi)
            xn = xn - functor(xn) / deriv1(xn)
        else:
            xn = hord1(xi, xn)
            xi = xi - functor(xi) / deriv1(xi)

        y = functor(xn)

        yield n, xn, xi, y, delta

        if abs(delta) < eps:
            break


def isolate(acc, left, right, functor):
    while functor(left + acc) * functor(right - acc) < 0:
        left += acc
        right -= acc

    while functor(left) * functor(left + acc) > 0:
        left += acc

    while functor(right) * functor(right - acc) > 0:
        right -= acc

    return left, right


def check_isolated(left, right, eps, deriv1, deriv2):
    mx = applicator(left, right, eps, deriv2, max)
    mn = applicator(left, right, eps, deriv1, min)

    return mx < 2 * mn

func_repr = 'sin(x / 2) + 1 - x ^ 2'
deriv1_repr = '0.5 * cos(x / 2) - 2 * x'
deriv2_repr = '0.25 * sin(x / 2) - 2'


def show_iterative_method():
    print('Программа уточняет корень уравнения {} = 0'.format(func_repr))
    print('Границы диапазона: [{}, {}]'.format(1.0, 2.8))
    print('Уточнение интервала изоляции: [{:.3f}, {:.3f}]'.format(*isolate(0.01, 1.0, 2.8, func)))
    condition = check_isolated(1.0, 2.8, 0.01, derivation1, derivation2)
    print('Проверка условия M <= 2m: {}'.format('верно' if condition else 'неверно'))

    for t in iterative_method(1.0, 2.8, 0.00001, func, derivation1):
        print('{}\t{:.5f}\t{:.5f}\t{:+.6f}\t{:+.6f}'.format(*t))


def show_combined_method():
    print('Программа уточняет корень уравнения {} = 0'.format(func_repr))
    print('Границы диапазона: [{}, {}]'.format(1.0, 2.8))
    print('Уточнение интервала изоляции: [{:.3f}, {:.3f}]'.format(*isolate(0.01, 1.0, 2.8, func)))
    condition = check_isolated(1.0, 2.8, 0.01, derivation1, derivation2)
    print('Проверка условия M <= 2m: {}'.format('верно' if condition else 'неверно'))

    for t in combined_method(1.0, 2.8, 0.00001, func, derivation1, derivation2):
        print('{}\t{:.5f}\t{:.5f}\t{:+.6f}\t{:+.6f}'.format(*t))


#show_iterative_method()
show_combined_method()
input()
