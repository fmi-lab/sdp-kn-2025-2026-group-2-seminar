#include <algorithm>
#include <iostream>
#include <list>
#include <queue>
#include <stack>
#include <utility>
#include <vector>

template <typename K, typename V>
struct KeyValuePair {
  K key;
  V value;

  KeyValuePair(const K& key, const V& value) 
    : key(key), value(value) {}
};

template <typename V>
class Graph {
public:
  using EdgeList = KeyValuePair<V, std::list<V>>;

  std::vector<V> vertices() const {
    std::vector<V> result;

    for (const EdgeList& e : data) {
      result.push_back(e.key);
    }

    return result;
  }

  std::list<V> successors(const V& u) const {
    for (const EdgeList& e : data) {
      if (e.key == u) {
        return e.value;
      }
    }

    return std::list<V>();
  }

  void add_vertex(const V& u) {
    data.push_back(EdgeList(u, std::list<V>()));
  }

  void add_edge(const V& u, const V& v) {
    for (EdgeList& e : data) {
      if (e.key == u) {
        e.value.push_back(v);
      }
    }
  }

  void print() const {
    for (const EdgeList& e : data) {
      std::cout << e.key << " -> ";
      for (const V& v : e.value) {
        std::cout << v << " ";
      }
      std::cout << '\n';
    }
  }

  bool has_edge(const V& u, const V& v) const {
    for (const V& s : successors(u)) {
      if (s == v) {
        return true;
      }
    }

    return false;
  }

private:
  std::vector<EdgeList> data;
};

template <typename V>
std::pair<unsigned, unsigned> degree(const Graph<V>& graph, const V& u) {
  unsigned out_degree = graph.successors(u).size();
  unsigned in_degree = 0;

  for (const V& v : graph.vertices()) {
    for (const V& w : graph.successors(v)) {
      if (w == u) {
        ++in_degree;
      }
    }
  }

  return std::make_pair(in_degree, out_degree);
}

template <typename V>
Graph<V> invert(const Graph<V>& graph) {
  Graph<V> result;

  for (const V& v : graph.vertices()) {
    result.add_vertex(v);
  }

  for (const V& v : graph.vertices()) {
    for (const V& u : graph.successors(v)) {
      result.add_edge(u, v);
    }
  }

  return result;
}

template <typename V>
void dfs(const Graph<V>& graph, std::vector<V>& path, const V& u) {
  path.push_back(u);

  for (const V& v : graph.successors(u)) {
    if (std::find(path.begin(), path.end(), v) == path.end()) {
      dfs(graph, path, v);
    }
  }
}

template <typename V>
std::vector<V> dfs(const Graph<V>& graph) {
  std::vector<V> path;

  for (const V& u : graph.vertices()) {
    if (std::find(path.begin(), path.end(), u) == path.end()) {
      dfs(graph, path, u);
    }
  }
  
  return path;
}

template <typename V>
void bfs(const Graph<V>& graph, std::vector<V>& path, const V& u) {
  std::queue<V> queue;
  queue.push(u);

  while (!queue.empty()) {
    V current = queue.front();
    queue.pop();

    path.push_back(current);

    for (const V& v : graph.successors(current)) {
      if (std::find(path.begin(), path.end(), u) == path.end()) {
        queue.push(v);
      }
    }
  }
}

template <typename V>
std::vector<V> bfs(const Graph<V>& graph) {
  std::vector<V> path;
  
  for (const V& u : graph.vertices()) {
    if (std::find(path.begin(), path.end(), u) == path.end()) {
      bfs(graph, path, u);
    }
  }

  return path;
}


template <typename V>
void topological_sort(const Graph<V>& graph, std::vector<V>& path, const V& u, std::stack<V>& stack) {
  path.push_back(u);

  for (const V& v : graph.successors(u)) {
    if (std::find(path.begin(), path.end(), v) == path.end()) {
      topological_sort(graph, path, v, stack);
    }
  }

  stack.push(u);
}

template <typename V>
std::vector<V> topological_sort(const Graph<V>& graph) {
  std::vector<V> path;
  std::stack<V> stack;

  for (const V& u : graph.vertices()) {
    if (std::find(path.begin(), path.end(), u) == path.end()) {
      topological_sort(graph, path, u, stack);
    }
  }
  
  std::vector<V> result;
  while (!stack.empty()) {
    result.push_back(stack.top());
    stack.pop();
  }

  return result;
}

int main() {
  Graph<int> graph;
  graph.add_vertex(1);
  graph.add_vertex(2);
  graph.add_vertex(3);
  graph.add_vertex(4);
  graph.add_vertex(5);

  graph.add_edge(1, 2);
  graph.add_edge(1, 3);
  graph.add_edge(1, 4);

  graph.add_edge(2, 1);
  graph.add_edge(2, 3);
  graph.add_edge(2, 5);

  graph.add_edge(3, 1);
  graph.add_edge(3, 2);
  graph.add_edge(3, 4);
  graph.add_edge(3, 5);

  graph.add_edge(4, 1);
  graph.add_edge(4, 3);
  graph.add_edge(4, 5);

  graph.add_edge(5, 2);
  graph.add_edge(5, 3);
  graph.add_edge(5, 4);

  graph.print();
  std::cout << '\n';

  Graph<int> oriented_graph;
  oriented_graph.add_vertex(1);
  oriented_graph.add_vertex(2);
  oriented_graph.add_vertex(3);
  oriented_graph.add_vertex(4);
  oriented_graph.add_vertex(5);

  oriented_graph.add_edge(1, 2);
  oriented_graph.add_edge(1, 3);
  oriented_graph.add_edge(1, 4);

  oriented_graph.add_edge(2, 3);
  oriented_graph.add_edge(2, 5);

  oriented_graph.add_edge(3, 4);
  oriented_graph.add_edge(3, 5);

  oriented_graph.add_edge(5, 2);
  oriented_graph.add_edge(5, 4);

  oriented_graph.print();
  std::cout << '\n';

  std::pair<unsigned, unsigned> d1 = degree(oriented_graph, 1);
  std::cout << d1.first << ' ' << d1.second << '\n'; // -> 0 3

  std::pair<unsigned, unsigned> d2 = degree(oriented_graph, 2);
  std::cout << d2.first << ' ' << d2.second << '\n'; // -> 2 2

  std::cout << '\n';
  Graph<int> inverted_graph = invert(oriented_graph);
  inverted_graph.print();
  std::cout << '\n';

  std::cout << '\n';
  for (int v : dfs(oriented_graph)) {
    std::cout << v << ' ';
  }
  std::cout << '\n';

  for (int v : dfs(inverted_graph)) {
    std::cout << v << ' ';
  }
  std::cout << '\n';

  std::cout << '\n';
  for (int v : bfs(oriented_graph)) {
    std::cout << v << ' ';
  }
  std::cout << '\n';

  Graph<int> dag;
  dag.add_vertex(1);
  dag.add_vertex(2);
  dag.add_vertex(3);
  dag.add_vertex(4);
  dag.add_vertex(5);

  dag.add_edge(1, 2);
  dag.add_edge(3, 2);
  dag.add_edge(4, 3);
  dag.add_edge(5, 3);

  dag.print();
  std::cout << '\n';

  std::cout << '\n';
  for (int v : topological_sort(dag)) {
    std::cout << v << ' ';
  }
  std::cout << '\n';
  return 0;
}