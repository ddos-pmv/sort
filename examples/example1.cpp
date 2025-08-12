#include <sort/parallel_quicksort.h>

#include <iostream>

int main() {
  std::vector<int> data = {5, 3, 8, 1, 2, 7, 4, 6};

  sort::parallel_quicksort(data);

  std::cout << "Отсортированный вектор: ";
  for (int x : data) std::cout << x << " ";
  std::cout << "\n";

  float numbers[] = {3.2f, 1.5f, 4.8f, 2.1f};
  sort::parallel_quicksort(numbers, std::greater<float>());

  std::cout << "Отсортированный массив: ";
  for (float x : numbers) std::cout << x << " ";
  std::cout << "\n";

  return 0;
}