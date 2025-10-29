#ifndef LINKED_LIST_HPP
#define LINKED_LIST_HPP

#include <cstddef>
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
  
  // WRITE YOUR PUBLIC METHODS HERE
  
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
  
  // WRITE YOUR PRIVATE METHODS HERE
  
  // ===========================================================
};

#endif