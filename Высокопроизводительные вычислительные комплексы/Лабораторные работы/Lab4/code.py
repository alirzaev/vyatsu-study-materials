from math import factorial as fact
from functools import reduce


l_0 = 0.2
alphas = [50, 50, 2]
thetas = [16000, 5, 1]
ks = [1, 5000, 100000]
U_max = 20
S_max = 400000


def v_min_i(l, t):
    return l * t


def s_min_i(l, t, k):
	return l * t * k


def v_i(l, t, k):
	return l * t + ((l * t / k) ** 0.5) * \
	sum((aa * l_0 * kk * tt) ** 0.5 for (aa, kk, tt) in zip(alphas, ks, thetas)) \
	/ (l_0 * U_max)


def s_min():
	return sum(s_min_i(a * l_0, t, k) for (a, t, k) in zip(alphas, thetas, ks))

def s():
	return l_0 * sum(a * k * t for (a, k, t) in zip(alphas, ks, thetas)) \
	+ ((sum((a * k * t) ** 0.5 for (a, k, t) in zip(alphas, ks, thetas))) ** 2) / U_max


def v_i_2(a, t, k):
	s = sum((a * k * t) ** 0.5 for (a, k, t) in zip(alphas, ks, thetas))
	return l_0 * a * t + (S_max / k) * ((a * k * t) ** 0.5) / s


def u():
	s = sum((a * k * t) ** 0.5 for (a, k, t) in zip(alphas, ks, thetas)) ** 2
	return s / (S_max - s_min())

print('v_min_i')
for (a, t) in zip(alphas, thetas):
    print(v_min_i(l_0 * a, t))

print('s_min_i')
for (a, t, k) in zip(alphas, thetas, ks):
    print(s_min_i(l_0 * a, t, k))

print('v_i')
for (a, t, k) in zip(alphas, thetas, ks):
    print(v_i(l_0 * a, t, k))

print('s')
print(s())

print('s_0')
print(s() - s_min())

print('s_min')
print(s_min())

print('v_i_2')
for (a, t, k) in zip(alphas, thetas, ks):
    print(v_i_2(a, t, k))

print('s_0_2')
print(S_max - s_min())

print('u')
print(u())