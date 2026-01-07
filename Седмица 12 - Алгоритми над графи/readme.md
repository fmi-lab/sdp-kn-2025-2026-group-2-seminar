# Седмица 12 - Алгоритми над графи

```c++
template <typename V>
using Graph = std::unordered_map<V, std::list<V>>;

template <typename V, typename W>
using WeightedGraph = std::unordered_map<V, std::list<std::pair<V, W>>>;
```

## Задача 01 - Търсене на цикъл в ориентиран граф

```c++
template <typename V>
bool has_cycle(const Graph<V>& graph);
```

Напишете функция, която приема ориентиран граф и проверява дали в графа има цикъл.

### Пример:
```c++
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
```

## Задача 02 - Минимално покриващо дърво

```c++
template <typename V, typename W>
int mst(const WeightedGraph<V, W>& graph);
```

Напишете функция, която построява минимално покриващо дърво на неориентиран тегловен граф и връща теглото на това дърво. 

### Пример:
```c++
WeightedGraph<int, int> weighted_graph {
  {0, {{1, 2}, {3, 6}}},
  {1, {{0, 2}, {2, 3}, {3, 8}, {4, 8}}},
  {2, {{1, 3}, {4, 7}}},
  {3, {{0, 6}, {1, 8}}},
  {4, {{1, 5}, {2, 7}}}
};

std::cout << mst(weighted_graph) << '\n'; // -> 16
```

## Задача 03 - Минимален път в граф

```c++
template <typename V, typename W>
std::unordered_map<V, W> shortest_paths(const WeightedGraph<V, W>& graph, const V& source)
```

Напишете функция, която приема неориентиран тегловен граф и начален възел. Функцията да намира дължината на най-кратките пътища от началния връх до всички върхове на графа.

### Пример:
```c++
WeightedGraph<int, int> weighted_graph {
  {0, {{1, 2}, {3, 6}}},
  {1, {{0, 2}, {2, 3}, {3, 8}, {4, 5}}},
  {2, {{1, 3}, {4, 7}}},
  {3, {{0, 6}, {1, 8}}},
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
```