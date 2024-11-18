#include <iostream>
#include <memory>
#include <vector>
#include <thread>
#include <cstdlib>
#include <ctime>
#include <numeric> // Для std::accumulate

void partial_sum(const int *array, int start, int end, int &result,int part_number)
{
    result = std::accumulate(array + start, array + end, 0);
    std::cout << "Sum of part " << part_number + 1 << ": " << result << std::endl; // Вывод суммы каждой части
}

int main()
{
    // Инициализация генератора случайных чисел
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    // Ввод размера массива и количества потоков
    int N, M;
    std::cout << "Enter the size of the array (N): ";
    std::cin >> N;
    std::cout << "Enter the number of parts (M): ";
    std::cin >> M;

    if (M <= 0 || M > N)
    {
        std::cerr << "M must be greater than 0 and less than or equal to N." << std::endl;
        return 1;
    }

    // Создаем динамический массив
    std::shared_ptr<int[]> array(new int[N]);

    // Заполнение массива случайными числами
    for (int i = 0; i < N; ++i)
    {
        array[i] = std::rand() % 100; // Случайные числа от 0 до 99
    }

    // Вывод значений массива
    std::cout << "Array: ";
    for (int i = 0; i < N; ++i)
    {
        std::cout << array[i] << " ";
    }
    std::cout << std::endl;

    // Определяем размер каждой части
    int part_size = N / M;
    std::vector<std::thread> threads;
    std::vector<int> partial_results(M);

    // Запускаем потоки для вычисления суммы
    for (int i = 0; i < M; ++i)
    {
        int start = i * part_size;
        int end = (i == M - 1) ? N : start + part_size; // Обрабатываем оставшиеся элементы в последнем потоке
        threads.emplace_back(partial_sum, array.get(), start, end, std::ref(partial_results[i]), i);
    }

    // Ожидаем завершения всех потоков
    for (auto &th : threads)
    {
        th.join();
    }

    // Вычисляем общую сумму
    int total_sum = std::accumulate(partial_results.begin(), partial_results.end(), 0);

    std::cout << "Total sum: " << total_sum << std::endl;

    return 0;
}