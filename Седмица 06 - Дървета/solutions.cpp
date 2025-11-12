#include <iostream>
#include <optional>
#include "binary_tree.hpp"
#include "tree.hpp"

int main() {
  Tree<int> tree(1);
  Tree<int> first_subtree(2, {Tree<int>(5), Tree<int>(6, {Tree<int>(11)})});
  Tree<int> second_subtree(3, {Tree<int>(7)});
  Tree<int> third_subtree(4, {Tree<int>(8), Tree<int>(9), Tree<int>(10)});

  tree.add_subtree(first_subtree);
  tree.add_subtree(second_subtree);
  tree.add_subtree(third_subtree);

  tree.dfs();
  std::cout << '\n';

  tree.bfs();
  std::cout << '\n';

  std::cout << std::boolalpha << tree.contains(6) << '\n';
  std::cout << tree.contains(7) << '\n';
  std::cout << tree.contains(12) << '\n';

  BinaryTree<int> bintree(
    1, 
    BinaryTree<int>(2, 
                    BinaryTree<int>(4), 
                    BinaryTree<int>(5, 
                                    BinaryTree<int>(8))), 
    BinaryTree<int>(3, 
                    BinaryTree<int>(6, 
                                    BinaryTree<int>(9), 
                                    BinaryTree<int>(10)), 
                    BinaryTree<int>(7, 
                                    BinaryTree<int>(), 
                                    BinaryTree<int>(11))));

  BinaryTree<int> copy(bintree);

  std::cout << (bintree == copy) << '\n';
  std::cout << (bintree == BinaryTree<int>()) << '\n';
  std::cout << (bintree == BinaryTree<int>(1, BinaryTree<int>(2))) << '\n';

  for (const std::vector<int>& path : bintree.paths()) {
    for (int i : path) {
      std::cout << i << ' ';
    }

    std::cout << '\n';
  }

  for (int i : bintree) {
    std::cout << i << ' ';
  }
  std::cout << '\n';

  std::optional<int> found1 = bintree.lowest_common_ancestor(5, 10);
  std::optional<int> found2 = bintree.lowest_common_ancestor(7, 9);
  std::optional<int> found3 = bintree.lowest_common_ancestor(7, 11);
  std::optional<int> not_found = bintree.lowest_common_ancestor(13, 12);

  if (found1.has_value()) {
    std::cout << found1.value() << '\n'; // -> 5
  }
  if (found2.has_value()) {
    std::cout << found2.value() << '\n'; // -> 8
  }
  if (found3.has_value()) {
    std::cout << found3.value() << '\n'; // -> 8
  }
  if (!not_found.has_value()) {
    std::cout << "not found\n";
  } else {
    std::cout << not_found.value() << '\n';
  }

  return 0;
}