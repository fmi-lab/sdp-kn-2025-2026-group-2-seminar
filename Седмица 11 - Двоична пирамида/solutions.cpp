#include <array>
#include <cstddef>
#include <functional>
#include <iostream>
#include <vector>

template <typename T>
class BinaryHeap {
public:
  using Comparator = std::function<bool(const T& a, const T& b)>;

  BinaryHeap(const Comparator& comparator) : comparator(comparator) {}

  BinaryHeap(const std::vector<T>& data, const Comparator& comparator)
    : data(data), comparator(comparator) {
    for (int i = data.size() / 2; i >= 0; --i) {
      heapify_down(i);
    }
  }

  bool empty() const {
    return data.empty();
  }

  std::size_t size() const {
    return data.size();
  }

  const T& top() const {
    return data.front();
  }

  void insert(const T& element) {
    data.push_back(element);
    heapify_up(data.size() - 1);
  }

  T pop() {
    T result = top();
    data[0] = data.back();
    data.pop_back();

    heapify_down(0);

    return result;
  }

private:
  std::vector<T> data;
  Comparator comparator;

  std::size_t left_child(std::size_t parent_index) const {
    return 2 * parent_index + 1;
  }

  std::size_t right_child(std::size_t parent_index) const {
    return 2 * parent_index + 2;
  }

  int parent(int child_index) const {
    return (child_index - 1) / 2;
  }

  void heapify_up(std::size_t index) {
    std::size_t parent_index = parent(index);

    while (index && comparator(data[index], data[parent_index])) {
      std::swap(data[index], data[parent_index]);
      index = parent_index;
      parent_index = parent(index);
    }
  }

  void heapify_down(std::size_t index) {
    std::size_t smallest_index = index;
    std::size_t left_child_index = left_child(index);
    std::size_t right_child_index = right_child(index);

    if (left_child_index < data.size() && comparator(data[left_child_index], data[smallest_index])) {
      smallest_index = left_child_index;
    }

    if (right_child_index < data.size() && comparator(data[right_child_index], data[smallest_index])) {
      smallest_index = right_child_index;
    }

    if (smallest_index == index) {
      return;
    }

    std::swap(data[index], data[smallest_index]);

    heapify_down(smallest_index);
  }
};

std::vector<double> medians(const std::vector<double>& arr) {
  BinaryHeap<double> min_heap(std::less<double>{});
  BinaryHeap<double> max_heap(std::greater<double>{});
  
  std::vector<double> result;

  for (double n : arr) {
    if (max_heap.empty() || n < max_heap.top()) {
      max_heap.insert(n);
    } else {
      min_heap.insert(n);
    }

    if (min_heap.size() > max_heap.size()) {
      max_heap.insert(min_heap.pop());
    }

    if (max_heap.size() > min_heap.size() + 1) {
      min_heap.insert(max_heap.pop());
    }

    if(max_heap.size() == min_heap.size()) {
      result.push_back((max_heap.top() + min_heap.top()) / 2);
    } else {
      result.push_back(max_heap.top());
    }
  }

  return result;
}

template <typename T>
void heap_sort(std::vector<T>& data) {
  BinaryHeap<int> heap(data, std::less<T>{});

  std::size_t i = 0;
  while (!heap.empty()) {
    data[i++] = heap.pop();
  }
}

int main() {
  std::vector<int> arr = {19, 1, 17, 100, 2, 7, 3, 36, 25};
  BinaryHeap<int> heap(arr, std::less<int>{});

  // while (!heap.empty()) {
  //   std::cout << heap.pop() << ' ';
  // }
  // std::cout << '\n';

  heap_sort(arr);
  for (int i : arr) {
    std::cout << i << ' ';
  }
  std::cout << '\n';

  for (double n : medians({19, 1, 17, 100, 2, 7, 3, 36, 25})) {
    std::cout << n << ' ';
  }
  std::cout << '\n';

  // -> 19 10 17 18 17 12 7 12 17

  return 0;
}