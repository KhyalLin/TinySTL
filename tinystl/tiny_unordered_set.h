#pragma once

#include "tiny_hashtable.h"
#include "tiny_hash_fun.h"
#include <functional>

namespace Tiny
{

template <typename Value, typename HashFcn = hash<Value>, 
        typename EqualKey = std::equal_to<Value>, typename Alloc = alloc>
class unordered_set
{
private:
    using ht = hashtable<Value, Value, HashFcn, std::_Identity<Value>, EqualKey, Alloc>;
    ht rep;

public:
    using key_type = typename ht::key_type;
    using value_type = typename ht::value_type;
    using hasher = typename ht::hasher;
    using key_equal = typename ht::key_equal;
    
    using size_type = typename ht::size_type;
    using difference_type = typename ht::difference_type;
    using pointer = typename ht::const_pointer;
    using const_pointer = typename ht::const_pointer;
    using reference = typename ht::const_reference;
    using const_reference = typename ht::const_reference;

    using iterator = typename ht::const_iterator;
    using const_iterator = typename ht::const_iterator;

    hasher hash_funct() const { return rep.hash_funct(); }
    key_equal key_eq() const { return rep.key_eq(); }

public:
    unordered_set(size_type n = 50, const hasher& hf = hasher(), 
                  const key_equal& eql = key_equal()) : rep(n, hf, eql) { }
    
    size_type size() const { return rep.size(); }
    bool empty() const { return rep.empty(); }
    void swap(unordered_set& h) { return rep.swap(h.rep); }
    iterator begin() const { return rep.begin(); }
    iterator end() const { return rep.end(); }

    std::pair<iterator, bool> insert(const value_type& obj) {
        return rep.insert_unique(obj);
    }
    std::pair<iterator, bool> insert_noresize(const value_type& obj) {
        return rep.insert_unique_noresize(obj);
    }
    iterator find(const key_type& key) const {
        return rep.find(key);
    }
    size_type count(const key_type& key) const {
        return rep.count(key);
    }
    size_type erase(const key_type& key) const {
        return rep.erase(key);
    }
    void erase(iterator it) {
        return rep.erase(it);
    }
    void clear() {
        rep.clear();
    }

    void resize(size_type hint) {
        rep.resize(hint);
    }
    size_type bucket_count() const {
        return rep.bucket_count();
    }
    size_type max_bucket_count() const {
        return rep.max_bucket_count();
    }
    size_type elems_in_bucket(size_type n) const {
        return rep.elems_in_bucket();
    }
};

}