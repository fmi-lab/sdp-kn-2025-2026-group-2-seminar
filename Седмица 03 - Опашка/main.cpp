#include "queue_with_stacks.hpp"
#include <algorithm>
#include <iostream>
#include <iterator>
#include <optional>
#include <queue>
#include <vector>

std::vector<std::string> generate_binary_numbers(unsigned n) {
  std::vector<std::string> result;
  QueueWithStacks<std::string> queue;
  queue.enqueue("1");

  for (unsigned i = 0; i < n; ++i) {
    std::string current = queue.first();
    queue.dequeue();

    queue.enqueue(current + "0");
    queue.enqueue(current + "1");

    result.push_back(current);
  }

  return result;
}

struct HammingQueue {
  std::queue<unsigned> queue;
  unsigned number;
};

unsigned extract_min(const HammingQueue* begin, const HammingQueue* end) {
  unsigned min = begin->queue.front();

  do {
    min = std::min(min, begin->queue.front());
    ++begin;
  }
  while (begin != end);

  return min;
}

std::vector<unsigned> generate_hamming_numbers(unsigned n) {
  std::vector<unsigned> result;
  HammingQueue queues[] = {
    {std::queue<unsigned>(), 2},
    {std::queue<unsigned>(), 3},
    {std::queue<unsigned>(), 5},
  };

  for (HammingQueue& h : queues) {
    h.queue.push(1);
  }

  for (unsigned i = 0; i < n; ++i) {
    unsigned min = extract_min(std::cbegin(queues), std::cend(queues));

    // unsigned min = std::min_element(
    //   std::begin(queues), 
    //   std::end(queues), 
    //   [](const HammingQueue& lhs, const HammingQueue& rhs){
    //     return lhs.queue.front() < rhs.queue.front();
    //   }
    // )->queue.front();

    result.push_back(min);

    for (HammingQueue& h : queues) {
      if (h.queue.front() == min) {
        h.queue.pop();
      }
      h.queue.push(min * h.number);
    }
  }

  return result;
}

struct Point {
  int x, y;
};

struct PositionWithDistance : Point {
  unsigned distance;
};

constexpr Point steps[] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};

bool is_valid(
  const std::vector<std::vector<bool>>& matrix,
  const Point& current,
  const Point& step
) {
  int x = current.x + step.x, y = current.y + step.y;

  return x >= 0 && x < matrix.size() && 
         y >= 0 && y < matrix.size() &&
         !matrix[x][y];
}

std::optional<unsigned> find_shortest_path(
  std::vector<std::vector<bool>>& matrix,
  const Point& start,
  const Point& end
) {
  std::queue<PositionWithDistance> queue;
  queue.push({start.x, start.y, 0});

  while (!queue.empty()) {
    PositionWithDistance current = queue.front();
    queue.pop();

    matrix[current.x][current.y] = true;

    for (const Point& step : steps) {
      if (current.x + step.x == end.x && current.y + step.y == end.y) {
        return current.distance + 1;
      }

      if(is_valid(matrix, current, step)) {
        queue.push({ current.x + step.x, current.y + step.y, current.distance + 1 });
      }
    }
  }

  return std::nullopt;
}

std::vector<int> find_first_negative(const std::vector<int>& arr, std::size_t n) {
  std::vector<int> result;
  std::queue<std::size_t> window;

  // seed first n-1 indices
  for (std::size_t i = 0; i < n - 1; ++i) {
    window.push(i);
  }

  for (std::size_t i = n-1; i < arr.size(); ++i) {
    window.push(i);
    
    // too many elements in the queue
    if (i - window.front() == n) {
      window.pop();
    }
    
    // move until the first index points to negative or empy
    while (!window.empty() && arr[window.front()] >= 0) {
      window.pop();
    }
    
    // save 0 - all are positive
    if (window.empty()) {
      result.push_back(0);
    } else {
      result.push_back(arr[window.front()]);
    } 
  }

  return result;
}

int main() {
  for (const std::string& binary : generate_binary_numbers(6)) {
    std::cout << binary << ' ';
  }
  std::cout << '\n';

  for (unsigned n : generate_hamming_numbers(15)) {
    std::cout << n << ' ';
  }
  std::cout << '\n';

  std::vector<std::vector<bool>> board = {
    {0, 1, 0, 0, 0},
    {0, 0, 0, 1, 0},
    {0, 1, 1, 0, 0},
    {0, 1, 0, 0, 1},
    {0, 0, 0, 0, 0}
  };
  std::optional<unsigned> result = find_shortest_path(board, {0, 0}, {4, 4});
  if (result.has_value()) {
    std::cout << result.value() << '\n';
  } else {
    std::cout << "No path\n";
  }
  return 0;
}