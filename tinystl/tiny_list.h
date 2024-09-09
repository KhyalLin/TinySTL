#pragma once

// waiting for swap()

#include <algorithm>
#include <iso646.h>
#include "tiny_construct.h"
#include "tiny_alloc.h"
#include "tiny_iterator.h"

namespace Tiny
{

template <typename T>
struct __list_node
{
    using node_pointer = __list_node<T>*;
    node_pointer prev;
    node_pointer next;
    T data;
};

template <typename T, typename Ref, typename Ptr>
struct __list_iterator
{
    using iterator = __list_iterator<T, T&, T*>;
    using self = __list_iterator<T, Ref, Ptr>;

    using iterator_category = bidirection_iterator_tag;
    using value_type = T;
    using pointer = Ptr;
    using reference = Ref;
    using link_type = __list_node<T>*;
    using size_type = size_t;
    using difference_type = ptrdiff_t;

    link_type node;

    __list_iterator() { }
    __list_iterator(link_type x) : node(x) { }
    __list_iterator(const iterator& x) : node(x.node) { }

    bool operator==(const self& x) const { return node == x.node; }
    bool operator!=(const self& x) const { return node != x.node; }
    reference operator*() const { return node->data; }
    pointer operator->() const { return &operator*(); }

    self& operator++() {
        node = node->next;
        return *this;
    }
    self operator++(int) {
        self tmp = *this;
        node = node->next;
        return tmp;
    }
    self& operator--() {
        node = node->prev;
        return *this;
    }
    self operator--(int) {
        self tmp = *this;
        node = node->prev;
        return tmp;
    }
};

template <typename T, typename Alloc = alloc>
class list
{
protected:
    using list_node = __list_node<T>;
    using list_node_allocator = simple_alloc<list_node, Alloc>;
public:
    using value_type = T;
    using pointer = value_type*;
    using iterator = __list_iterator<T, T&, T*>;
    using reference = value_type&;
    using size_type = size_t;
    using difference_type = ptrdiff_t;
    using const_iterator = __list_iterator<T, const T&, const T*>;
    using const_reference = const value_type&;
    using link_type = list_node*;

protected:
    link_type node;
    link_type get_node() { return list_node_allocator::allocate(); }
    void put_node(link_type p) { list_node_allocator::deallocate(p); }
    link_type create_node(const T& x) {
        link_type p = get_node();
        construct(&p->data, x);
        return p;
    }
    void destroy_node(link_type p) {
        destroy(&p->data);
        put_node(p);
    }
    void empty_initialized() {
        node = get_node();
        node->next = node;
        node->prev = node;
    }
    void transfer(iterator position, iterator first, iterator last) {
        if (position == last) return;
        last.node->prev->next = position.node;
        first.node->prev->next = last.node;
        position.node->prev->next = first.node;
        link_type tmp = position.node->prev;
        position.node->prev = last.node->prev;
        last.node->prev = first.node->prev;
        first.node->prev = tmp;
    }

public:
    iterator begin() { return node->next; }
    iterator end() { return node; }
    const_iterator begin() const { return node->next; }
    const_iterator end() const { return node; }
    bool empty() const { return node->next == node; }
    size_type size() const {
        size_type result = distance(begin(), end());
        return result;
    }
    reference front() { return *begin(); }
    reference back() { return *(--end()); }
    const_reference front() const { return *begin(); }
    const_reference back() const { return *(--end()); }

    list() { empty_initialized(); }
    ~list() { clear(), destroy_node(node); }
    list(const list&);
    list(list&&);
    template <typename InputIterator>
    list(InputIterator first, InputIterator last);
    list& operator=(const list&);

    iterator insert(iterator position, const T& x) {
        link_type tmp = create_node(x);
        tmp->next = position.node;
        tmp->prev = position.node->prev;
        position.node->prev->next = tmp;
        position.node->prev = tmp;
        return tmp;
    }
    void push_front(const T& x) { insert(begin(), x); }
    void push_back(const T& x) { insert(end(), x); }
    iterator erase(iterator position) {
        link_type next_node = position.node->next;
        link_type prev_node = position.node->prev;
        prev_node->next = next_node;
        next_node->prev = prev_node;
        destroy_node(position.node);
        return next_node;
    }
    void pop_front() { erase(begin()); }
    void pop_back() { erase(--end()); }
    void swap(list&);

    void splice(iterator position, list&);
    void splice(iterator position, list&, iterator i);
    void splice(iterator position, list&, iterator first, iterator last);
    void clear();
    void remove(const T& value);
    void unique();
    void merge(list&);
    void reverse();
    void sort();
};

template <typename T, typename Alloc>
list<T, Alloc>::list(const list& x)
{
    empty_initialized();
    for (const T& item : x)
        push_back(x);
}

template <typename T, typename Alloc>
list<T, Alloc>::list(list&& x)
{
    empty_initialized();
    swap(x);
}

template <typename T, typename Alloc>
template <typename InputIterator>
list<T, Alloc>::list(InputIterator first, InputIterator last)
{
    empty_initialized();
    for (; first != last; first++)
        push_back(*first);
}

template <typename T, typename Alloc>
list<T, Alloc>& list<T, Alloc>::operator=(const list& x)
{
    clear();
    for (const T& item : x)
        push_back(x);
    return *this;
}

template <typename T, typename Alloc>
void list<T, Alloc>::swap(list& x)
{
    std::swap(node, x.node);
}

template <typename T, typename Alloc>
void list<T, Alloc>::clear()
{
    link_type cur = node->next;
    while (cur != node) {
        link_type tmp = cur;
        cur = cur->next;
        destroy_node(tmp);
    }
    node->next = node;
    node->prev = node;
}

template <typename T, typename Alloc>
void list<T, Alloc>::remove(const T& value)
{
    iterator first = begin();
    iterator last = end();
    while (first != last) {
        iterator next = first;
        next++;
        if (*first == value)
            erase(first);
        first = next;
    }
}

template <typename T, typename Alloc>
void list<T, Alloc>::unique()
{
    if (empty()) return;
    iterator first = begin();
    iterator last = end();
    iterator next = first;
    while (++next != last) {
        if (*first == *next)
            erase(next);
        else
            first = next;
        next = first;
    }
}

template <typename T, typename Alloc>
void list<T, Alloc>::splice(iterator position, list& x)
{
    if (x.empty()) return;
    transfer(position, x.begin(), x.end());
}


template <typename T, typename Alloc>
void list<T, Alloc>::splice(iterator position, list&, iterator i)
{
    iterator j = i;
    j++;
    if (position == i or position == j)
        return;
    transfer(position, i, j);
}

template <typename T, typename Alloc>
void list<T, Alloc>::splice(iterator position, list&, iterator first, iterator last)
{
    if (first == last) return;
    transfer(position, first, last);
}

template <typename T, typename Alloc>
void list<T, Alloc>::merge(list& x)
{
    iterator first1 = begin();
    iterator last1 = end();
    iterator first2 = x.begin();
    iterator last2 = x.end();
    while (first1 != last1 and first2 != last2)
    {
        if (*first1 <= *first2) {
            first1++;
            continue;
        }
        iterator next = first2;
        transfer(first1, first2, ++next);
        first2 = next;
    }
    if (first2 != last2)
        transfer(last1, first2, last2);
}

template <typename T, typename Alloc>
void list<T, Alloc>::reverse()
{
    if (node->next == node->prev) return;
    iterator first = ++begin();
    while (first != end()) {
        iterator old = first;
        ++first;
        transfer(begin(), old, first);
    }
}

template <typename T, typename Alloc>
void list<T, Alloc>::sort()
{
    if (node->next == node->prev) return;
    list<T, Alloc> carry;
    list<T, Alloc> counter[64];
    int fill = 0;
    while (!empty())
    {
        carry.splice(carry.begin(), *this, begin());
        int i;
        for (i = 0; i < fill; i++)
        {
            if (counter[i].empty()) break;
            counter[i].merge(carry);
            carry.swap(counter[i]);
        }
        carry.swap(counter[i - 1]);
        if (i == fill) fill++;
    }

    for (int i = 1; i < fill; i++)
        counter[i].merge(counter[i - 1]);
    swap(counter[fill - 1]);
}

}