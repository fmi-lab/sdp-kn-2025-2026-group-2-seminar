#include "bst.hpp"
#include <iostream>

int main() {
  BinarySearchTree<int, char> tree;
  
  tree.insert(12, 'H');
  tree.insert(8, 'K');
  tree.insert(15, 'I');
  tree.insert(4, 'C');
  tree.insert(10, 'A');
  tree.insert(9, 'P');
  tree.insert(11, 'F');
  tree.insert(13, 'O');
  tree.insert(20, 'G');
  tree.insert(17, 'M');

  tree.remove(12);

  tree.pretty_print();

  std::optional<char> value = tree.search(12);

  if (value.has_value()) {
    std::cout << value.value() << '\n';
  } else {
    std::cout << "missing\n";
  }

  tree.insert(9, 'X');

  for (char c : tree) {
    std::cout << c << ' ';
  }
  std::cout << '\n';

  BinarySearchTree<int, char> other;
  other.insert(7, 'L');
  other.insert(14, 'J');
  other.insert(16, 'B');
  other.insert(18, 'W');
  other.insert(3, 'Z');
  other.insert(11, 'Q');
  
  other.pretty_print();

  tree.merge(other);
  tree.pretty_print();

  return 0;
}