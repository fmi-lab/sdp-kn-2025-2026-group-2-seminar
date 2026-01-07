#include <functional>
#include <iostream>
#include <limits>
#include <list>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

template <typename V>
using Graph = std::unordered_map<V, std::list<V>>;

template <typename V, typename W>
using WeightedGraph = std::unordered_map<V, std::list<std::pair<V, W>>>;

// Kahn's algorithm
template <typename V>
bool has_cycle(const Graph<V>& graph) {
  std::unordered_map<V, unsigned> indegrees;
  std::vector<V> result;
  std::queue<V> queue;

  for (const auto& [_, children] : graph) {
    for (const V& child : children) {
      if (indegrees.count(child) == 0) {
        indegrees[child] = 0;
      }
      ++indegrees[child];
    }
  }

  for (const auto& [v, _] : graph) {
    if (indegrees.count(v) == 0) {
      queue.push(v);
    }
  }

  while (!queue.empty()) {
    V current = queue.front();
    queue.pop();

    result.push_back(current);

    for (const V& child : graph.at(current)) {
      --indegrees[child];

      if (indegrees[child] == 0) {
        queue.push(child);
      }
    }
  }

  return result.size() != graph.size();
}

// Dijkstra's algorithm
template <typename V, typename W>
std::unordered_map<V, W> shortest_paths(const WeightedGraph<V, W>& graph, const V& source) {
  std::unordered_map<V, W> distances;

  for (const auto& [v, _] : graph) {
    distances[v] = std::numeric_limits<W>::max();
  }
  distances[source] = 0;

  using pwv = std::pair<W, V>;
  std::priority_queue<pwv, std::vector<pwv>, std::greater<pwv>> queue;
  queue.push({0, source});

  while (!queue.empty()) {
    auto [distance, v] = queue.top();
    queue.pop();

    if (distance <= distances[v]) {
      for (const auto& [u, w] : graph.at(v)) {
        if (distances[v] + w < distances[u]) {
          distances[u] = distances[v] + w;
          queue.push({distances[u], u});
        }
      }
    }    
  }

  return distances;
}

// Prim's algorithm
template <typename V, typename W>
int mst(const WeightedGraph<V, W>& graph) {
  std::unordered_set<V> visited;

  using pwv = std::pair<W, V>;
  std::priority_queue<pwv, std::vector<pwv>, std::greater<pwv>> queue;
  queue.push({0, graph.begin()->first});

  int result = 0;
  
  while (!queue.empty()) {
    auto [weight, v] = queue.top();
    queue.pop();

    if (visited.find(v) == visited.end()) {
      visited.insert(v);
      result += weight;

      for (auto const& [u, w] : graph.at(v)) {
        if (visited.find(u) == visited.end()) {
          queue.push({w, u});
        }
      }
    }
  }

  return result;
}

int main() {
  Graph<int> graph = {
    {0, {1}},
    {1, {2}},
    {2, {3}},
    {3, {}},
    {4, {5}},
    {5, {1, 2}}
  };

  std::cout << std::boolalpha << has_cycle(graph) << '\n'; // -> false

  graph[3].push_back(5);
  std::cout << has_cycle(graph) << '\n'; // -> true

  WeightedGraph<int, int> weighted_graph {
    {0, {{1, 2}, {3, 6}}},
    {1, {{0, 2}, {2, 3}, {3, 2}, {4, 5}}},
    {2, {{1, 3}, {4, 7}}},
    {3, {{0, 6}, {1, 2}}},
    {4, {{1, 5}, {2, 7}}}
  };

  for (const auto& [v, d] : shortest_paths(weighted_graph, 0)) {
    std::cout << "0 -> " << v << " = " << d << '\n';
  }

  /*
  0 -> 0 = 0
  0 -> 1 = 2
  0 -> 2 = 5
  0 -> 3 = 6
  0 -> 4 = 7
  */
  return 0;
}