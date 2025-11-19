#ifndef BST_HPP
#define BST_HPP

#include <iostream>
#include <optional>
#include <stack>
#include <string>
#include <utility>
#include <vector>

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

  std::optional<V> search(const K& key) const {
    TreeNode* current = root_node;

    while (current && current->key != key) {
      if (current->key > key) {
        current = current->left;
      } else {
        current = current->right;
      }
    }

    if (!current) {
      return std::nullopt;
    }

    return current->value;
  }

  void insert(const K& key, const V& value) {
    if (!root_node) {
      root_node = new TreeNode(key, value);
      return;
    }

    TreeNode *current = root_node, *parent = nullptr;
    while (current && current->key != key) {
      parent = current;
      if (current->key > key) {
        current = current->left;
      } else {
        current = current->right;
      }
    }

    if (current) {
      current->value = value;
      return;
    }

    if (parent->key > key) {
      parent->left = new TreeNode(key, value);
    } else {
      parent->right = new TreeNode(key, value);
    }
  }

  void remove(const K& key) {
    TreeNode *current = root_node, *parent = nullptr;

    while (current && current->key != key) {
      parent = current;
      if (current->key > key) {
        current = current->left;
      } else {
        current = current->right;
      }
    }

    if (!current) {
      return;
    }

    if (!current->left || !current->right) {
      TreeNode* new_node = current->left ? current->left : current->right;

      if (!parent) {
        root_node = new_node;
      } else if (parent->left == current) {
        parent->left = new_node;
      } else {
        parent->right = new_node;
      }

      delete current;
      return;
    }

    TreeNode *successor = current->right, *successor_parent = current;
    while (successor->left) {
      successor_parent = successor;
      successor = successor->left;
    }

    current->key = successor->key;
    current->value = successor->value;

    if (successor_parent == current) {
      successor_parent->right = successor->right;
    } else {
      successor_parent->left = successor->right;
    }

    delete successor;
  }

  void merge(const BinarySearchTree& other) {
    std::vector<std::pair<K, V>> result;

    Iterator begin_this = begin(), end_this = end(), begin_other = other.begin(), end_other = other.end();
    while (begin_this != end_this && begin_other != end_other) {
      if (begin_this.key() < begin_other.key()) {
        result.push_back(std::make_pair(begin_this.key(), *begin_this));
        ++begin_this;
      } else {
        if (begin_this.key() == begin_other.key()) {
          ++begin_this;
        }

        result.push_back(std::make_pair(begin_other.key(), *begin_other));
        ++begin_other;
      }
    }

    while (begin_this != end_this) {
      result.push_back(std::make_pair(begin_this.key(), *begin_this));
      ++begin_this;
    }

    while (begin_other != end_other) {
      result.push_back(std::make_pair(begin_other.key(), *begin_other));
      ++begin_other;
    }

    free(root_node);
    root_node = build(result, 0, result.size() - 1);
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

    const K& key() const {
      return stack.top()->key;
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

  TreeNode* build(const std::vector<std::pair<K, V>>& element, int begin, int end) {
    if (begin > end) {
      return nullptr;
    }

    int mid = (end - begin) / 2 + begin;
    auto [key, value] = element[mid];
    return new TreeNode(key, value, build(element, begin, mid - 1), build(element, mid + 1, end));
  }

};

#endif