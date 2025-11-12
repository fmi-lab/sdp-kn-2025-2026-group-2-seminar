#ifndef BINARY_TREE_HPP
#define BINARY_TREE_HPP

#include <optional>
#include <stack>
#include <utility>
#include <vector>

template <typename T>
class BinaryTree {
public:
  BinaryTree() : root_node(nullptr) {}

  BinaryTree(const T& data, BinaryTree&& left = BinaryTree(), BinaryTree&& right = BinaryTree())
    : root_node(new TreeNode(data, std::exchange(left.root_node, nullptr), std::exchange(right.root_node, nullptr))) {}

  BinaryTree(const BinaryTree<T>& other) : root_node(copy(other.root_node)) {}

  BinaryTree(BinaryTree<T>&& other) : root_node(std::exchange(other.root_node, nullptr)) {}

  ~BinaryTree() {
    free(root_node);
  }

  BinaryTree<T>& operator=(const BinaryTree<T>& other) {
    BinaryTree<T> copy(other);
    swap(copy);

    return *this;
  }

  BinaryTree<T>& operator=(BinaryTree<T>&& other) {
    BinaryTree<T> copy(std::move(other));
    swap(copy);

    return *this;
  }

  const T& root() const {
    return root_node->data;
  }

  bool empty() const {
    return !root_node;
  }

  bool operator==(const BinaryTree& other) const {
    return equal(root_node, other.root_node);
  }

  std::vector<std::vector<T>> paths() const {
    std::vector<std::vector<T>> result;
    std::vector<T> path;

    paths(root_node, path, result);

    return result;
  }

  std::optional<T> lowest_common_ancestor(const T& lhs, const T& rhs) const {
    bool found_first = false, found_second = false;
    std::optional<T> result = lowest_common_ancestor(root_node, lhs, rhs, found_first, found_second);

    if (found_first && found_second) {
      return result;
    }

    return std::nullopt;
  }

private:
  struct TreeNode {
    T data;
    TreeNode *left, *right;

    TreeNode(const T& data, TreeNode* const left = nullptr, TreeNode* const right = nullptr)
    : data(data), left(left), right(right) {}
  };

public:
  class Iterator {
  public:
    Iterator(TreeNode* node) {
      windstack(node);
    }

    T& operator*() {
      return stack.top()->data;
    }

    const T& operator*() const {
      return stack.top()->data;
    }

    Iterator& operator++() {
      TreeNode* current = stack.top();
      stack.pop();

      windstack(current->right);

      return *this;
    }

    bool operator!=(const Iterator& other) const {
      if (stack.empty()) {
        return !other.stack.empty();
      }

      return other.stack.empty() || stack.top() != other.stack.top();
    }

  private:
    friend BinaryTree<T>;

    std::stack<TreeNode*> stack;
  
    void windstack(TreeNode* node) {
      while (node) {
        stack.push(node);
        node = node->left;
      }
    }
  };

  Iterator begin() const {
    return Iterator(root_node);
  }

  Iterator end() const {
    return Iterator(nullptr);
  }
  
private:
  TreeNode *root_node;

  TreeNode* copy(const TreeNode* node) {
    if (!node) {
      return nullptr;
    }

    return new TreeNode(node->data, copy(node->left), copy(node->right));
  }

  void free(TreeNode* node) {
    if (!node) {
      return;
    }

    free(node->left);
    free(node->right);
    delete node;
  }

  void swap(BinaryTree<T>& other) {
    using std::swap;

    swap(root_node, other.root_node);
  }

  bool equal(const TreeNode* node1, const TreeNode* node2) const {
    if (!node1 && !node2) {
      return true;
    }

    if (!node1 || !node2) {
      return false;      
    }

    return node1->data == node2->data &&
           equal(node1->left, node2->left) &&
           equal(node1->right, node2->right);
  }

  void paths(const TreeNode* node, std::vector<T>& current, std::vector<std::vector<T>>& all_paths) const {
    if (!node) {
      return;
    }

    current.push_back(node->data);

    if (!node->left && !node->right) {
      all_paths.push_back(current);
      current.pop_back();
      return;
    }

    paths(node->left, current, all_paths);
    paths(node->right, current, all_paths);

    current.pop_back();
  }

    std::optional<T> lowest_common_ancestor(const TreeNode* node, const T& lhs, const T& rhs, bool& found_first, bool& found_second) const {
    if (!node) {
      return std::nullopt;
    }
    
    std::optional<T> left = lowest_common_ancestor(node->left, lhs, rhs, found_first, found_second);
    std::optional<T> right = lowest_common_ancestor(node->right, lhs, rhs, found_first, found_second);
    
    if (node->data == lhs) {
      found_first = true;
      return node->data;
    }

    if (node->data == rhs) {
      found_second = true;
      return node->data;
    }


    if (left && right) {
      return node->data;
    }

    return left ? left : right;
  }
};

#endif