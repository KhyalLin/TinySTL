#pragma once

// waiting for less<>, binary_function<>, _Select1st<>

#include "tiny_tree.h"
#include <functional>

namespace Tiny
{

template <typename Key, typename T, typename Compare = std::less<Key>, typename Alloc = alloc>
class map
{
public:
    using key_type = Key;
    using data_type = T;
    using mapped_type = T;
    using value_type = std::pair<const Key, T>;
    using key_compare = Compare;
    using self = map<Key, T, Compare, Alloc>;

    class value_compare : public std::binary_function<value_type, value_type, bool>
    {
        friend class map<Key, T, Compare, Alloc>;
    protected:
        Compare comp;
        value_compare(Compare c) : comp(c) { }
    public:
        bool operator()(const value_type& x, const value_type& y) const {
            return comp(x.first, y.first);
        }
    };

private:
    using rep_type = rb_tree<key_type, value_type, std::_Select1st<value_type>, key_compare, Alloc>;
    rep_type t;

public:
    using pointer = typename rep_type::pointer;
    using const_pouinter = typename rep_type::const_pointer;
    using reference = typename rep_type::reference;
    using const_reference = typename rep_type::const_reference;
    using iterator = typename rep_type::iterator;
    using const_iterator = typename rep_type::const_iterator;
    using size_type = typename rep_type::size_type;
    using difference_type = typename rep_type::difference_type;

    map() : t(Compare()) { }
    explicit map(const Compare& comp) : t(comp) { }
    map(const self& x) : t(x.t) { }
    map(self&& x) : t(x.t) { }
    self& operator=(const self& x) { t = x.t; return *this; }

    key_compare key_comp() const { return t.key_comp(); }
    value_compare value_comp() const { return value_compare(t.key_comp()); }
    iterator begin() { return t.begin(); }
    const_iterator begin() const { return t.begin(); }
    iterator end() { return t.end(); }
    const_iterator end() const { return t.end(); }
    bool empty() const { return t.empty(); }
    size_type size() const { return t.size(); }
    static size_type max_size() { return rep_type::max_size(); }
    T& operator[](const key_type& k) {
        return insert(value_type(k, T())).first->second;
    }
    void swap(self& x) { t.swap(x.t); }
    
    using pair_iterator_bool = std::pair<iterator, bool>;
    pair_iterator_bool insert(const value_type& x) {
        return t.insert_unique(x);
    }
    void erase(iterator position) {
        t.erase(position);
    }
    size_type erase(const value_type& x) {
        t.erase(x);
    }
    void clear() { t.clear(); }

    iterator find(const key_type& x) { return t.find(x); }
    const_iterator find(const key_type& x) const { return t.find(x); }
    size_type count(const key_type& x) const { return t.count(x); }
    iterator lower_bound(const key_type& x) { 
        return t.lower_bound(x); 
    }
    const_iterator lower_bound(const key_type& x) const { 
        return t.lower_bound(x); 
    }
    iterator upper_bound(const key_type& x) { 
        return t.upper_bound(x); 
    }
    const_iterator upper_bound(const key_type& x) const { 
        return t.upper_bound(x); 
    }
};

}