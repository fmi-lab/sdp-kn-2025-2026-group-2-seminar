#ifndef LINKED_STACK_HPP
#define LINKED_STACK_HPP

#include "stack.hpp"
#include <utility>

template <typename T>
class LinkedStack : public Stack<T> {
public:
  LinkedStack() : top(nullptr) {}
  LinkedStack(const LinkedStack<T>& other) : top(nullptr) {
    if (other.empty()) {
      return;
    }

    top = new Node(other.top->data);
    Node *next = other.top->next;
    Node* current = top;

    while (next) {
      current = current->next = new Node(next->data);
      next = next->next;
    }
  }
  LinkedStack(LinkedStack<T>&& other)
    : top(std::exchange(other.top, nullptr)) {}
  ~LinkedStack() {
    while (!empty()) {
      pop();
    }
  }
  LinkedStack<T>& operator=(const LinkedStack<T>& other) {
    LinkedStack<T> copy(other);
    swap(copy);
    return *this;
  }
  LinkedStack<T>& operator=(LinkedStack<T>&& other) {
    LinkedStack<T> copy(std::move(other));
    swap(copy);
    return *this;
  }

  void push(const T& element) {
    top = new Node(element, top);
  }

  T pop() {
    T result = top->data;
    Node* next = top->next;

    delete top;

    top = next;
    return result;
  }

  T& peek() {
    return top->data;
  }

  const T& peek() const {
    return top->data;
  }

  bool empty() const {
    return !top;
  }

private:
  struct Node {
    T data;
    Node* next;

    Node(const T& data, Node* next = nullptr) 
      : data(data), next(next) {}
  };

  Node* top;

  void swap(LinkedStack<T>& other) {
    std::swap(top, other.top);
  }
};

#endif