#pragma once

// waiting for less<>, forward<>
#include "tiny_tree.h"
#include "tiny_functional.h"
#include <functional>

namespace Tiny
{

template <typename Key, typename Compare = std::less<Key>, typename Alloc = alloc>
class set
{
public:
    using key_type = Key;
    using value_type = Key;
    using key_compare = Compare;
    using value_compare = Compare;
    using self = set<Key, Compare, Alloc>;

private:
    using rep_type = rb_tree<key_type, value_type, identity<value_type>, key_compare, Alloc>;
    rep_type t;

public:
    using pointer = typename rep_type::const_pointer;
    using const_pointer = typename rep_type::const_pointer;
    using reference = typename rep_type::const_reference;
    using const_reference = typename rep_type::const_reference;
    using iterator = typename rep_type::const_iterator;
    using const_iterator = typename rep_type::const_iterator;
    using size_type = typename rep_type::size_type;
    using difference_type = typename rep_type::difference_type;    
    set() : t(Compare()) { }
    explicit set(const Compare& comp) : t(comp) { }
    set(const self& x) : t(x.t) { }
    set(self&& x) : t(x.t) { }

    self& operator=(const self& x)  { t = x.t; return *this; }
    key_compare key_comp() const { return t.key_comp(); }
    value_compare value_comp() const { return t.key_comp(); }
    iterator begin() const { return t.begin(); }
    iterator end() const { return t.end(); }
    bool empty() const { return t.empty(); }
    size_type size() const { return t.size(); }
    static size_type max_size() { return rep_type::max_size(); }
    void swap(self& x) { t.swap(x.t); }

    using pair_iterator_bool = std::pair<iterator, bool>;
    pair_iterator_bool insert(const value_type& x) {
        return t.insert_unique(x);
    }
    void erase(iterator position) {
        t.erase(position);
    }
    size_type erase(const value_type& x) {
        return t.erase(x);
    }
    void clear() { t.clear(); }

    iterator find(const key_type& x) const { return t.find(x); }
    size_type count(const key_type& x) const { return t.count(x); }
    iterator lower_bound(const key_type& x) const {
        return t.lower_bound(x);
    }
    iterator upper_bound(const key_type x) const {
        return t.upper_bound(x);
    }
};

}