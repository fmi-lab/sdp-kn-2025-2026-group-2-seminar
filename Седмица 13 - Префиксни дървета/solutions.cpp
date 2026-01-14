#include <cstddef>
#include <iostream>
#include <optional>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

template <typename T>
class Trie {
public:
  Trie() : root(new Node) {}
  Trie(const Trie<T>& other) : root(copy(other.root)) {}
  Trie(Trie<T>&& other) : root(std::exchange(other.root, nullptr)) {}
  ~Trie() {
    free(root);
  }
  Trie<T>& operator=(const Trie<T>& other) {
    Trie<T> copy(other);
    swap(copy);
    return *this;
  }
  Trie<T>& operator=(Trie<T>&& other) {
    Trie<T> copy(std::move(other));
    swap(copy);
    return *this;
  }

  void insert(const std::string& word, const T& value) {
    Node* node = root;

    for (char c : word) {
      if (!node->children.count(c)) {
        node->children[c] = new Node;
      }

      node = node->children[c];
    }

    node->value = value;
  }

  std::optional<T> lookup(const std::string& word) const {
    Node* node = root;

    for (char c : word) {
      if (!node->children.count(c)) {
        return std::nullopt;
      }

      node = node->children[c];
    }

    return node->value;
  }

  std::vector<std::string> autocomplete(const std::string& prefix) const {
    Node* node = root;

    for (char c : prefix) {
      if (!node->children.count(c)) {
        return std::vector<std::string>();
      }

      node = node->children[c];
    }

    std::vector<std::string> suffixes = collect(node);

    for (std::string& suffix : suffixes) {
      suffix = prefix + suffix;
    }

    return suffixes;
  }

  std::vector<std::string> all_words() const {
    return collect(root);
  }

  void remove(const std::string& word) {
    remove(root, word, 0);
  }

private:
  struct Node {
    std::optional<T> value;
    std::unordered_map<char, Node*> children;

    Node(const std::optional<T>& value = std::nullopt) : value(value) {}
  };

  Node* root;

  std::vector<std::string> collect(Node* node) const {
    std::vector<std::string> result;

    if (node->value.has_value()) {
      result.push_back("");
    }

    for (const auto& [c, child] : node->children) {
      std::vector<std::string> suffixes = collect(child);

      for (std::string& suffix : suffixes) {
        suffix = c + suffix;
      }

      result.insert(result.end(), suffixes.begin(), suffixes.end());
    }

    return result;
  }

  Node* remove(Node* node, const std::string& word, std::size_t index) {
    if (index == word.size()) {
      node->value.reset();
    } else {
      char c = word[index];
      auto it = node->children.find(c);

      if (it != node->children.end()) {
        Node* updated_node = remove(it->second, word, index + 1);
        if (!updated_node) {
          node->children.erase(it);
        } else {
          node->children[c] = updated_node;
        }
      }
    }

    if (!node->value.has_value() && node->children.empty() && node != root) {
      delete node;
      node = nullptr;
    }

    return node;
  }

  Node* copy(Node* node) const {
    Node* new_node = new Node(node->value);

    for (const auto& [c, child] : node->children) {
      new_node->children[c] = copy(child);  
    }

    return new_node;
  }

  void free(Node* node) const {
    for (const auto& [_, child] : node->children) {
      free(child);
    }

    delete node;
  }

  void swap(Trie<T>& other) {
    using std::swap;

    std::swap(root, other.root);
  }
};

int main() {
  Trie<int> trie;
  trie.insert("to", 7);
  trie.insert("A", 15);
  trie.insert("tea", 3);
  trie.insert("ted", 4);
  trie.insert("ten", 12);
  trie.insert("inn", 9);
  trie.insert("in", 5);
  trie.insert("", 0);

  std::optional<int> value = trie.lookup("");
  if (value.has_value()) {
    std::cout << value.value() << '\n';
  } else {
    std::cout << "Missing\n";
  }

  trie.remove("");

  for (const std::string& word : trie.all_words()) {
    std::cout << word << ' ';
  }
  std::cout << '\n';

  for (const std::string& word : trie.autocomplete("te")) {
    std::cout << word << ' ';
  }
  std::cout << '\n';

  return 0;
}