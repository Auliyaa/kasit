#pragma once

#include <unordered_map>
#include <list>
#include <utility>

namespace kasit
{

template <typename Key, typename Value>
class lru_cache_t
{
public:
  explicit lru_cache_t(size_t cache_max_sz)
    : _cache_max_sz(cache_max_sz)
  {
  }
  
  bool get(const Key& key, Value& value)
  {
    auto it = _cache.find(key);
    if (it == _cache.end()) return false;
    
    touch(it);
    value = it->second.value;
    return true;
  }
  
  Value get(const Key& key)
  {
    Value r{};
    get(key, r);
    return r;
  }
  
  bool has(const Key& key) const
  {
    return _cache.find(key) != _cache.end();
  }

  void put(const Key& key, const Value& value)
  {
    auto it = _cache.find(key);
    
    if (it != _cache.end())
    {
      it->second.value = value;
      touch(it);
      return;
    }
    
    while (_cache.size() >= _cache_max_sz) evict();
    
    _lru_list.push_front(key);
    _cache.emplace(key, entry{value, _lru_list.begin()});
  }

  void resize(size_t new_cache_max_sz)
  {
    _cache_max_sz = new_cache_max_sz;
    while (_cache.size() > _cache_max_sz) evict();
  }
  
  size_t size() const
  {
    return _cache.size();
  }
  
  size_t capacity() const
  {
    return _cache_max_sz;
  }
  
  void clear()
  {
    _cache.clear();
    _lru_list.clear();
  }
  
private:
  struct entry
  {
    Value value;
    typename std::list<Key>::iterator it;
  };
  
  size_t _cache_max_sz;
  std::list<Key> _lru_list;
  std::unordered_map<Key, entry> _cache;
  
  void touch(typename std::unordered_map<Key, entry>::iterator it)
  {
    _lru_list.erase(it->second.it);
    _lru_list.push_front(it->first);
    it->second.it = _lru_list.begin();
  }
  
  void evict()
  {
    const Key& lru_key = _lru_list.back();
    _cache.erase(lru_key);
    _lru_list.pop_back();
  }
};
  
} // ::kasit