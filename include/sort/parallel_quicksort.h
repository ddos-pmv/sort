#pragma once

#include <algorithm>
#include <functional>
#include <future>
#include <iterator>
#include <thread>

namespace sort {

namespace config {
constexpr size_t MIN_PARALLEL_SIZE = 10000;
constexpr size_t INSERTION_SORT_THRESHOLD = 64;
}  // namespace config

template <typename Iterator, typename Compare>
void insertion_sort(Iterator first, Iterator last, Compare comp) {
  for (auto it = first; it != last; ++it) {
    auto key = std::move(*it);
    auto j = it;

    while (j != first && comp(key, *(j - 1))) {
      *j = std::move(*(j - 1));
      --j;
    }
    *j = std::move(key);
  }
}

template <typename Iterator, typename Compare>
Iterator quick_partition(Iterator first, Iterator last, Compare comp) {
  auto pivot = *std::prev(last);
  auto i = first;

  for (auto j = first; j != std::prev(last); ++j) {
    if (comp(*j, pivot)) {
      std::iter_swap(i, j);
      ++i;
    }
  }
  std::iter_swap(i, std::prev(last));
  return i;
}

template <typename Iterator, typename Compare>
void parallel_quicksort(
    Iterator first, Iterator last, Compare comp,
    size_t max_threads = std::thread::hardware_concurrency()) {
  auto size = std::distance(first, last);

  if (size <= 1) return;

  if (size < config::INSERTION_SORT_THRESHOLD) {
    insertion_sort(first, last, comp);
    return;
  }

  auto pivot = quick_partition(first, last, comp);

  if (size >= config::MIN_PARALLEL_SIZE && max_threads > 1) {
    auto future = std::async(std::launch::async, [=]() {
      parallel_quicksort(first, pivot, comp, max_threads / 2);
    });

    parallel_quicksort(pivot + 1, last, comp, max_threads / 2);
    future.wait();
  } else {
    parallel_quicksort(first, pivot, comp, 1);
    parallel_quicksort(pivot + 1, last, comp, 1);
  }
}

template <typename Iterator>
void parallel_quicksort(Iterator first, Iterator last) {
  using value_type = typename std::iterator_traits<Iterator>::value_type;
  parallel_quicksort(first, last, std::less<value_type>{});
}

template <typename Container, typename Compare>
void parallel_quicksort(Container& container, Compare comp) {
  parallel_quicksort(std::begin(container), std::end(container), comp);
}

template <typename Container>
void parallel_quicksort(Container& container) {
  parallel_quicksort(container.begin(), container.end());
}

// Специализация для C-массивов
template <typename T, size_t N, typename Compare>
void parallel_quicksort(T (&array)[N], Compare comp) {
  parallel_quicksort(std::begin(array), std::end(array), comp);
}

template <typename T, size_t N>
void parallel_quicksort(T (&array)[N]) {
  parallel_quicksort(std::begin(array), std::end(array));
}

}  // namespace sort