#include <cstddef>
#include <iostream>
#include <string>
#include <utility>

template <typename T>
class CircularLinkedList {
public:
  CircularLinkedList() : last(nullptr), size(0) {}
  CircularLinkedList(const CircularLinkedList<T>& other)
    : last(nullptr), size(0) {
    if (other.empty()) {
      return;
    }

    Node *current = other.last->next, *other_first = current;
    do {
      insert_last(current->data);
      current = current->next;
    } while (current != other_first);
  }
  CircularLinkedList(CircularLinkedList<T>&& other)
    : last(std::exchange(other.last, nullptr)),
      size(std::exchange(other.size, 0)) {}
  ~CircularLinkedList() {
    while (!empty()) {
      remove_first();
    }
  }
  CircularLinkedList<T>& operator=(const CircularLinkedList<T>& other) {
    CircularLinkedList<T> copy(other);
    swap(copy);
    return *this;
  }
  CircularLinkedList<T>& operator=(CircularLinkedList<T>&& other) {
    CircularLinkedList<T> copy(std::move(other));
    swap(copy);
    return *this;
  }

  bool empty() const {
    return !last;
  }

  std::size_t get_size() const {
    return size;
  }

  void insert_first(const T& data) {
    if (empty()) {
      last = new Node(data);
      last->next = last;
    } else {
      last->next = new Node(data, last->next);
    }

    ++size;
  }

  void insert_last(const T& data) {
    insert_first(data);
    advance_first();
  }

  void remove_first() {
    if (last == last->next) {
      delete last;
      last = nullptr;
    } else {
      Node* next = last->next->next;
      delete last->next;
      last->next = next; 
    }

    --size;
  }

  void remove_last() {
    for (int i = 0; i < size - 1; ++i) {
      advance_first();
    }
    remove_first();
  }

  void advance_first() {
    last = last->next;
  }

  const T& first() const {
    return last->next->data;
  }

  void reverse() {
    if (empty()) return;

    Node *current = last->next, *prev = last, *next;

    advance_first();
    do {
      next = current->next;
      current->next = prev;
      prev = current;
      current = next;
    } while (current != last);
  }

  std::pair<CircularLinkedList<T>, CircularLinkedList<T>> split() const {
    CircularLinkedList<T> first, second;

    Node *fast = last->next, *slow = fast;

    // while (fast->next != last && fast != last) {
    //   slow = slow->next;
    //   fast = fast->next->next;
    // }
    
    do {
      slow = slow->next;
      fast = fast->next;
      if (fast != last->next) {
        fast = fast->next;
      }
    } while (fast != last->next);

    Node* current = last->next;
    while (current != slow) {
      first.insert_last(current->data);
      current = current->next;
    }

    while (slow != last->next) {
      second.insert_last(slow->data);
      slow = slow->next;
    }

    return std::make_pair(first, second);
  }

  void print() const {
    if (!last) return;

    Node* current = last->next;

    do {
      std::cout << current->data << ' ';
      current = current->next;
    }
    while (current != last->next);
    std::cout << '\n';
  }
private:
  struct Node {
    T data;
    Node* next;

    Node(const T& data, Node* next = nullptr) 
      : data(data), next(next) {}
  };

  Node* last;
  std::size_t size;

  void swap(CircularLinkedList<T>& other) {
    using std::swap;
    swap(last, other.last);
    swap(size, other.size);
  }
};

int main() {
  CircularLinkedList<std::string> circle;
  for (const std::string& name : {"Gosho", "Pesho", "Tosho", "Ivan", "Dragan", "Petkan", "Asen"}) {
    circle.insert_last(name);
  }
  CircularLinkedList<std::string> circle2(circle);

  for (std::size_t i = 0; i < circle.get_size() - 1; ++i) {
    for (std::size_t j = 0; j < 2; ++j) {
      circle.advance_first();
    }
    circle.remove_first();
  }
  std::cout << circle.first() << '\n';

  circle2.reverse();
  circle2.print();

  auto [first, second] = circle2.split();
  first.print();
  second.print();
  return 0;
}