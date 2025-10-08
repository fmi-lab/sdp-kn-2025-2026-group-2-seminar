#include <algorithm>
#include <iostream>
#include <iterator>

template <typename T>
void insertion_sort(T* begin, T* end) {
  T* iter = begin + 1;

  while (iter != end) {
    T* prev = iter - 1;
    T value = *iter;

    while (prev >= begin && *prev > value) {
      std::iter_swap(prev, prev + 1);
      --prev;
    }

    ++iter;
  }
}

template <typename T>
T* partition(T* begin, T* end, const T& pivot) {
  while (begin != end) {
    if (*begin < pivot) {
      ++begin;
    } else {
      std::iter_swap(begin, end - 1);
      --end;
    }
  }

  return begin;
}

template <typename T>
void quick_sort(T* begin, T* end) {
  if (end - begin <= 1) {
    return;
  }

  T pivot = *begin;
  T* middle = partition(begin + 1, end, pivot);
  std::iter_swap(begin, middle - 1);
  quick_sort(begin, middle - 1);
  quick_sort(middle, end);
}

template <typename T>
void print(const T* begin, const T* end) {
  while (begin != end) {
    std::cout << *begin++ << ' ';
  }
  std::cout << '\n';
}

int main() {
  int arr[] = {5, 3, 7, 1, 8, 9, 4, 6};
  quick_sort(std::begin(arr), std::end(arr));
  print(std::begin(arr), std::end(arr));
  return 0;
}