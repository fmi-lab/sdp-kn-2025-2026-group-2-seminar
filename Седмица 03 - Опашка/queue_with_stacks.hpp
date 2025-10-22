#ifndef QUEUE_WITH_STACKS_HPP
#define QUEUE_WITH_STACKS_HPP

#include "queue.hpp"
#include <stack>

template <typename T>
class QueueWithStacks : public Queue<T> {
public:
  void enqueue(const T& element) {
    if (empty()) {
      bottom.push(element);
    } else {
      top.push(element);
    }
  }

  void dequeue() {
    bottom.pop();
    if (bottom.empty()) {
      move_to_bottom();
    }
  }

  T& first() {
    return bottom.top();
  }

  const T& first() const {
    return bottom.top();
  }

  bool empty() const {
    return bottom.empty() && top.empty();
  }

private:
  std::stack<T> bottom, top;

  void move_to_bottom() {
    while (!top.empty()) {
      bottom.push(top.top());
      top.pop();
    }
  }
};

#endif