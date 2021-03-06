# Курсовая работа "Распараллеливание расчёта матрицы вещественных чисел"

## Постановка задачи

- Программа, которая рассчитывает матрицу вещественных чисел и сохраняет её в файл.
- Предназначена для ПК

Размер задач

| Размер задачи (N) | Размер матрицы (NxN), МБ |
| ----------------- | ------------------------ |
| 1024              | 8                        |
| 2048              | 32                       |
| 4096              | 128                      |
| 8192              | 512                      |
| 16384             | 2048                     |
| 32768             | 8192                     |

Существующий однопоточный алгоритм:

- расчёт строчки матрицы
- сохранение её в файл

Особенности:

- расчёт одной строчки не зависит от расчёта других

- расчёт медленнее (в среднем) сохранения

  

**Задачка для курсовой**: распараллелить существующий алгоритм.

















## Предложенный многопоточный алгоритм

- Набор задач (задача -- расчёт строчки матрицы)
- Создаём пул потоков для расчёта матрицы, каждый поток берёт задачу, обрабатывает её и результат (`vector<double>`)  складывает в контейнер.
- Есть сохраняющий поток, который извлекает результат расчёта из контейнера и сохраняет его в файл.































## Кажущиеся проблемы:

- Расчётных задач много и они маленькие -- это может приводить к ненужной дополнительной нагрузке в ```thread_pool```.
- Какой выбрать контейнер, куда складывать результаты расчётов, кажется, что очередь подходит плохо.
- Как получить ответ на главный вопрос многопоточного программирования: сколько брать thread'ов? hyperthreading? 4 cores -- 3 расчётных потока (+1 на сохранение) или 4?



[1, 3, 2, 4] ->  1, 2, 3, 4

[1, 2, 4] -> 1, 2, _, 4 -> [4]



























## Архитектура

https://github.com/gwqw/OTUSCppCourseWork

```c++
SimpleTaskCalculator
    CalcResult taskCalculation(std::size_t task_num)
    std::size_t getTasksNumber() const
    
CalcTaskMgr
    CalcTaskMgr(TaskCalculatorHolder task_generator)
    void run()
    void subscribe(SubscriberHolder subscriber)
    
ResultSaver
    void update(ResultHolder calc_result)
    
CalcResult
    std::size_t task_num
    std::vector<double> line
```



[[Код]]


```
[0 0 3 4 5 0 7]
     ^     ^   
```











## Сколько потоков?

**Время расчёта**

| task size | threads = 1, s | threads = 2, s | threads = 3, s | threads = 4, s |
| -- | -- | -- | -- | -- |
| 1024 | 0.459 | 0.235 | 0.162 | 0.132 |
| 2048 | 1.801 | 0.917 | 0.617 | 0.49 |
| 4096 | 7.19 | 3.64 | 2.45 | 1.94 |
| 8192 | 28.05 | 14.28 | 9.68 | 7.77 |
| 16384 | 112.5 | 57.7 | 38.5 | 31 |

```
t_c = a/threads * N^2 
a = 4.34e-7
```

**Время сохранения**

| task size | t_save, s |
| --------- | --------- |
| 8192      | 0.49      |

```
t_s = b * N^2 
b = 6e-8
```



**Когда последний поток лишний?**

```
t_c / N ~ t_s / N
b ~ a / threads
threads ~ a/b ~ 7
```



**Крайние случаи**

https://www.cpubenchmark.net/singleThread.html

| CPU                         | parrots | max_threads |
| --------------------------- | ------- | ----------- |
| Intel Core i5-2300 @ 2.8GHz | 1575    | 7           |
| AMD Ryzen 5 3600            | 2804    | ~4          |
| AMD Ryzen 7 PRO 3700        | 3027    | ~4          |

















## Память

```
dt = t_s - t_c
t_s1 = t_s / N
N_str ~ dt / t_s1 = (1 - (a/b) / threads)*N
a/b = 7 -- мой комп
```





**Крайние случаи**

```
32k -> 8Gb
a/b = 4
threads = 6
=> N_str ~ N / 3
2.7 Gb (измерено: 2.3 Gb и 1.9 Gb)
```





































## Follow up

- object pool



























