#include <iostream>
#include "linked_list.hpp"

int main() {
  LinkedList<int> list;

  for (int i : {1, 2, 3, 4, 5}) {
    list.insert_last(i);
  }

  LinkedList<int>::Iterator it = ++list.begin();
  list.insert_after(6, it);

  list.remove_at(it);
  for (auto i = list.rbegin(); i != list.rend(); ++i) {
    std::cout << *i << " ";
  }
  std::cout << '\n';

  LinkedList<int> list2;
  for (int i : {7, 8, 9}) {
    list.insert_last(i);
  }

  list.append(list2);
  for (int i : list) {
    std::cout << i << ' ';
  }
  std::cout << '\n';

  LinkedList<int> list3;
  for (int i : {1, 2, 1, 3, 2}) {
    list3.insert_last(i);
  }

  // list3.unique();
  list3.filter([](const int& x){ return x != 1; });
  list3.insert_last(4);
  for (int i : list3) {
    std::cout << i << ' ';
  }
  std::cout << '\n';

  return 0;
}