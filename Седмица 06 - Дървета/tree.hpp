#ifndef TREE_HPP
#define TREE_HPP

#include <iostream>
#include <queue>
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

  void dfs() const {
    std::cout << root() << ' ';

    for (const Tree<T>& child : children) {
      child.dfs();
    }
  }

  void bfs() const {
    std::queue<Tree<T>> queue;
    queue.push(*this);

    while (!queue.empty()) {
      Tree<T> current = queue.front();
      queue.pop();

      std::cout << current.root() << ' ';

      for (const Tree<T>& child : current.children) {
        queue.push(child);
      }
    }
  }

  bool contains(const T& data) const {
    if (data == root()) {
      return true;
    }

    for (const Tree<T>& child : children) {
      if (child.contains(data)) {
        return true;
      }
    }

    return false;
  }

private:
  T data;
  std::vector<Tree<T>> children;
};

#endif