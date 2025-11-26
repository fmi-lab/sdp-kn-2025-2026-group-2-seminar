#include <iostream>
#include "avl-tree.hpp"

int main() {
  AVLTree<int, char> tree;
  tree.insert(4, 'C');
  tree.insert(8, 'K');
  tree.insert(9, 'P');
  tree.insert(10, 'A');
  tree.insert(11, 'F');
  tree.insert(12, 'H');
  tree.insert(13, 'O');
  tree.insert(15, 'I');
  tree.insert(17, 'M');
  tree.insert(20, 'G');

  // tree.remove(4);
  // tree.remove(9);
  // tree.remove(20);

  // tree.remove(8);

  // tree.remove(10);

  tree.pretty_print();

  std::optional<char> closest = tree.closest_key(15);
  if (closest.has_value()) {
    std::cout << closest.value() << '\n';
  } else {
    std::cout << "missing\n";
  }

  return 0;
}