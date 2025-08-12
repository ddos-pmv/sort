#include <sort/parallel_quicksort.h>

#include <algorithm>
#include <chrono>
#include <functional>
#include <iostream>
#include <random>
#include <vector>

std::vector<int> generate_random_data(size_t size) {
  std::vector<int> data(size);
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(1, 1000000);

  for (auto& x : data) {
    x = dis(gen);
  }
  return data;
}

template <typename SortFunc, typename Container>
double measure_sort(SortFunc sort_func, Container& data) {
  auto start = std::chrono::high_resolution_clock::now();
  sort_func(data);
  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> diff = end - start;
  return diff.count();
}

int main() {
  const size_t test_sizes[] = {10'000, 100'000, 1'000'000, 10'000'000};
  const int iterations = 5;

  std::cout << "Размер\t\tСтандартная (с)\tПараллельная (с)\tУскорение\n";
  std::cout << "-------------------------------------------------\n";

  for (size_t size : test_sizes) {
    double std_time = 0.0;
    double parallel_time = 0.0;

    for (int i = 0; i < iterations; ++i) {
      auto data = generate_random_data(size);
      auto data_copy = data;

      std_time +=
          measure_sort([](auto& v) { std::sort(v.begin(), v.end()); }, data);

      parallel_time +=
          measure_sort([](auto& v) { sort::parallel_quicksort(v); }, data_copy);

      if (!std::is_sorted(data_copy.begin(), data_copy.end())) {
        std::cerr << "Ошибка: параллельная сортировка не работает!\n";
        return 1;
      }
    }

    std_time /= iterations;
    parallel_time /= iterations;
    double speedup = std_time / parallel_time;

    std::cout << size << "\t\t" << std_time << "\t\t" << parallel_time << "\t\t"
              << (speedup >= 1.0 ? speedup : 1.0) << "x\n";
  }

  return 0;
}