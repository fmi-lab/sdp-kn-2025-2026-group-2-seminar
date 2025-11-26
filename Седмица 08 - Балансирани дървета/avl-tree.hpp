#ifndef AVL_TREE_HPP
#define AVL_TREE_HPP

#include <complex>
#include <cstdio>
#include <iostream>
#include <optional>
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

  void insert(const K& key, const V& value)  {
    root_node = insert(root_node, key, value);
  }

  void remove(const K& key) {
    root_node = remove(root_node, key);
  }

  std::optional<V> closest_key(const K& key) const {
    if (!root_node) {
      return std::nullopt;
    }

    TreeNode *current = root_node;
    K closest_key = root_node->key;
    V closest_value = root_node->value;

    while (current) {
      if (std::abs(current->key - key) < std::abs(closest_key - key)) {
        closest_key = current->key;
        closest_value = current->value;
      }

      if (key < current->key) {
        current = current->left;
      } else if (key > current->key) {
        current = current->right;
      } else {
        return current->value;
      }
    }

    return closest_value;
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

  int get_height(const TreeNode* node) const {
    if (!node) {
      return 0;
    }

    return node->height;
  }

  int get_balance_factor(const TreeNode* node) const {
    if (!node) {
      return 0;
    }

    return get_height(node->left) - get_height(node->right);
  }

  TreeNode* rotate_left(TreeNode* node) {
    TreeNode* right = node->right;

    node->right = right->left;
    right->left = node;

    node->height = 1 + std::max(get_height(node->left), get_height(node->right));
    right->height = 1 + std::max(get_height(right->left), get_height(right->right));

    return right;
  }

  TreeNode* rotate_right(TreeNode* node) {
    TreeNode* left = node->left;

    node->left = left->right;
    left->right = node;

    node->height = 1 + std::max(get_height(node->left), get_height(node->right));
    left->height = 1 + std::max(get_height(left->left), get_height(left->right));

    return left;
  }

  TreeNode* balance(TreeNode* node) {
    int balance_factor = get_balance_factor(node);

    if (balance_factor < - 1) {
      if (get_balance_factor(node->right) > 0) {
        node->right = rotate_right(node->right);
      }

      node = rotate_left(node);
    } else if (balance_factor > 1) {
      if (get_balance_factor(node->left) < 0) {
        node->left = rotate_left(node->left);
      }

      node = rotate_right(node);
    }

    return node;
  }

  TreeNode* leftmost(TreeNode* node) {
    TreeNode* current = node;

    while (current->left) {
      current = current->left;
    }

    return current;
  }

  TreeNode* insert(TreeNode* node, const K& key, const V& value) {
    if (!node) {
      return new TreeNode(key, value);
    }

    if (key < node->key) {
      node->left = insert(node->left, key, value);
    } else if (key > node->key) {
      node->right = insert(node->right, key, value);
    } else {
      node->value = value;
      return node;
    }

    node->height = 1 + std::max(get_height(node->left), get_height(node->right));

    return balance(node);
  }

  TreeNode* remove(TreeNode* node, const K& key) {
    if (!node) {
      return node;
    }

    if (key < node->key) {
      node->left = remove(node->left, key);
    } else if (key > node->key) {
      node->right = remove(node->right, key);
    } else {
      if (!node->left && !node->right) {
        delete node;
        node = nullptr;
      } else if (!node->left || !node->right) {
        TreeNode* new_node = node->left ? node->left : node->right;

        delete node;
        node = new_node;
      } else {
        TreeNode* successor = leftmost(node->right);

        node->key = successor->key;
        node->value = successor->value;

        node->right = remove(node->right, successor->key);
      }
    }

    if (!node) {
      return node;
    }

    node->height = 1 + std::max(get_height(node->left), get_height(node->right));

    return balance(node);    
  }
};

#endif