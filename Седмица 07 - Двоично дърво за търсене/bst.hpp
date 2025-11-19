#ifndef BST_HPP
#define BST_HPP

#include <iostream>
#include <optional>
#include <stack>
#include <string>
#include <utility>

template <typename K, typename V>
class BinarySearchTree {
public:
  BinarySearchTree() : root_node(nullptr) {}
  BinarySearchTree(const BinarySearchTree &other)
      : root_node(copy(other.root_node)) {}
  BinarySearchTree<K, V> &operator=(const BinarySearchTree &other) {
    BinarySearchTree<K, V> copy(other);
    swap(copy);

    return *this;
  }
  BinarySearchTree(BinarySearchTree &&other) 
    : root_node(std::exchange(other.root_node, nullptr)) {}
  BinarySearchTree<K, V> &operator=(BinarySearchTree &&other) {
    BinarySearchTree<K, V> copy(std::move(other));
    swap(copy);

    return *this;
  }
  ~BinarySearchTree() { free(root_node); }

  void pretty_print() {
    pretty_print(root_node, "", true);
  }

private:
  struct TreeNode {
    K key;
    V value;
    TreeNode *left, *right;

    TreeNode(const K &key, const V &value, TreeNode *const left = nullptr,
            TreeNode *const rigth = nullptr)
        : key(key), value(value), left(left), right(rigth) {}
  };

public:
  class Iterator {
  public:
    Iterator(TreeNode* node) {
      windstack(node);
    }

    V& operator*() {
      return stack.top()->value;
    }

    const V& operator*() const {
      return stack.top()->value;
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
    friend BinarySearchTree<K, V>;

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
  TreeNode* root_node;

  TreeNode *copy(TreeNode *const node) {
    if (!node) {
      return nullptr;
    }

    return new TreeNode(node->key, node->value, copy(node->left), copy(node->right));
  }

  void free(TreeNode *node) {
    if (!node) {
      return;
    }

    free(node->left);
    free(node->right);

    delete node;
  }

  void pretty_print(TreeNode *node, std::string indent, bool last) {
    if (node != nullptr) {
      std::cout << indent;
      if (last) {
        std::cout << "R----";
        indent += "   ";
      } else {
        std::cout << "L----";
        indent += "|  ";
      }
      std::cout << node->value << '\n';
      pretty_print(node->left, indent, false);
      pretty_print(node->right, indent, true);
    }
  }

};

#endif