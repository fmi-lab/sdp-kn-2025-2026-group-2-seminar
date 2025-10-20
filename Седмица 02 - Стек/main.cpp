#include "prelimitary.hpp"
#include <iostream>
#include <vector>

template <typename T>
struct State {
  T *begin, *end;
  bool merge;

  State(T* begin, T* end, bool merge = false) : begin(begin), end(end), merge(merge) {}
};


template <typename T>
void merge(T* begin, T* middle, T* end) {
  std::vector<T> left(begin, middle), right(middle, end);
  std::size_t i = 0, j = 0, left_size = left.size(), right_size = right.size();

  while (i < left_size && j < right_size) {
    if (left[i] < right[j]) {
      *begin++ = left[i++];
    } else {
      *begin++ = right[j++];
    }
  }

  while (i < left_size) {
   *begin++ = left[i++]; 
  }

  while (j < right_size) {
   *begin++ = right[j++]; 
  }
}

template <typename T>
void merge_sort(T* begin, T* end) {
  LinkedStack<State<T>> stack;
  stack.push(State(begin, end));

  while (!stack.empty()) {
    State<T> current_state = stack.pop();

    T* middle = current_state.begin + (current_state.end - current_state.begin) / 2;
    
    if (current_state.merge) {
      merge(current_state.begin, middle, current_state.end);
    } else if (current_state.end - current_state.begin > 1) {
      stack.push(State<T>(current_state.begin, current_state.end, true));
      stack.push(State<T>(middle, current_state.end));
      stack.push(State(current_state.begin, middle));
    }
  }
}

int main() {
  int arr[] = {2, 15, 1, 30, 4, 193, 6, 255, 4, 18};
  merge_sort(std::begin(arr), std::end(arr));

  for (int element : arr) {
    std::cout << element << ' ';
  }
  std::cout << '\n';
  return 0;
}