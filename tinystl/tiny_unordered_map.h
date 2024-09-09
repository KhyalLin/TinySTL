#pragma once

#include "tiny_hashtable.h"
#include "tiny_hash_fun.h"
#include <functional>

namespace Tiny
{

template <typename Key, typename T, typename HashFcn = hash<Key>,
        typename EqualKey = std::equal_to<Key>, typename Alloc = alloc>
class unordered_map
{
private:
    using ht = hashtable<std::pair<const Key, T>, Key, HashFcn,
            std::_Select1st<std::pair<const Key, T>>, EqualKey, Alloc>;
    ht rep;

public:
    using data_type = T;
    using mapped_type = T;
    using key_type = typename ht::key_type;
    using value_type = typename ht::value_type;
    using hasher = typename ht::hasher;
    using key_equal = typename ht::key_equal;

    using size_type = typename ht::size_type;
    using difference_type = typename ht::difference_type;
    using pointer = typename ht::pointer;
    using const_pointer = typename ht::const_pointer;
    using reference = typename ht::reference;
    using const_reference = typename ht::const_reference;

    using iterator = typename ht::iterator;
    using const_iterator = typename ht::const_iterator;

    hasher hash_funct() const { return rep.hash_funct(); }
    key_equal key_eq() const { return rep.key_eq(); }

public:
    unordered_map(size_type n = 50, const hasher& hf = hasher(),
                  const key_equal& eql = key_equal()) : rep(n, hf, eql) { }

    size_type size() const { return rep.size(); }
    bool empty() const { return rep.empty(); }
    void swap(unordered_map& h) { rep.swap(h.rep); }
    iterator begin() { return rep.begin(); }
    iterator end() { return rep.end(); }
    const_iterator begin() const { return rep.begin(); }
    const_iterator end() const { return rep.end(); }

    std::pair<iterator, bool> insert(const value_type& obj) {
        return rep.insert_unique(obj);
    }
    iterator find(const key_type& key) { 
        return rep.find(key);
    }
    const_iterator find(const key_type& key) const {
        return rep.find(key);
    }
    size_type count(const key_type& key) const {
        return rep.count(key);
    }
    T& operator[](const key_type& key) {
        iterator it = rep.find(key);
        if (it != rep.end()) return it->second;
        return rep.insert_unique({key, T()}).first->second;
    }

    size_type erase(const key_type& key) {
        return rep.erase(key);
    }
    void erase(iterator it) {
        rep.erase(it);
    }
    void clear() {
        rep.clear();
    }
    void resize(size_type hint) { rep.resize(hint); }
    size_type bucket_count() const { return rep.bucket_count(); }
    size_type max_bucket_count() const { return rep.max_bucket_count(); }
    size_type elems_in_bucket(size_type n) const { return rep.elems_in_bucket(n); }
};

}