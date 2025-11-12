#ifndef TREE_HPP
#define TREE_HPP

#include <vector>

template <typename T>
class Tree {
public:
  Tree(const T& data, const std::vector<Tree<T>>& children = std::vector<Tree<T>>())
    : data(data), children(children) {}

  void add_subtree(const Tree& subtree) {
    children.push_back(subtree);
  }

  const T& root() const {
    return data;
  }

  bool leaf() const {
    return children.empty();
  }

private:
  T data;
  std::vector<Tree<T>> children;
};

#endif