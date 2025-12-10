#include <algorithm>
#include <cstddef>
#include <functional>
#include <iostream>
#include <list>
#include <optional>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

template <typename K, typename V>
struct KeyValuePair {
  K key;
  V value;

  KeyValuePair(const K& key, const V& value) 
    : key(key), value(value) {}
};

template <typename K, typename V>
class Dictionary {
public:
  using Bucket = std::list<KeyValuePair<K, V>>;
  using Hash = std::function<unsigned(const K& k)>;

  Dictionary(const Hash& hash, std::size_t capacity = 128)
    : hash(hash), buckets(std::vector<Bucket>(capacity)), elements_count(0) {}

  void set(const K& key, const V& value) {
    unsigned index = hash(key) % buckets.size();
    Bucket& bucket = buckets[index];

    typename Bucket::iterator bucket_iterator = std::find_if(
      bucket.begin(), bucket.end(), 
      [&key](const KeyValuePair<K, V>& pair){
        return pair.key == key;
      }
    );
    
    KeyValuePair<K, V> entry(key, value);
    if (bucket_iterator != bucket.end()) {
      *bucket_iterator = entry;
      return;
    }

    bucket.push_back(entry);
    ++elements_count;

    if (load_factor() > 0.7) {
      resize();
    }
  }

  std::optional<V> lookup(const K& key) const {
    unsigned index = hash(key) % buckets.size();
    const Bucket& bucket = buckets[index];

    typename Bucket::const_iterator bucket_iterator = std::find_if(
      bucket.begin(), bucket.end(), 
      [&key](const KeyValuePair<K, V>& pair){
        return pair.key == key;
      }
    );

    if (bucket_iterator != bucket.end()) {
      return bucket_iterator->value;
    }

    return std::nullopt;
  }

  void remove(const K& key) {
    unsigned index = hash(key) % buckets.size();
    Bucket& bucket = buckets[index];

    typename Bucket::iterator bucket_iterator = std::find_if(
      bucket.begin(), bucket.end(), 
      [&key](const KeyValuePair<K, V>& pair){
        return pair.key == key;
      }
    );

    if (bucket_iterator != bucket.end()) {
      bucket.erase(bucket_iterator);
    }
  }

  class Iterator {
  public:
    using BucketsIterator = typename std::vector<Bucket>::iterator;
    using EntriesIterator = typename Bucket::iterator;

    Iterator(BucketsIterator bucket_iterator, BucketsIterator bucket_iterator_end, EntriesIterator entries_iterator)
      : bucket_iterator(bucket_iterator), bucket_iterator_end(bucket_iterator_end), entries_iterator(entries_iterator) {
      skip_empty_buckets();
    }

    KeyValuePair<K, V>& operator*() {
      return *entries_iterator;
    }

    Iterator& operator++() {
      ++entries_iterator;
      skip_empty_buckets();

      return *this;
    }

    bool operator!=(const Iterator& other) const {
      return bucket_iterator != other.bucket_iterator || entries_iterator != other.entries_iterator;
    }

  private:
    BucketsIterator bucket_iterator;
    BucketsIterator bucket_iterator_end;
    EntriesIterator entries_iterator;

    void skip_empty_buckets() {
      while(bucket_iterator != bucket_iterator_end && entries_iterator == bucket_iterator->end()) {
        ++bucket_iterator;
        if (bucket_iterator != bucket_iterator_end) {
          entries_iterator = bucket_iterator->begin();
        }
      }
    }
  };

  Iterator begin() {
    return Iterator(buckets.begin(), buckets.end(), buckets.front().begin());
  }

  Iterator end() {
    return Iterator(buckets.end(), buckets.end(), buckets.back().end());
  }

  std::vector<K> keys() {
    std::vector<K> result;

    for (const KeyValuePair<K, V>& entry : *this) {
      result.push_back(entry.key);
    }

    return result;
  }

  std::vector<V> values() {
    std::vector<V> result;

    for (const KeyValuePair<K, V>& entry : *this) {
      result.push_back(entry.value);
    }

    return result;
  }

private:
  std::vector<Bucket> buckets;
  Hash hash;
  std::size_t elements_count;

  static constexpr double load_factor_threshold = 0.7;

  double load_factor() const {
    return static_cast<double>(elements_count) / buckets.size();
  }

  void resize() {
    std::vector<Bucket> new_buckets(buckets.capacity() * 2);

    for (const KeyValuePair<K, V>& entry : *this) {
      unsigned index = hash(entry.key) % new_buckets.size();
      Bucket& bucket = new_buckets[index];

      typename Bucket::iterator bucket_iterator = std::find_if(
        bucket.begin(), bucket.end(), 
        [&entry](const KeyValuePair<K, V>& pair){
          return pair.key == entry.key;
        }
      );
      
      if (bucket_iterator != bucket.end()) {
        *bucket_iterator = entry;
        return;
      }

      bucket.push_back(entry);
    }

    buckets = std::move(new_buckets);
  }
};

template <typename T>
class Set : Dictionary<T, bool> {
public:
  Set() : Dictionary<T, bool>(std::hash<int>{}) {}

  void set(const T& value) {
    Dictionary<T, bool>::set(value, true);
  }

  bool lookup(const T& value) const {
    std::optional<bool> exists = Dictionary<T, bool>::lookup(value);

    return exists.has_value();
  }
};

template <typename V>
using Graph = std::unordered_map<V, std::list<V>>;

int main() {
  Dictionary<std::string, int> dict(std::hash<std::string>{}, 1);
  
  for (int i = 0; i < 20; ++i) {
    dict.set("Number: " + std::to_string(i), i);
  }

  dict.remove("Number: 15");

  std::optional<int> value = dict.lookup("Number: 15");
  if (value.has_value()) {
    std::cout << value.value() << '\n';
  } else {
    std::cout << "missing\n";
  }

  for (const KeyValuePair<std::string, int>& entry : dict) {
    std::cout << entry.key << " -> " << entry.value << '\n';
  }

  for (const std::string& key : dict.keys()) {
    std::cout << key << ", ";
  }
  std::cout << '\n';
  
  for (int value : dict.values()) {
    std::cout << value << ", ";
  }
  std::cout << '\n';

  Set<int> set;
  set.set(4);
  set.set(5);
  set.set(7);
  set.set(5);

  std::cout << std::boolalpha << set.lookup(5) << '\n';
  std::cout << std::boolalpha << set.lookup(8) << '\n';

  return 0;
}