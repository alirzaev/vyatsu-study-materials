# Лабораторная работа №8 по дисциплине "Высокопроизводительные вычислительные комплексы"

Задача: [раскраска графа](https://ru.wikipedia.org/wiki/%D0%A0%D0%B0%D1%81%D0%BA%D1%80%D0%B0%D1%81%D0%BA%D0%B0_%D0%B3%D1%80%D0%B0%D1%84%D0%BE%D0%B2)
c помощью [жадного алгоритма](https://ru.wikipedia.org/wiki/%D0%A0%D0%B0%D1%81%D0%BA%D1%80%D0%B0%D1%81%D0%BA%D0%B0_%D0%B3%D1%80%D0%B0%D1%84%D0%BE%D0%B2#%D0%96%D0%B0%D0%B4%D0%BD%D0%B0%D1%8F_%D1%80%D0%B0%D1%81%D0%BA%D1%80%D0%B0%D1%81%D0%BA%D0%B0).

# Настройка окружения

- Ubuntu 18.04 (можно WSL)

- CMake >= 3.10

- GCC >= 4

- MPICH

Установка компонентов:

```
sudo apt install make gcc cmake libmpich-dev mpich -y
```

# Сборка

```
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release -g Unix Makefiles ../
make
```

# Запуск
```
mpirun -n <кол-во процессов> ./ColorizeGraphMPI
```
