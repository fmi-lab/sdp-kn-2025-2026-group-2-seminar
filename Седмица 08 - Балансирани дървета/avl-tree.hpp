#ifndef AVL_TREE_HPP
#define AVL_TREE_HPP

#include <iostream>
#include <stack>
#include <string>
#include <utility>

template <typename K, typename V>
class AVLTree {
public:
  AVLTree() : root_node(nullptr) {}
  AVLTree(const AVLTree &other)
    : root_node(copy(other.root_node)) {}
  AVLTree<K, V> &operator=(const AVLTree &other) {
    AVLTree<K, V> copy(other);
    swap(copy);

    return *this;
  }
  AVLTree(AVLTree &&other)
    : root_node(std::exchange(other.root_node, nullptr)) {}
  AVLTree<K, V> &operator=(AVLTree &&other) {
    AVLTree<K, V> copy(std::move(other));
    swap(copy);

    return *this;
  }
  ~AVLTree() { free(root_node); }

  void pretty_print() {
    pretty_print(root_node, "", true);
  }

private:
  struct TreeNode {
    K key;
    V value;
    TreeNode* left;
    TreeNode* right;
    std::size_t height;

    TreeNode(const K& key, const V& value) 
      : key(key), value(value), left(nullptr), right(nullptr), height(1) {}
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

    const K& key() const {
      return stack.top()->key;
    }

  private:
    friend AVLTree<K, V>;

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
};

#endif