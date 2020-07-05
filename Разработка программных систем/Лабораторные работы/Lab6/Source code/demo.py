#!/usr/bin/python3
from typing import Optional
from primerandom import get_random_number, get_random_array


def read_int(min_value: int, max_value: int) -> Optional[int]:
    try:
        number = int(input())
        if min_value <= number <= max_value:
            return number
        else:
            return None
    except ValueError:
        return None


if __name__ == '__main__':
    while True:
        print('Выберите действие:')
        print('1. Выдать случайное простое число')
        print('2. Выдать массив случаных чисел')
        print('3. Выйти')

        op = read_int(1, 3)

        if op is None:
            print('Неизвестная команда')
            continue

        if op == 1:
            print(get_random_number())
        elif op == 2:
            print('Введите длину массива:')
            while True:
                ln = read_int(1, 1000000)
                if ln is None:
                    print('Длина массива должна быть натуральным числом в диапазоне (0; 1000000]')
                else:
                    print('\n'.join(str(v) for v in get_random_array(ln)))
                    break
        elif op == 3:
            break
