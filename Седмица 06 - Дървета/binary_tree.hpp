#ifndef BINARY_TREE_HPP
#define BINARY_TREE_HPP

#include <utility>

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
private:
  struct TreeNode {
    T data;
    TreeNode *left, *right;

    TreeNode(const T& data, TreeNode* const left = nullptr, TreeNode* const right = nullptr)
    : data(data), left(left), right(right) {}
  };

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
};

#endif