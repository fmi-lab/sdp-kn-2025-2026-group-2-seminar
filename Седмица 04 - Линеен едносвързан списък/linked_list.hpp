#ifndef LINKED_LIST_HPP
#define LINKED_LIST_HPP

#include <cstddef>
#include <functional>
#include <stack>
#include <utility>

template <typename T>
class LinkedList {
public:  
  LinkedList() : first(nullptr), last(nullptr), size(0) {}
  LinkedList(const LinkedList& other)
    : first(nullptr), last(nullptr), size(0) {
    if (other.empty()) {
      return;
    }

    first = last = new Node(other.first->data);
    Node* next = other.first->next;

    while (next) {
      last = last->next = new Node(next->data);
      next = next->next;
    }
  }
  LinkedList(LinkedList&& other)
    : first(std::exchange(other.first, nullptr)),
      last(std::exchange(other.last, nullptr)),
      size(std::exchange(other.size, 0)) {}
  ~LinkedList() {
    while (!empty()) {
      remove_first();
    }
  }
  LinkedList<T>& operator=(const LinkedList& other) {
    LinkedList<T> copy(other);
    swap(copy);

    return *this;
  }
  LinkedList<T>& operator=(LinkedList&& other) {
    LinkedList<T> copy(std::move(other));
    swap(copy);

    return *this;
  }

  bool empty() const {
    return !first;
  }

  std::size_t get_size() const {
    return size;
  }

  void insert_first(const T& data) {
    if (empty()) {
      first = last = new Node(data);
    } else {
      first = new Node(data, first);
    }

    ++size;
  }

  void insert_last(const T& data) {
    if (empty()) {
      first = last = new Node(data);
    } else {
      last = last->next = new Node(data);
    }

    ++size;
  }
  void remove_first() {
    if (first == last) {
      delete first;
      first = last = nullptr;
    } else {
      Node* next = first->next;
      delete first;
      first = next;
    }

    --size;
  }
  void remove_last() {
    if (first == last) {
      delete first;
      first = last = nullptr;
    } else {
      Node* iter = first;

      while (iter->next->next) {
        iter = iter->next;
      }

      delete last;
      last = iter;
      last->next = nullptr;
    }

    --size;
  }

private:
  struct Node;

public:
  class Iterator {
  public:
    Iterator(Node* const current) : current(current) {}

    bool operator!=(const Iterator& other) const {
      return current != other.current;
    }

    bool operator==(const Iterator& other) const {
      return !(*this != other);
    }

    Iterator& operator++() {
      current = current->next;
      return *this;
    }

    const T& operator*() const {
      return current->data;
    }

    T& operator*() {
      return current->data;
    }

  private:
    friend class LinkedList<T>;

    Node* current;
  };

  Iterator begin() const {
    return Iterator(first);
  }

  Iterator end() const {
    return Iterator(nullptr);
  }

  // ===========================================================
  
  void insert_after(const T& data, const Iterator& position) {
    if (position == Iterator(last)) {
      insert_last(data);
      return;
    }
    
    Node* current = position.current;

    Node* new_node = new Node(data, current->next);
    current->next = new_node;
  }

  void remove_at(const Iterator& position) {
    if (position == begin()) {
      remove_first();
    } else if (position == Iterator(last)) {
      remove_last(); 
    } else {
      Node* current = position.current;
      Node* prev = previous(current);
      prev->next = current->next;
      delete current;
    }
  }

  void append(const LinkedList& other) {
    Node* current = other.first;
    while (current) {
      insert_last(current->data);
      current = current->next;
    }
  }

  void unique() {
    Node* current = first;

    while (current) {
      Node *prev = current, *iter = current->next;

      while (iter) {
        if (current->data == iter->data) {
          prev->next = iter->next;
          if (iter == last) {
            last = prev;
          }
          delete iter;
          iter = prev->next;
        } else {
          iter = iter->next;
          prev = prev->next;
        }
      }

      current = current->next;
    }
  }

  void filter(const std::function<bool(const T&)>& predicate) {
    while (!predicate(first->data)) {
      remove_first();
    }

    if (!first) {
      last = nullptr;
      return;
    }

    Node *current = first->next, *prev = first;
    while (current) {
      if (!predicate(current->data)) {
        prev->next = current->next;
        delete current;
        current = prev->next;
      } else {
        current = current->next;
        prev = prev->next;
      }
    }

    last = prev;
  }

  void partition(const std::function<bool(const T&)>& predicate) {
    Node *iter = first;
    Node *left_begin = nullptr, *left_end = nullptr, *right_begin = nullptr, *right_end = nullptr;

    while (iter) {
      if (predicate(iter->data)) {
        if (!left_begin) {
          left_begin = left_end = iter;
        } else {
          left_end = left_end->next = iter;
        }
      } else {
        if (!right_begin) {
          right_begin = right_end = iter;
        } else {
          right_end = right_end->next = iter;
        }
      }

      iter = iter->next;
    }

    if (left_end) left_end->next = right_begin;
    if (right_end) right_end->next = nullptr;
    first = left_begin ? left_begin : right_begin;
    last = right_end ? right_end : left_end;
  }

  class ReverseIterator {
  public:
    ReverseIterator(Node* node) {
      Node* current = node;
      while (current) {
        stack.push(current);
        current = current->next;
      }
    }

    bool operator!=(const ReverseIterator& other) const {
      if (stack.empty()) {
        return !other.stack.empty();
      }

      return other.stack.empty() || stack.top() != other.stack.top();
    }

    ReverseIterator& operator++() {
      stack.pop();
      return *this;
    }

    T& operator*() {
      return stack.top()->data;
    }

    const T& operator*() const {
      return stack.top()->data;
    }

  private:
    friend LinkedList<T>;
    std::stack<Node*> stack;
  };
  
  ReverseIterator rbegin() const {
    return ReverseIterator(first);
  }

  ReverseIterator rend() const {
    return ReverseIterator(nullptr);
  }
  
  // ===========================================================

private:
  struct Node {
    T data;
    Node* next;

    Node(const T& data, Node* const next = nullptr) 
      : data(data), next(next) {}
  };

  Node *first, *last;
  std::size_t size;

  void swap(LinkedList& other) {
    using std::swap;

    swap(first, other.first);
    swap(last, other.last);
    swap(size, other.size);
  }

  // ===========================================================
  
  Node* previous(Node* current) const {
    Node* iter = first;
    while (iter->next != current) {
      iter = iter->next;
    }

    return iter;
  }
  
  // ===========================================================
};

#endif